#include "stdafx.h"
#include "MonsterScript.h"
#include "Device.h"
#include "SceneMgr.h"
#include "TraceScript.h"
#include <EventMgr.h>
#include <Collider2D.h>
#include <GameObj.h>
#include "BugBulletScript.h"
#include "InfoScript.h"
CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_iEffOn(0)
	, m_fAccTime(0.f)
	, m_bHitEff(false)
	, m_bDestroyEff(false)
	, m_iMaxhp(-1)
	, m_iCurhp(-1)
	, m_bDead(false)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Update()
{
	if (m_bHitEff)
	{
		m_fAccTime += DT;
		m_iEffOn = 2;
		Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &m_iEffOn);
		Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
		Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_1, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Hit.PNG"));
		if (m_fAccTime > 0.5f)
		{
			m_bHitEff = false;
			m_fAccTime = 0.f;
			m_iEffOn = 0;
			Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &m_iEffOn);
			Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
		}
	}
	if (GetDestoryEff())
	{
		m_fAccTime += DT;
		m_iEffOn = 3;
		Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &m_iEffOn);
		Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
	
		if (m_fAccTime > 50.f)
		{
			SetDestoryEff(false);
			m_fAccTime = 0.f;
			m_iEffOn = 0;
			Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &m_iEffOn);
			Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
		}
	}
	if (m_iCurhp == 0 && !m_bDead)
	{
		m_bDead = true;
		tEvent tEV;
		tEV.eType = EVENT_TYPE::DELETE_OBJECT;
		tEV.lParam = (INT_PTR)Object();
		CEventMgr::GetInst()->AddEvent(tEV);

		//벌레가 죽는경우 알려줘야함
		if (!wcscmp(Object()->GetName().c_str(), L"Bug"))
		{
			//Object()->
		}
	}

}

void CMonsterScript::OnCollisionEnter(CCollider2D * _pOther)
{
	if (!wcscmp(Object()->GetName().c_str(), L"Bug"))
		return;

	//플레이어 충돌하는경우 
	if (!wcscmp(_pOther->Object()->GetName().c_str(), L"Player"))
	{
		CScene* pInfo = CSceneMgr::GetInst()->GetInfoScene();
		CLayer** pLayer = pInfo->GetAllLayer();
		vector<CGameObj*> vecObj = pLayer[0]->GetParentObject();
		CScript* pScript = vecObj[0]->GetScript<CInfoScript>();
		if (((CInfoScript*)pScript)->IsImmune())
			return;
		CGameObj* pBullet = Object();
		CScript* pPlayerScript = _pOther->Object()->GetScript<CTraceScript>();
		//if (!wcscmp(Object()->GetName().c_str(), L"BugBullet"))
		//{
		//	CScript* pScript = pBullet->GetScript<CBugBulletScript>();
		//	if (((CBugBulletScript*)pScript)->GetActive())
		//	{
		//		((CTraceScript*)pPlayerScript)->Hitted();
		//	}
		//	((CBugBulletScript*)pScript)->SetActive();
		//}
		//else
		//{
			((CTraceScript*)pPlayerScript)->Hitted();
		//}
	}
}

void CMonsterScript::SaveToScene(FILE * _pFile)
{
	fwrite(&m_iCurhp, sizeof(int), 1, _pFile);
	fwrite(&m_iMaxhp, sizeof(int), 1, _pFile);
}

CComponent * CMonsterScript::LoadFromScene(FILE * _pFile)
{
	fread(&m_iCurhp, sizeof(int), 1, _pFile);
	fread(&m_iMaxhp, sizeof(int), 1, _pFile);
	return nullptr;
}


void CMonsterScript::HittedFunc()
{
	if (!wcscmp(Object()->GetName().c_str(), L"BugBullet"))
		return;
	--m_iCurhp;
	m_bHitEff = true;
}

