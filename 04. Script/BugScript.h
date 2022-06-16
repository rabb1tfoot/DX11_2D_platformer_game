#pragma once
#include "Script.h"
class CBugScript :
	public CScript
{
private:
	CGameObj* m_pHive;
	CGameObj* m_pPlayer;

	float m_fColliderTime;
	float m_fAnimTime;
	float m_fAttackTime;
	float m_fAngle;
	float m_fSpeed;
	bool m_bAttacking;


public:
	virtual void Update();
	virtual void OnCollision(CCollider2D* _pOther);


private:
	void AttackBullet(CGameObj * _pOther);
public:
	void SetSpeed(float _speed) { m_fSpeed = _speed; }
	void SetHive(CGameObj* _pObject) { m_pHive = _pObject; }
	void SetPlayer(CGameObj* _pObject) { m_pPlayer = _pObject; }
	CLONE(CBugScript);
	CBugScript();
	virtual ~CBugScript();
};

