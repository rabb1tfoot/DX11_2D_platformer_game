#pragma once
#include <Script.h>
#include <ResPtr.h>
#include <Sound.h>
class CTraceScript : public CScript
{
private:
	Vec3 m_vSize;
	PLAYER_DIR m_eType;
	PLAYER_DIR m_eTypeEyesight;
	PLAYER_DIR m_eAim;
	PLAYER_KEY m_eOldKey;
	float m_fAccHitTime;
	float m_fSpeed;
	int m_iTouchCount;
	float m_fHp;
	Vec3 m_vForce{};
	bool m_bIsAir;
	bool m_bHitted;
	bool m_bPause;
	bool m_bArmed;
	bool m_bUpAnimed;
	bool m_bDownJump;
	bool m_bDead;
	bool m_AbleGunIcon;
	int m_iMaxhp;
	int m_iCurhp;
	int m_iGunType;
	int m_iTryShootTime;
	CScript* m_pScript;
	CGameObj* m_pHpObject;
	CGameObj* m_pGunUI;
	CGameObj* m_pBullet;
	CResPtr<CSound> m_pSound;

public:
	virtual void awake();
	virtual void Update();
	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

public:
	void SetSize(Vec3 _vsize) { m_vSize = _vsize; };
	Vec3 GetSize() { return m_vSize; }
	void SetInfoScript(CScript* _infoscript) { m_pScript = _infoscript; }
	PLAYER_DIR GetPlayerDir() { return m_eType; }
	void Hitted();
	void Pause() { m_bPause = true; };
	void UnPause() { m_bPause = false; };
	void Armed() { m_bArmed = true; };
	void Dead() { m_bDead = true; }
	void SetBulletDead() { m_pBullet = nullptr; }
	bool GetGunIcon() {return m_AbleGunIcon; }
	void CreateGunIcon();

	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);

private:
	void CheckAnim();
	void CreateBullet();
	void GameOver();
	void CreateHpBar();
	void CreateGunUI();

private:
		CLONE(CTraceScript);

public:
	CTraceScript();
	virtual ~CTraceScript();
};

