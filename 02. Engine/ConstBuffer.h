#pragma once
#include "Entity.h"
class CConstBuffer :
	public CEntity
{
private:
	ID3D11Buffer * m_pBuffer;
	UINT		   m_iSize;   //버퍼 최대사이즈
	UINT		   m_iRegister;

	void*		   m_pSysMem;
	UINT		   m_iCurSize; //현재 버퍼 사이즈

public:
	void Create(UINT _iSize, UINT _iRegisterNum);
	void AddData(void* _psrc, UINT _iSize);
	void UpdateData();
	void SetRegister(UINT _iShaderType);
	void SetRegisterAll();

public:
	CLONE_DISABLE(CConstBuffer);

public:
	CConstBuffer();
	virtual ~CConstBuffer();
};

