#pragma once
#include "Script.h"

class CGameObj;
class CCollider2D;

class CHiveScript :
	public CScript
{
private:
	CGameObj* m_pBug[3];
	float m_fAccSpawnTime[3];
	float m_fAnimTime;
	int m_iHp;

public:
	virtual void awake();
	virtual void Update();

public:
	void Init();

private:
	void CreateBug(float _Pos);
public:
	CLONE(CHiveScript);
	CHiveScript();
	virtual ~CHiveScript();
};

