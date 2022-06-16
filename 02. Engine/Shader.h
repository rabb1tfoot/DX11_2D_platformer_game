#pragma once
#include "ReSource.h"

class CBlendState;

struct tPramInfo
{
	SHADER_PARAM eType;
	wchar_t		 szName[50];
	bool		 m_bAccess;
};

class CShader :
	public CReSource
{
private:
	ID3DBlob * m_pVSBlob;
	ID3DBlob * m_pHSBlob;
	ID3DBlob * m_pDSBlob;
	ID3DBlob * m_pGSBlob;
	ID3DBlob * m_pPSBlob;
	ID3DBlob * m_pErrBlob;

	ID3D11VertexShader *	 m_pVShader;
	ID3D11HullShader *		 m_pHShader;
	ID3D11DomainShader *	 m_pDShader;
	ID3D11GeometryShader *	 m_pGShader;
	ID3D11PixelShader *		 m_pPShader;

	CBlendState*			m_pBlendState;

	vector<tPramInfo>		m_vecParam;

public:
	ID3DBlob * GetVSBlob() { return m_pVSBlob; }
	ID3DBlob * GetHSBlob() { return m_pHSBlob; }
	ID3DBlob * GetDSBlob() { return m_pDSBlob; }
	ID3DBlob * GetGSBlob() { return m_pGSBlob; }
	ID3DBlob * GetPSBlob() { return m_pPSBlob; }

	ID3D11VertexShader* GetVShader() { return m_pVShader; }
	ID3D11HullShader* GetHShader() { return m_pHShader; }
	ID3D11DomainShader* GetDShader() { return m_pDShader; }
	ID3D11GeometryShader* GetGShader() { return m_pGShader; }
	ID3D11PixelShader* GetPShader() { return m_pPShader; }

	void SetBlendState(CBlendState* _pBlendState) { m_pBlendState = _pBlendState; }

public:
	void CreateVertexShader(const wstring& _strFilePath, const string& _strFuncName);
	//void CreateHullShader(const wstring& _strFilePath, const string& _strFuncName);
	//void CreateDomainShader(const wstring& _strFilePath, const string& _strFuncName);
	//void CreateGeometryShader(const wstring& _strFilePath, const string& _strFuncName);
	void CreatePixelShader(const wstring& _strFilePath, const string& _strFuncName);

	void UpdateData();

	void AddParam(SHADER_PARAM _eType, const wstring& _strMean, bool _bAccess = true)
	{
		tPramInfo param;
		param.eType = _eType;
		param.m_bAccess = _bAccess;
		wcscpy_s(param.szName, 50, _strMean.c_str());
		m_vecParam.push_back(param);
	}

	const vector<tPramInfo>& GetParamInfo() { return 	m_vecParam; }

public:
	CShader(RES_TYPE _etype);
	virtual ~CShader();
};

