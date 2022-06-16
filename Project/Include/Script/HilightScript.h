#pragma once
#include "Script.h"
class CHilightScript :
	public CScript
{
private:
	int m_iClearEffOn;
	float m_fClearColor;

	float m_fAccTime;
	float m_fdurTime;

	bool m_bHittedOn;

public:
	virtual void Update();


public:
	void EffectHittedOn() { m_bHittedOn = true; }
	void EffectHittedOff() { m_bHittedOn = false; }

public:
	CLONE(CHilightScript);

public:
	CHilightScript();
	virtual ~CHilightScript();
};

