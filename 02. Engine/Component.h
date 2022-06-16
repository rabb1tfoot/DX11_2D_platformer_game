#pragma once
#include "GameObj.h"


class CComponent :
	public CEntity
{
private:
	CGameObj * m_pObj;
	COMPONENT_TYPE m_eComType;

public:
	virtual void awake() {};
	virtual void start() {};

	virtual void Update() = 0;
	virtual void Lateupdate() {};
	virtual void Finalupdate() {};

	virtual void OnEnable() {};
	virtual void OnDisable() {};

	COMPONENT_TYPE GetComponentType() { return m_eComType; }
	CGameObj * Object() { return m_pObj; }
	CGameObj* ParentObject() { return m_pObj->GetParent(); }
	bool HasParent() { return m_pObj->GetParent(); }

	virtual void SaveToScene(FILE* _pFile) = 0;
	virtual CComponent* LoadFromScene(FILE* _pFile) = 0;

protected:
	void SetComponentType(COMPONENT_TYPE _etype) { m_eComType = _etype; }
	CTransform* Transform() { return m_pObj->Transform(); }
	CMeshRender* Meshrender() { return m_pObj->MeshRender(); }
	CCollider2D* Collider2D() { return m_pObj->Collider2D(); }
	CCamera* Camera() { return m_pObj->Camera(); }
	CAnimator2D* Animator2D() { return m_pObj->Animator2D(); }

private:
	void SetGameObject(CGameObj* _obj) { m_pObj = _obj; }

	CLONE_DISABLE(CComponent);

public:
	CComponent(COMPONENT_TYPE _etype);
	CComponent(const CComponent& _com);
	virtual ~CComponent();

	friend class CGameObj;
};

