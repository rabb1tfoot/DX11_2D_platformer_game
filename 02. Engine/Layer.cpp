#include "stdafx.h"
#include "Layer.h"
#include "Core.h"
#include "GameObj.h"
#include "Transform.h"
#include "SceneMgr.h"
#include "Scene.h"

CLayer::CLayer()
	: m_vecParentObj{}
	, m_vecObj{}
{
}


CLayer::~CLayer()
{
	Safe_Delete_Vec(m_vecParentObj);

}


void CLayer::awake()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
	{
		m_vecParentObj[i]->awake();
	}
}

void CLayer::start()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
	{
		m_vecParentObj[i]->start();
	}
}

void CLayer::Update()
{
	for(UINT i=0; i<m_vecParentObj.size(); ++i)
	{
		if (m_vecParentObj[i]->GetActive())
			m_vecParentObj[i]->Update();
	}
}

void CLayer::Lateupdate()
{
	for (UINT i = 0; i<m_vecParentObj.size(); ++i)
	{
		if (m_vecParentObj[i]->GetActive())
			m_vecParentObj[i]->Lateupdate();
	}
}

void CLayer::Finalupdate()
{
	for (UINT i = 0; i<m_vecParentObj.size(); ++i)
	{
		if (m_vecParentObj[i]->GetActive())
			m_vecParentObj[i]->Finalupdate();
	}
}

void CLayer::Render()
{
	for (UINT i = 0; i<m_vecObj.size(); ++i)
	{
		if (m_vecObj[i]->GetActive())
			m_vecObj[i]->Render();
	}
	m_vecObj.clear();
}

void CLayer::AddObject(CGameObj * _pObj, bool _bMoveAll, bool _bLoad)
{
	// 1. Object 가 최상위 부모 오브젝트인가?
	if (!_pObj->GetParent())
	{
		m_vecParentObj.push_back(_pObj);
	}
	if(!_bLoad)
		_pObj->SetLayerIdx(m_iLayerIdx);
	else
		_pObj->SetLayerIdx(_pObj->m_iLayerIdx);

	// 2. 자식 오브젝트도 전부 옮길것인지
	if (_bMoveAll)
	{
		const vector<CGameObj*>& vecChild = _pObj->GetChild();
		for (UINT i = 0; i < vecChild.size(); ++i)
		{
			AddObject(vecChild[i], _bMoveAll);
		}
	}
}

void CLayer::RemoveParent(CGameObj * _pObj)
{
	vector<CGameObj*>::iterator iter = m_vecParentObj.begin();
	for (; iter != m_vecParentObj.end(); ++iter)
	{
		if (_pObj == *iter)
		{
			m_vecParentObj.erase(iter);
			return;
		}
	}

	// 부모오브젝트, Layer 에서 제거
	// Layer 안에 없었다.
	assert(nullptr);
}

void CLayer::ClearObject()
{
	if (nullptr == this)
		return;
	m_vecObj.clear();
}
