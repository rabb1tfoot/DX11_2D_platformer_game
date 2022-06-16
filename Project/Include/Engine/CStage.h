#pragma once

class CObj;

class CStage
{
private:
	vector<CObj*> m_vObj;

public:
	void Enter();
	void Exit();
	void AddObj(CObj* _obj);
	void Update();
	void Render();
public:
	CStage();
	~CStage();
};

