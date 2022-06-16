#pragma once
#include "Script.h"

class CCollider2D;

class CBossBulletRand :
	public CScript
{
private:
	Vec2 m_Angle;
	float m_fSpeed;
	bool m_bDead;
	PLAYER_DIR m_eDir;
	CGameObj* m_pPlayer;

public:
	virtual void Update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);

public:
	void Init(CGameObj* _player);
	void Init2();
	void SetDir(PLAYER_DIR _dir) { m_eDir = _dir; }
	CLONE(CBossBulletRand)
	CBossBulletRand();
	virtual ~CBossBulletRand();
};

