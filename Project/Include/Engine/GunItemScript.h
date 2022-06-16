#pragma once
#include "Script.h"
class CCollider2D;
class CGunItemScript :
	public CScript
{
private:
	int m_iGunType;
	bool m_bDead;
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);
public:
	void SetGunType(int _type) { m_iGunType = _type; }
public:
	CGunItemScript();
	virtual ~CGunItemScript();
};

