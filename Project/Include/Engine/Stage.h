#pragma once

class CObj;

class CStage
{
private:
	vector<CObj*> m_vObj[UINT(OBJ_TYPE::END)];
	vector<CObj*> m_vdeleteObj;

public:
	void Enter();
	void Exit();
	void AddObj(CObj* _obj, OBJ_TYPE _etype);
	void AddDeleteObj(CObj* _Obj);
	void UpdateRender();


public:
	CStage();
	~CStage();
};

