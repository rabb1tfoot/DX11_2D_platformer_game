#pragma once
#include "State.h"
class CBlendState :
	public CState
{
private:
	ID3D11BlendState * m_pState;
	D3D11_BLEND_DESC   m_tDesc;
	Vec4			   m_vFactor;

public:
	void SetFactor(const Vec4& _vFactor) { m_vFactor = _vFactor; }

public:
	virtual void UpdateData();
	static void UpdateDefaultData();
	void Create(D3D11_BLEND_DESC* _pDesc);
	CLONE_DISABLE(CBlendState);

public:
	CBlendState();
	virtual ~CBlendState();
};

