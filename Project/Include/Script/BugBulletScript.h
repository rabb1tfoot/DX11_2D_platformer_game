#pragma once
#include "Script.h"

class CGameObj;

class CBugBulletScript :
	public CScript
{
private:
	CGameObj* m_pPlayer;
	Vec2 m_Angle;

	bool m_bDead;
	bool m_bActive;
public:
	virtual void Update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);

public:
	void Init(CGameObj* _player);
	void SetActive() { m_bActive = true; };
	bool GetActive() { return m_bActive; }
	CLONE(CBugBulletScript);
	CBugBulletScript();
	virtual ~CBugBulletScript();
};

