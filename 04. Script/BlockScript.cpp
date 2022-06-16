#include "stdafx.h"
#include "BlockScript.h"
#include <EventMgr.h>
#include <func.h>


CBlockScript::CBlockScript()
	: CScript((UINT)SCRIPT_TYPE::BLOCKSCRIPT)
	, m_pBlockingObj(nullptr)
	, m_bDead(false)
{
}


CBlockScript::~CBlockScript()
{
}


void CBlockScript::awake()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer** pLayer = pCurScene->GetAllLayer();
	vector<CGameObj*> vecObj = pLayer[4]->GetParentObject();
	for (int i = 0; i < vecObj.size(); ++i)
	{
		if (!wcscmp(m_wBlockName.c_str(), vecObj[i]->GetName().c_str()))
		{
			m_pBlockingObj = vecObj[i];
			break;
		}
	}
}

void CBlockScript::OnCollision(CCollider2D * _pOther)
{
	//ÃÑ¾ËÀÌ¶û ºÎŠ°Ä¡´Â°æ¿ì
	if (_pOther->Object()->GetLayerIdx() == 2)
	{
		if (m_pBlockingObj == nullptr)
			return;

		tEvent tEV;
		tEV.eType = EVENT_TYPE::DELETE_OBJECT;
		tEV.lParam = (INT_PTR)m_pBlockingObj;
		CEventMgr::GetInst()->AddEvent(tEV);

		m_pBlockingObj = nullptr;
	}
}

void CBlockScript::SaveToScene(FILE * _pFile)
{
	SaveWString(m_pBlockingObj->GetName().c_str(), _pFile);
}

CComponent * CBlockScript::LoadFromScene(FILE * _pFile)
{
	m_wBlockName = LoadWString(_pFile);
	return nullptr;
}
