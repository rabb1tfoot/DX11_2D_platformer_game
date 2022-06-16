#include "stdafx.h"
#include "GunItemScript.h"
#include "InfoScript.h"
#include "TraceScript.h"
#include <SceneMgr.h>
#include <Scene.h>
#include <Layer.h>
#include <EventMgr.h>

CGunItemScript::CGunItemScript()
	: CScript((UINT)SCRIPT_TYPE::GUNITEMSCRIPT)
	, m_iGunType(-1)
	, m_bDead(false)
{
}


CGunItemScript::~CGunItemScript()
{
}


void CGunItemScript::OnCollisionEnter(CCollider2D * _pOther)
{
	wstring strName = _pOther->Object()->GetName();

	if (!wcscmp(strName.c_str(), L"Player"))
	{
		CLayer** pLayer = CSceneMgr::GetInst()->GetInfoScene()->GetAllLayer();
		vector<CGameObj*> vecInfoObj = pLayer[0]->GetParentObject();
		CScript* pScript = vecInfoObj[0]->GetScript<CInfoScript>();
		((CInfoScript*)pScript)->SetPlayerWeapon(m_iGunType);
		((CInfoScript*)pScript)->SetActiveWeapon(m_iGunType);

		pLayer = CSceneMgr::GetInst()->GetCurScene()->GetAllLayer();
		vecInfoObj = pLayer[1]->GetParentObject();
		CScript* pPlayerScript = vecInfoObj[0]->GetScript<CTraceScript>();
		((CTraceScript*)pPlayerScript)->Armed();
		if (false == ((CTraceScript*)pPlayerScript)->GetGunIcon()   )
		{
			((CTraceScript*)pPlayerScript)->CreateGunIcon();
		}

		//자신삭제
		if (false == m_bDead)
		{
			tEvent tEv = {};
			tEv.eType = EVENT_TYPE::DELETE_OBJECT;
			tEv.lParam = (INT_PTR)Object();
			CEventMgr::GetInst()->AddEvent(tEv);
			m_bDead = true;
		}
	}
}

void CGunItemScript::SaveToScene(FILE * _pFile)
{
	fwrite(&m_iGunType, sizeof(int), 1, _pFile);
}

CComponent * CGunItemScript::LoadFromScene(FILE * _pFile)
{
	fread(&m_iGunType, sizeof(int), 1, _pFile);
	return nullptr;
}
