#include "stdafx.h"
#include "samplerState.h"
#include "Device.h"


CSamplerState::CSamplerState()
	: m_pState(nullptr)
	, m_tDesc{}
	, m_iRegister(0)
{
}


CSamplerState::~CSamplerState()
{
	SAFE_RELEASE(m_pState);
}


void CSamplerState::Create(D3D11_SAMPLER_DESC* _pDesc, UINT _iRegister)
{
	m_iRegister = _iRegister;

	DEVICE->CreateSamplerState(_pDesc, &m_pState);
}

void CSamplerState::UpdateData()
{
	CONTEXT->VSSetSamplers(m_iRegister, 1, &m_pState);
	CONTEXT->HSSetSamplers(m_iRegister, 1, &m_pState);
	CONTEXT->DSSetSamplers(m_iRegister, 1, &m_pState);
	CONTEXT->GSSetSamplers(m_iRegister, 1, &m_pState);
	CONTEXT->CSSetSamplers(m_iRegister, 1, &m_pState);
	CONTEXT->PSSetSamplers(m_iRegister, 1, &m_pState);
}
