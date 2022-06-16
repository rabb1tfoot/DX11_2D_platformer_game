#pragma once
#include "State.h"
class CSamplerState :
	public CState
{
private:
	ID3D11SamplerState * m_pState;
	D3D11_SAMPLER_DESC	 m_tDesc;
	UINT				 m_iRegister;

public:
	void Create(D3D11_SAMPLER_DESC* _pDesc, UINT _iRegister);
	virtual void UpdateData();

	CLONE_DISABLE(CSamplerState);

public:
	CSamplerState();
	virtual ~CSamplerState();
};

