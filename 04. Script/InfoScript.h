#pragma once
#include "Script.h"
class CInfoScript :
	public CScript
{
private:
	int m_iPlayerHp;
	int m_iPlayerWeapon;
	int m_iActiveWeapon;
	bool m_bImmune;
	bool m_bSound;
	CResPtr<CSound> m_pSound;

public:
	virtual void Update();
	int GetPlayerHp() { return m_iPlayerHp; };
	int GetPlayerWeapon() { return m_iPlayerWeapon; }
	int GetActiveWeapon() { return m_iActiveWeapon; }
	void SetPlayerHpdown();
	void SetPlayerWeapon(int _num) { m_iPlayerWeapon = _num; }
	void SetImmune() { m_bImmune = true; }
	bool IsImmune() { return m_bImmune; }
	void SetActiveWeapon(int _num) { m_iActiveWeapon = _num; }

public:
	CInfoScript();
	virtual ~CInfoScript();
};

