#include "stdafx.h"
#include "Texture.h"

#include "Device.h"
#include "ResMgr.h"

CTexture::CTexture(RES_TYPE _etype)
	: CReSource(_etype)
	, m_pSRV(nullptr)
	, m_pTex2D(nullptr)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pTex2D);
}

void CTexture::Load(const wstring & _strFilePath)
{
	// Texture 를 SystemMem 에 Loading
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);
	
	HRESULT hRet = S_OK;

	// dds
	if (!wcscmp(szExt, L".dds") && !wcscmp(szExt, L".DDS"))
	{
		hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}
	// tga
	else if (!wcscmp(szExt, L".tga") && !wcscmp(szExt, L".TGA"))
	{
		hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	else // png, jpg, jpeg, bmp
	{
		hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, &m_pSRV)))
	{
		MessageBox(nullptr, L"Fail Texture Load", L"텍스쳐 로딩 실패", MB_OK);
	}
			
	m_pSRV->GetResource(((ID3D11Resource**)&m_pTex2D));
	
}

void CTexture::SetRegister(UINT _iRegister, UINT _iShaderType)
{
	if((UINT)SHADER_TYPE::VERTEX_SHADER & _iShaderType)
		CONTEXT->VSSetShaderResources(_iRegister, 1, &m_pSRV);
	if((UINT)SHADER_TYPE::PIXEL_SHADER & _iShaderType)
		CONTEXT->PSSetShaderResources(_iRegister, 1, &m_pSRV);
}

void CTexture::SetRegisterAll(UINT _iRegister)
{
	CONTEXT->VSSetShaderResources(_iRegister, 1, &m_pSRV);
	CONTEXT->PSSetShaderResources(_iRegister, 1, &m_pSRV);
}

bool CTexture::LoadFromScene(FILE * _pFile)
{
	CReSource::LoadFromScene(_pFile);

	CResPtr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(GetName().c_str());

	if (nullptr != pTex)
		return false;

	wstring strPath = CPathMgr::GetResPath();
	strPath += GetPath();
	Load(strPath);

	return true;
}

void CTexture::ClearRegister(UINT _iRegister, UINT _iShaderType)
{
	ID3D11ShaderResourceView* pView = nullptr;

	if ((UINT)SHADER_TYPE::VERTEX_SHADER & _iShaderType)
		CONTEXT->VSSetShaderResources(_iRegister, 1, &pView);
	if ((UINT)SHADER_TYPE::PIXEL_SHADER & _iShaderType)
		CONTEXT->PSSetShaderResources(_iRegister, 1, &pView);	
}