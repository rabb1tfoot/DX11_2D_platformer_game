#pragma once

#include <DirectXTex.h>
#include <DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug")
#else
#pragma comment(lib, "DirectXTex")
#endif

#include "ReSource.h"
class CTexture :
	public CReSource
{
private:
	ID3D11ShaderResourceView * m_pSRV;
	ID3D11Texture2D*			m_pTex2D;
	ScratchImage				m_Image;

public:
	virtual void Load(const wstring& _strFilePath);

public:
	float GetWidth() {return (float)m_Image.GetMetadata().width; }
	float GetHeight() {return (float)m_Image.GetMetadata().height; }

public:
	void SetRegister(UINT _iRegister, UINT _iShaderType);
	void SetRegisterAll(UINT _iRegister);

	virtual bool LoadFromScene(FILE* _pFile);

	static void ClearRegister(UINT _iRegister, UINT _iShaderType);

public:
	CTexture(RES_TYPE _etype);
	virtual ~CTexture();
};

