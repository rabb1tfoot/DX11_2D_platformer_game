#pragma once
#include "Script.h"
class CBlackoutScript :
	public CScript
{
private:
	bool m_bActiveFadeOff;
	bool m_bActiveFadeIn;
	float m_fAccTime;
	int m_iActiveNum;
public:
	virtual void Update();

	void SetFadeOff() { m_bActiveFadeOff = true; }
	void SetFadeIn() { m_bActiveFadeIn = true; }

public:
	CLONE(CBlackoutScript);
	CBlackoutScript();
	virtual ~CBlackoutScript();
};

