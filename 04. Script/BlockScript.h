#pragma once
#include "Script.h"

class CGameObj;
class CCollider2D;

class CBlockScript :
	public CScript
{
private:
	CGameObj* m_pBlockingObj;
	wstring m_wBlockName;
	bool m_bDead;

public:
	virtual void awake();
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

public:
	void SetBlocking(CGameObj* _pObject) { m_pBlockingObj = _pObject; }
	CGameObj* GetBlocking() {return m_pBlockingObj; }
public:
	CLONE(CBlockScript);
	CBlockScript();
	virtual ~CBlockScript();
};

