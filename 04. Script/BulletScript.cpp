#include "stdafx.h"
#include "BulletScript.h"
#include <EventMgr.h>
#include <Collider2D.h>
#include <SceneMgr.h>
#include <Scene.h>
#include <TimeMgr.h>
#include <MonsterScript.h>
#include <TraceScript.h>


CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
	, m_fSpeed(500.f)
	, m_bAttach(false)
	, m_fAttachTime(0.f)
	, m_fDesTime(0.f)
	, m_bDead(false)
	, m_bDestroy(false)
	, m_iGunType(1)

{
}


CBulletScript::~CBulletScript()
{
}


void CBulletScript::Update()
{
	//터치되지않은경우
	if (!m_bAttach)
	{
		if(m_iGunType == 1)
			Animator2D()->PlayAnimation(L"Gun_Bullet1", true);
		if(m_iGunType == 2)
			Animator2D()->PlayAnimation(L"Gun_Bullet2", true);
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 tRot = Transform()->GetLocalRot();
		if (m_eType == PLAYER_DIR::LEFT)
		{
			vPos.x -= m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, XM_PI, tRot.z));
		}
		else if (m_eType == PLAYER_DIR::RIGHT)
		{
			vPos.x += m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
		}
		else if (m_eType == PLAYER_DIR::TOP)
		{
			vPos.y += m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, tRot.y, XM_PI / 2));
		}
		else if (m_eType == PLAYER_DIR::BOTTOM)
		{
			vPos.y -= m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, tRot.y, (XM_PI / 2) * -1));
		}
		else if (m_eType == PLAYER_DIR::LT)
		{
			vPos.x -= m_fSpeed * DT;
			vPos.y += m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, XM_PI, (XM_PI / 4) * -1));
		}
		else if (m_eType == PLAYER_DIR::LB)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			vPos.x -= m_fSpeed * DT;
			vPos.y -= m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, XM_PI, (XM_PI / 4)));
		}
		else if (m_eType == PLAYER_DIR::RT)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			vPos.x += m_fSpeed * DT;
			vPos.y += m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, tRot.y, XM_PI / 4));
		}
		else if (m_eType == PLAYER_DIR::RB)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			vPos.x += m_fSpeed * DT;
			vPos.y -= m_fSpeed * DT;
			Transform()->SetLocalPos(vPos);
			Transform()->SetLocalRot(Vec3(tRot.x, tRot.y, (XM_PI / 4) * -1));
		}
	}
	//애니메이션 시간 체크
	if (m_bAttach)
	{
		m_fAttachTime += DT;
	}

	//애니메이션 종료시간시 블릿 삭제
	if (m_fAttachTime > 0.6f)
	{
		if (false == m_bDead)
		{
			tEvent tEv = {};
			tEv.eType = EVENT_TYPE::DELETE_OBJECT;
			tEv.lParam = (INT_PTR)Object();
			CEventMgr::GetInst()->AddEvent(tEv);
			m_bDead = true;
			CScript* pScript = m_pPlayer->GetScript<CTraceScript>();
			((CTraceScript*)pScript)->SetBulletDead();
		}
	}
	if(m_bDestroy)
		m_fDesTime += DT;

	if (m_fDesTime > 0.1f && m_bAttach ==false)
	{
		m_bAttach = true;
		Animator2D()->PlayAnimation(L"Gun_FireMid", false);
	}

	if (m_fDesTime > 0.6f)
	{
		if (false == m_bDead)
		{
			tEvent tEv = {};
			tEv.eType = EVENT_TYPE::DELETE_OBJECT;
			tEv.lParam = (INT_PTR)Object();
			CEventMgr::GetInst()->AddEvent(tEv);
			m_bDead = true;
			CScript* pScript = m_pPlayer->GetScript<CTraceScript>();
			((CTraceScript*)pScript)->SetBulletDead();
			m_fDesTime = 0.f;
		}
	}
}

void CBulletScript::laterDestroy()
{
	m_bDestroy = true;
}

void CBulletScript::OnCollisionEnter(CCollider2D * _pOther)
{
	//타일일 경우
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//타일 또는 몬스터와 충돌할 경우
	if (_pOther->Object()->GetLayerIdx() == pCurScene->FindLayer(L"Tile")->GetLayerIdx()
		|| _pOther->Object()->GetLayerIdx() == pCurScene->FindLayer(L"Monster")->GetLayerIdx())
	{
		if(!wcscmp(_pOther->Object()->GetName().c_str(), L"BossBullet") || 
			!wcscmp(_pOther->Object()->GetName().c_str(), L"BossBomb") ||
			!wcscmp(_pOther->Object()->GetName().c_str(), L"BugBullet"))
			return;
		Animator2D()->PlayAnimation(L"Gun_FireMid", false);
		m_bAttach = true;
	}
	//몬스터 스크립트에서 체력관리
	if (_pOther->Object()->GetLayerIdx() == pCurScene->FindLayer(L"Monster")->GetLayerIdx())
	{
		CScript* MonScript = _pOther->Object()->GetScript<CMonsterScript>();
		((CMonsterScript*)(MonScript))->HittedFunc();
	}
}

