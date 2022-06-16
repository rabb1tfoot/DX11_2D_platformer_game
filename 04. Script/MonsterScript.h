#pragma once
#include "Script.h"
class CMonsterScript :
	public CScript
{
private:
	int m_iMaxhp;
	int m_iCurhp;
	int m_iEffOn;
	float m_fAccTime;
	bool m_bHitEff;
	bool m_bDestroyEff;
	bool m_bDead;


public:
	virtual void Update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

public:
	void SetDestoryEff(bool _des) { m_bDestroyEff = _des; }
	bool GetDestoryEff() { return m_bDestroyEff; }
	void HittedFunc();
	void SetHp(int _hp) { m_iMaxhp = _hp; m_iCurhp = _hp; }

	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

