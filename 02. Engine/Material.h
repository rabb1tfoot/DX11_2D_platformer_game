#pragma once
#include "ReSource.h"

#include "ResPtr.h"
#include "Shader.h"
#include "Texture.h"

class CMaterial :
	public CReSource
{
private:
	CResPtr<CShader>  m_pShader;
	tShaderParam      m_tParam;
	tTileParam		  m_tTileParam;
	CResPtr<CTexture> m_arrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
	bool			  m_bTileRender;

public:
	CResPtr<CShader> GetShader() { return m_pShader; }
	void SetShader(CResPtr<CShader> _pShader);
	void SetData(SHADER_PARAM _eType, void* _pSrc);
	const tShaderParam& GetData() { return m_tParam; }
	const tTileParam& GetTileData() { return m_tTileParam; }
	CResPtr<CTexture>* GetTexData() { return m_arrTex; }
	virtual void Load(const wstring& _strFilePath);
	virtual void Save();

	virtual bool LoadFromScene(FILE* _pFile);

public:
	void UpdateData();

public:
	CLONE(CMaterial);

public:
	CMaterial(RES_TYPE _etype);
	virtual ~CMaterial();
};

