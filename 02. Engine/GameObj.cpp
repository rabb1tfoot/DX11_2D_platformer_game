#include "stdafx.h"
#include "GameObj.h"
#include "Transform.h"
#include "MeshRender.h"
#include "script.h"
#include "EventMgr.h"
#include "Core.h"

CGameObj::CGameObj()
	: m_arrCom{}
	, m_bDead(false)
	, m_iLayerIdx(-1)
	, m_pParent(nullptr)
	, m_bActive(true)
{
}

CGameObj::CGameObj(const CGameObj & _origin)
	: CEntity(_origin)
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_iLayerIdx(-1)
	, m_bActive(true)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _origin.m_arrCom[i])
			continue;

		AddComponent(_origin.m_arrCom[i]->Clone());
	}

	for (UINT i = 0; i < _origin.m_vecScript.size(); ++i)
	{
		AddComponent(_origin.m_vecScript[i]->Clone());
	}

	for (UINT i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}
}


CGameObj::~CGameObj()
{
	Safe_Delete_Array(m_arrCom);
	Safe_Delete_Vec(m_vecScript);
	Safe_Delete_Vec(m_vecChild);

	//for (UINT i = 0; i < m_vecScript.size(); ++i)
	//{
	//	m_vecScript[i]->ReleaseScript();
	//}
}


void CGameObj::awake()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->awake();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->awake();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->awake();
	}
}

void CGameObj::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObj::Update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->Update();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Update();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Update();
	}
}

void CGameObj::Lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->Lateupdate();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Lateupdate();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Lateupdate();
	}
}

void CGameObj::Finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->Finalupdate();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Finalupdate();
	}

	// Layer �� Object ���
	// ����� �س��� ���������� �θ� �ƴ� ������Ʈ�鵵 �������� ����ȴ�.
	if (!GetDead())
	{
		CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
		pCurLayer->RegisterObj(this);
	}
}

void CGameObj::OnEnable()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->OnEnable();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->OnEnable();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->OnEnable();
	}
}

void CGameObj::OnDisable()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->OnDisable();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->OnDisable();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->OnDisable();
	}
}

void CGameObj::Render()
{
	if (!MeshRender())
		return;

	((CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER])->Render();
}

void CGameObj::AddComponent(CComponent * _comp)
{
	COMPONENT_TYPE eType = _comp->GetComponentType();

	if (COMPONENT_TYPE::SCRIPT == eType)
	{
		m_vecScript.push_back((CScript*)_comp);
	}
	else
	{
		assert(!m_arrCom[(UINT)eType]);
		m_arrCom[(UINT)eType] = _comp;
	}

	_comp->SetGameObject(this);
}

void CGameObj::AddChild(CGameObj * _pChildObj)
{
	// 1. Child Object �� �θ� �ִ� ���  
	//	 - ���� �θ� ��Ͽ��� ���� �Ǿ�� ��
	if (_pChildObj->GetParent())
	{
		// ���� �θ� �ٽ� �ڽ����� ������ ���(�ߺ� �̺�Ʈ ó�� �߻�)
		assert(_pChildObj->GetParent() != this);

		_pChildObj->ClearParent();
	}
	else
	{
		// �ڽ����� �������� ������Ʈ�� �ֻ��� �θ� ������Ʈ�ε�, Ư�� Layer �� �����ִ°��
		// Layer �� �����ϵ�, �θ������Ʈ�μ��� ����� ����
		if (_pChildObj->GetLayerIdx() != -1)
		{
			CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(_pChildObj->GetLayerIdx());

			// Scene Load �߿��� ����ó�� �˻� ����
			if (SCENE_STATE::LOAD != CCore::GetInst()->GetState())
				pCurLayer->RemoveParent(_pChildObj);
		}
	}

	m_vecChild.push_back(_pChildObj);
	_pChildObj->SetParent(this);
}

void CGameObj::ClearParent()
{
	if (!m_pParent)
		return;

	vector<CGameObj*>::iterator iter = m_pParent->m_vecChild.begin();
	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if (this == *iter)
		{
			m_pParent->m_vecChild.erase(iter);
			break;
		}
	}

	m_pParent = nullptr;

	// �ֻ��� ������Ʈ�� �Ǿ��� ������, �ش� ���̾ �ֻ��� ������Ʈ�μ� ��� �ȴ�.
	if (!m_bDead)
	{
		CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(GetLayerIdx());
		pCurLayer->AddObject(this, true);
	}

}

void CGameObj::ClearLayer()
{
	// 1. Layer �� �������� �ʴ´�.
	if (-1 == m_iLayerIdx)
		return;

	// 2. �θ� ������Ʈ�� ������.	
	if (!m_pParent)
	{
		CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
		pCurLayer->RemoveParent(this);
	}

	m_iLayerIdx = -1;
}

void CGameObj::Disconnect()
{
	if (m_pParent)
	{
		ClearParent();
	}
	else
	{
		ClearLayer();
	}
}

bool CGameObj::IsAncestor(CGameObj* _pTarget)
{
	CGameObj* myParent = m_pParent;

	while (myParent)
	{
		if (myParent == _pTarget)
			return true;

		myParent = myParent->m_pParent;
	}
	return false;
}

void CGameObj::SetActive(bool _bool)
{
	if (m_bActive == _bool)
		return;

	m_bActive = _bool;

	tEvent event{};

	if (m_bActive)
		event.eType = EVENT_TYPE::ON_ENABLE;
	else
		event.eType = EVENT_TYPE::ON_DISABLE;

	event.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(event);
}

void CGameObj::SaveToScene(FILE * _pFile)
{
	fwrite(&m_iLayerIdx, sizeof(int), 1, _pFile);
}

void CGameObj::LoadFromScene(FILE * _pFile)
{
	fread(&m_iLayerIdx, sizeof(int), 1, _pFile);
}

void CGameObj::ClearChild(UINT _iChildIdx)
{
	assert(!(m_vecChild.size() <= _iChildIdx));

	CGameObj* pChild = m_vecChild[_iChildIdx];

	pChild->ClearParent();
}

CScript * CGameObj::FindScript(UINT iIdx)
{
	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->GetScriptType() == iIdx)
			return m_vecScript[i];
	}
}

