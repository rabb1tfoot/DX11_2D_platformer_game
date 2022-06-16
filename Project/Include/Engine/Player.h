#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
private:
	float m_fReload;
	bool  m_bShoot;

public:
	CPlayer();
	CPlayer(SHAPE _etype);
	virtual ~CPlayer();

public:
	virtual void Update();

private:
	void CreateBullet();
};

