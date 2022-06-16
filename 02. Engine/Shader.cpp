#include "stdafx.h"
#include "Shader.h"
#include "PathMgr.h"
#include "Device.h"
#include "BlendState.h"


CShader::CShader(RES_TYPE _etype)
	: CReSource(_etype)
	, m_pVSBlob(nullptr)
	, m_pHSBlob(nullptr)
	, m_pDSBlob(nullptr)
	, m_pGSBlob(nullptr)
	, m_pPSBlob(nullptr)
	, m_pErrBlob(nullptr)
	, m_pVShader(nullptr)
	, m_pHShader(nullptr)
	, m_pDShader(nullptr)
	, m_pGShader(nullptr)
	, m_pPShader(nullptr)
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pDSBlob);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pErrBlob);

	SAFE_RELEASE(m_pVShader);
	SAFE_RELEASE(m_pHShader);
	SAFE_RELEASE(m_pDShader);
	SAFE_RELEASE(m_pGShader);
	SAFE_RELEASE(m_pPShader);
}


void CShader::CreateVertexShader(const wstring & _strFilePath, const string & _strFuncName)
{
	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strFilePath;

	D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str()
		, "vs_5_0", iFlag, 0, &m_pVSBlob, &m_pErrBlob);

	if(nullptr != m_pErrBlob)
		OutputDebugStringA((char*)m_pErrBlob->GetBufferPointer());

	DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr
		, &m_pVShader);
}

void CShader::CreatePixelShader(const wstring & _strFilePath, const string & _strFuncName)
{
	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strFilePath;

	D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str()
		, "ps_5_0", iFlag, 0, &m_pPSBlob, &m_pErrBlob);
	if (nullptr != m_pErrBlob)
		OutputDebugStringA((char*)m_pErrBlob->GetBufferPointer());

	DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr
		, &m_pPShader);
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_pVShader, nullptr, 0);
	CONTEXT->HSSetShader(m_pHShader, nullptr, 0);
	CONTEXT->DSSetShader(m_pDShader, nullptr, 0);
	CONTEXT->GSSetShader(m_pGShader, nullptr, 0);
	CONTEXT->PSSetShader(m_pPShader, nullptr, 0);

	if (m_pBlendState)
		m_pBlendState->UpdateData();
	else
		CBlendState::UpdateDefaultData();
}
