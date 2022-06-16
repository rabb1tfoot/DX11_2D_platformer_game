#pragma once
#include "Obj.h"
class CEnemy :
	public CObj
{
public:
	CEnemy();
	CEnemy(SHAPE _etype);
	virtual ~CEnemy();

private:
	int m_iDir;
	float m_fReload;
	bool m_bShoot;

public:
	virtual void Update();

private:
	void CreateBullet();
};

