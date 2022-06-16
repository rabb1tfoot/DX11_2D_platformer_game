#pragma once
#include "Script.h"
class CCameraScipt :
	public CScript
{
private:
	float m_fSpeed;
	float m_fAccTime;
	bool m_bChasePlayer;
	bool m_bSceneMove;
	Vec2 m_vSceneMovePoint;
	wstring m_vChangeSceneName;
	Vec2 m_vCamBoundLT;
	Vec2 m_vCamBoundRB;

public:
	virtual void Update();
	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);
	void SetChasing() { m_bChasePlayer = true; }
	void SetSceneMove(Vec2 _vDirect, wstring _wSceneName);
	void SetBoundVec(Vec2* _vBound);

public:
	CLONE(CCameraScipt);
	CCameraScipt();
	virtual ~CCameraScipt();

private:
	void ChasingPlayer();
	void SceneMove();
};

