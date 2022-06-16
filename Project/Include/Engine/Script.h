#pragma once
#include "Component.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"

#include "GameObj.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Animation2D.h"
#include "Animator2D.h"

#include "Scene.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "Prefab.h"

class CScript :
	public CComponent
{
private:
	UINT m_iScriptType;
public:
	virtual void Update() {}
	virtual void awake() {}
	virtual void ReleaseScript() {};

public:
	UINT GetScriptType() { return m_iScriptType; }
	virtual void SaveToScene(FILE* _pFile) {};
	virtual CComponent* LoadFromScene(FILE* _pFile) { return nullptr; };

protected:
	void Instantiate(CResPtr<CPrefab>& _pPrefab, const Vec3& _vPos, const wstring& _strLayerName);

	virtual void OnCollisionEnter(CCollider2D* _pOther) {};
	virtual void OnCollision(CCollider2D* _pOther) {};
	virtual void OnCollisionExit(CCollider2D* _pOther) {};

	void AddChild(CGameObj* _pChildObj);
	void ClearChild(UINT _iChildIdx);
	void ClearParent();

	void CreateObject(CGameObj* _pTarget);
	void DeleteObject(CGameObj* _pTarget);

public:
	CScript(UINT _iType);
	virtual ~CScript();

	friend class CCollider2D;
};

