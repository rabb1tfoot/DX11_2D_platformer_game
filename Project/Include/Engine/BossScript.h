#pragma once
#include "Script.h"

enum class BossState
{
	STATE_BOMB,
	STATE_RAND,
};

class CBossScript :
	public CScript
{
private:
	Vec3 m_vFirstPos;
	int m_iTakeDmg;
	float m_fAccTransTime;
	float m_fAccBulletTime;
	float m_fModeChange;
	float m_fSpeedX;
	float m_fSpeedY;
	BossState m_eType;
	PLAYER_DIR m_eDir;
	CResPtr<CSound> m_pSound;

public:
	void Init();
	virtual void awake();
	virtual void Update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);

private:
	void CreateBullet();

public:
	CLONE(CBossScript);
	CBossScript();
	virtual ~CBossScript();
};

