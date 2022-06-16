#pragma once
#include "Script.h"
class CTitleGround :
	public CScript
{
private:
	float m_fAccTime;

public:
	virtual void Update();
public:
	CLONE(CTitleGround);
	CTitleGround();
	virtual ~CTitleGround();
};

