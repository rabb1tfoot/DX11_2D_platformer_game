#pragma once
#include "Obj.h"
class CBullet :
	public CObj
{
public:
	CBullet();
	CBullet(SHAPE _etype);
	virtual ~CBullet();


public:
	virtual void Update();
};

