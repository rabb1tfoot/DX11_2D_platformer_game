#pragma once

class CGameObj;

class CEventMgr
{
	SINGLE(CEventMgr);

	vector<tEvent> m_vecEvent;
	vector<CGameObj*> m_vecDeleteObj;
	vector<CGameObj*> m_vecNewObj;

public:
	void Update();

public:
	void AddEvent(tEvent& _event) { m_vecEvent.push_back(_event); }
};

