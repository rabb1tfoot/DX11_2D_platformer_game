#pragma once
#include <Script.h>

class CGameObj;

class CBulletScript :
	public CScript
{
private:
	float m_fSpeed;
	float m_fAttachTime; //터치한 시간
	float m_fDesTime;
	bool m_bAttach;
	bool m_bDestroy;
	PLAYER_DIR m_eType;
	bool m_bDead;
	int m_iGunType;
	CGameObj* m_pPlayer;
public:
	virtual void Update();

public:
	void SetAttach() { m_bAttach = true; }
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDir(PLAYER_DIR _etype) { m_eType = _etype; }
	void SetGunType(int _type) { m_iGunType = _type; }
	void SetPlayer(CGameObj* _pPlayer) { m_pPlayer = _pPlayer; }
	void laterDestroy();

	virtual void OnCollisionEnter(CCollider2D* _pOther);
	CLONE(CBulletScript);

public:
	CBulletScript();
	virtual ~CBulletScript();
};

