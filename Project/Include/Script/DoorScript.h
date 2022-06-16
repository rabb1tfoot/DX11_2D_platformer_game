#pragma once
#include "Script.h"
class CDoorScript :
	public CScript
{
private:
	Vec2 m_vMovePoint;
	wstring m_wSceneName;

public:
	void SetMoveScenePoint(Vec2 _vPoint, wstring _wSceneName);
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

public:
	CLONE(CDoorScript);
	CDoorScript();
	virtual ~CDoorScript();
};

