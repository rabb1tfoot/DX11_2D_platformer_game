#include "stdafx.h"
#include "EventMgr.h"

#include "GameObj.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Script.h"


CEventMgr::CEventMgr()
	: m_vecDeleteObj{}
	, m_vecEvent{}
	, m_vecNewObj{}
{
}


CEventMgr::~CEventMgr()
{
	Safe_Delete_Vec(m_vecDeleteObj);
	Safe_Delete_Vec(m_vecNewObj);
}

void CEventMgr::Update()
{
	//이벤트 처리 진행

	for (UINT i = 0; i < m_vecDeleteObj.size(); ++i)
	{
		m_vecDeleteObj[i]-> Disconnect();
		SAFE_DELETE(m_vecDeleteObj[i]);
	}

	for (UINT i = 0; i < m_vecNewObj.size(); ++i)
	{
		m_vecNewObj[i];
	}

	m_vecDeleteObj.clear();
	m_vecNewObj.clear();

	//이벤트 등록, 부모자식관계 처리

	for (UINT i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_vecEvent[i].wParam);
			pLayer->AddObject((CGameObj*)m_vecEvent[i].lParam, true);
			break;
		}
		case EVENT_TYPE::DELETE_OBJECT:
			((CGameObj*)m_vecEvent[i].lParam)->SetDead();
			m_vecDeleteObj.push_back(((CGameObj*)m_vecEvent[i].lParam));
			break;

		case EVENT_TYPE::ADD_CHILD:
			((CGameObj*)m_vecEvent[i].lParam)->AddChild((CGameObj*)m_vecEvent[i].wParam);
			break;

		case EVENT_TYPE::CLEAR_PARANT:
			((CGameObj*)m_vecEvent[i].lParam)->ClearParent();
			break;

		case EVENT_TYPE::CLEAR_CHILD:
			((CGameObj*)m_vecEvent[i].lParam)->ClearChild((UINT)m_vecEvent[i].wParam);
			break;

		case EVENT_TYPE::ON_ENABLE:
			((CGameObj*)m_vecEvent[i].lParam)->OnEnable();
			break;

		case EVENT_TYPE::ON_DISABLE:
			((CGameObj*)m_vecEvent[i].lParam)->OnDisable();
			break;

		case EVENT_TYPE::CHANGE_LAYER:
		{
			//wParam의 HIGHWORD : layerIndx
			//wParam의 LOWWORD : bool (자식의 레이어도 동일하게 옮길것인지 아닌지)
			WORD iLayerIdx = HIWORD(m_vecEvent[i].wParam);
			WORD bMoveAll = LOWORD(m_vecEvent[i].wParam);

			//기존 레이어에서 제거
			int iPrevLayerIdx = ((CGameObj*)m_vecEvent[i].lParam)->GetLayerIdx();
			CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(iPrevLayerIdx);
			if (nullptr == ((CGameObj*)m_vecEvent[i].lParam)->GetParent())
				pLayer->RemoveParent((CGameObj*)m_vecEvent[i].lParam);

			pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(iLayerIdx);
			pLayer->AddObject(((CGameObj*)m_vecEvent[i].lParam), (bool)bMoveAll);
			break;
		}

		case EVENT_TYPE::DEL_SCRIPT:
		{
			//wParam : SCRIPT_TYPE
			//lParam : GameObject

			CGameObj* MypGameObj = ((CGameObj*)m_vecEvent[i].lParam);
			vector<CScript*>& MyScripts = MypGameObj->GetScripts();

			vector<CScript*>::iterator iter = MyScripts.begin();
			for (; iter != MyScripts.end(); ++iter)
			{
				if ((*iter)->GetScriptType() == (UINT)m_vecEvent[i].wParam)
				{
					delete *iter;
					MyScripts.erase(iter);
					break;
				}
			}
		}

		}
	}
	m_vecEvent.clear();
}
