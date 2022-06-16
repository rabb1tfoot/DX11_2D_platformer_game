#pragma once
#include "Entity.h"

class CGameObj;

class CLayer :
	public CEntity
{
private:
	vector<CGameObj*> m_vecParentObj;
	vector<CGameObj*> m_vecObj;
	UINT m_iLayerIdx;

public:
	void awake();
	void start();

	void Update();
	void Lateupdate();
	void Finalupdate();

	void Render();

public:
	void AddObject(CGameObj* _pObj, bool _bMoveAll, bool _bLoad = false);
	UINT GetLayerIdx() { assert(this); return m_iLayerIdx; }

private:
	void SetLayerIdx(UINT _iIdx) { m_iLayerIdx = _iIdx; }
	void RegisterObj(CGameObj* _pObj) { m_vecObj.push_back(_pObj); }
	void RemoveParent(CGameObj* _pObj);
	void ClearObject();

public:
	const vector<CGameObj*>& GetParentObject() { return m_vecParentObj; }
	const vector<CGameObj*>& GetAllObject() { return m_vecObj; }

	CLONE_DISABLE(CLayer);
public:
	CLayer();
	virtual ~CLayer();

	friend class CScene;
	friend class CGameObj;
	friend class CEventMgr;
};

