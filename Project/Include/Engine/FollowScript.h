#pragma once
#include "Script.h"

class CGameObj;

class CFollowScript :
	public CScript
{
private:
	wstring m_wFollowedName;
	CGameObj* m_pObject;
public:
	virtual void awake();
	virtual void Update();

public:
	void SetFollowName(wstring _name) { m_wFollowedName = _name; }
public:
	CLONE(CFollowScript);
	CFollowScript();
	virtual ~CFollowScript();
};

