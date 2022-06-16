#pragma once
#include "ReSource.h"
#include "ResPtr.h"
#include "Shader.h"
class CMesh :
	public CReSource
{
private:
	ID3D11Buffer *					  m_pVB;
	ID3D11Buffer *					  m_pIB;
									 
	UINT							 m_iVtxSize;
	UINT							 m_iVtxCount;
	D3D11_USAGE						 m_eVtxUsage;
									 
	UINT							 m_iIdxSize;
	UINT							 m_iIdxCount;
									 
	void*							 m_pVtxSysMem;
	void*							 m_pIdxSysMem;
									 
	D3D11_PRIMITIVE_TOPOLOGY		 m_eTopology;
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecLayout;
	ID3D11InputLayout*				 m_pLayout;

public:

	void* GetVtxSysMem() { return m_pVtxSysMem; }
	void* GetIdxSysMem() { return m_pIdxSysMem; }

	void CreateMesh(UINT _iVtxSize, UINT _iVtxCount, D3D11_USAGE _eVtxUsage, void* _pVtxSysMem
		, UINT _iIdxSize, UINT _iIdxCount, void* _pIdxSysMem, D3D11_PRIMITIVE_TOPOLOGY _etopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void SetLayout(CResPtr<CShader> _pShader);
	void Render();

	virtual bool LoadFromScene(FILE* _pFile);

public:
	CMesh(RES_TYPE _etype);
	virtual ~CMesh();
};

