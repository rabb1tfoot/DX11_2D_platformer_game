#pragma once
#include "Script.h"

class CCollider2D;

class CBossBulletPara :
	public CScript
{
private:
	bool m_bDead;
	bool m_bIsAir;
	bool m_bDown;
	float m_fForce;
	float m_fAccTime;
	PLAYER_DIR m_eDIR;

public:
	virtual void Update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);

public:
	void SetDir(PLAYER_DIR _etype) { m_eDIR = _etype; }
public:
	CLONE(CBossBulletPara)
	CBossBulletPara();
	virtual ~CBossBulletPara();
};

