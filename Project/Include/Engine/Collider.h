#pragma once

class CObj;

class CCollider
{
private:
	Vec3 m_fLT;
	Vec3 m_fRB;
	bool  m_bOnCollision;

public:
	bool IsCollision(OBJ_TYPE _etype, OBJ_TYPE _etype2);

public:
	Vec3 GetLT() { return m_fLT; }
	Vec3 GetRB() { return m_fRB; }

public:
	CCollider();
	CCollider(CObj* _obj);
	~CCollider();
};

