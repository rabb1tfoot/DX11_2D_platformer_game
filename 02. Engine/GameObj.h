#pragma once
#include "Entity.h"

class CComponent;
class CTransform;
class CMeshRender;
class CCamera;
class CScript;
class CCollider2D;
class CAnimator2D;

class CGameObj :
	public CEntity
{
private:
	CGameObj * m_pParent;

	CComponent * m_arrCom[(UINT)COMPONENT_TYPE::END];
	UINT		 m_iLayerIdx;
	bool m_bDead;
	bool m_bActive;

	vector<CScript*> m_vecScript;
	vector<CGameObj*> m_vecChild;

public:
	void awake();
	void start();

	void Update();
	void Lateupdate();
	void Finalupdate();

	void OnEnable();
	void OnDisable();

	void Render();

public:
	void AddComponent(CComponent* _comp);
	CTransform* Transform() { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	CMeshRender* MeshRender() { return (CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]; }
	CCamera*     Camera() { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }
	CCollider2D* Collider2D() { return (CCollider2D*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]; }
	CAnimator2D* Animator2D() { return (CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATION2D]; }
	CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType];}
	vector<CScript*>& GetScripts() { return m_vecScript; }

	int GetLayerIdx() { return m_iLayerIdx; }

public:
	void AddChild(CGameObj* _pChildObj);
	CGameObj* GetParent() { return m_pParent; }

	const vector<CGameObj*>& GetChild() {return m_vecChild;}
	bool IsAncestor(CGameObj* _pTarget);

	void SetActive(bool _bool);
	bool GetActive() { return m_bActive; }

	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

private:
	void SetLayerIdx(UINT _iIdx) { m_iLayerIdx = _iIdx; }
	void SetParent(CGameObj* _pParent) { m_pParent = _pParent; }
	void ClearParent();
	void ClearLayer();

	void SetDead() { m_bDead = true; }
	bool GetDead() { return m_bDead; }

	void ClearChild(UINT _iChildIdx);
	void Disconnect();

public:
	template<typename T>
	CScript* GetScript();
	CScript * FindScript(UINT iIdx);

public:
	CLONE(CGameObj);
	CGameObj(const CGameObj& _origin);
public:
	CGameObj();
	virtual ~CGameObj();

	friend class CLayer;
	friend class CEventMgr;
};

template<typename T>
inline CScript * CGameObj::GetScript()
{
	//const type_info& info = typeid(T);

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		if (dynamic_cast<T*>(m_vecScript[i]))
			return m_vecScript[i];
	}

	return nullptr;
}