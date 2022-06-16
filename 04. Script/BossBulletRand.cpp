#include "stdafx.h"
#include "BossBulletRand.h"
#include <time.h>
#include <stdlib.h>
#include <EventMgr.h>
CBossBulletRand::CBossBulletRand()
	: CScript((UINT)SCRIPT_TYPE::BOSSBULLETRAND)
	, m_fSpeed(0.f)
	, m_bDead(false)
	, m_eDir(PLAYER_DIR::END)
{
}

CBossBulletRand::~CBossBulletRand()
{
}


void CBossBulletRand::Update()
{
	Vec3 vMyPos = Transform()->GetLocalPos();
	vMyPos.x -= m_Angle.x *  DT * 200.f;
	vMyPos.y -= m_Angle.y *  DT * 200.f;

	Transform()->SetLocalPos(vMyPos);
}

void CBossBulletRand::OnCollisionEnter(CCollider2D * _pOther)
{
	//타일레이어 일 경우
	if (_pOther->Object()->GetLayerIdx() == 4)
	{
		if (!m_bDead)
		{
			tEvent tEV;
			tEV.eType = EVENT_TYPE::DELETE_OBJECT;
			tEV.lParam = (INT_PTR)Object();
			CEventMgr::GetInst()->AddEvent(tEV);

			m_bDead = true;
		}
	}
}

void CBossBulletRand::OnCollision(CCollider2D * _pOther)
{
	//타일레이어 일 경우
	if (_pOther->Object()->GetLayerIdx() == 4)
	{
		if (!m_bDead)
		{
			tEvent tEV;
			tEV.eType = EVENT_TYPE::DELETE_OBJECT;
			tEV.lParam = (INT_PTR)Object();
			CEventMgr::GetInst()->AddEvent(tEV);

			m_bDead = true;
		}
	}
}


void CBossBulletRand::Init(CGameObj* _player)
{
	m_pPlayer = _player;
	Vec3 vPlayerPos = _player->Transform()->GetLocalPos();
	Vec3 vMyPos = Object()->Transform()->GetLocalPos();

	m_Angle.x = vMyPos.x - vPlayerPos.x;
	m_Angle.y = vMyPos.y - vPlayerPos.y;

	float fDist = sqrt(pow(m_Angle.x, 2) + pow(m_Angle.y, 2));
	m_Angle.x /= fDist;
	m_Angle.y /= fDist;
}

void CBossBulletRand::Init2()
{
	srand(time(NULL));

	m_Angle.x = rand() % 90;
	m_Angle.y = rand() % 90;

	float fDist = sqrt(pow(m_Angle.x, 2) + pow(m_Angle.y, 2));
	m_Angle.x /= fDist;
	m_Angle.y /= fDist;
}
