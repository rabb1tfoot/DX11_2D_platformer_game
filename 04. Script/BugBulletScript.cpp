#include "stdafx.h"
#include "BugBulletScript.h"
#include <EventMgr.h>


CBugBulletScript::CBugBulletScript()
	:CScript((UINT)SCRIPT_TYPE::BUGBULLETSCRIPT)
	, m_pPlayer(nullptr)
	, m_Angle{}
	, m_bDead(false)
	, m_bActive(false)
{
}



void CBugBulletScript::Init(CGameObj * _player)
{
	m_pPlayer = _player;
	Vec3 vPlayerPos = _player->Transform()->GetLocalPos();
	Vec3 vMyPos = Object()->Transform()->GetLocalPos();

	m_Angle.x = vMyPos.x - vPlayerPos.x;
	m_Angle.y = vMyPos.y - vPlayerPos.y;

	float fDist = sqrt(pow(m_Angle.x, 2) + pow(m_Angle.y, 2));
	m_Angle.x / fDist;
	m_Angle.y / fDist;
}

void CBugBulletScript::Update()
{
	Vec3 vMyPos = Transform()->GetLocalPos();
	vMyPos.x -= m_Angle.x *  DT;
	vMyPos.y -= m_Angle.y *  DT;
	
	Transform()->SetLocalPos(vMyPos);
}

void CBugBulletScript::OnCollisionEnter(CCollider2D * _pOther)
{
	if (_pOther->Object() == m_pPlayer && m_bActive || _pOther->Object()->GetLayerIdx() == 4)
	{
		if (!m_bDead)
		{
			tEvent tEV;
			tEV.eType = EVENT_TYPE::DELETE_OBJECT;
			tEV.lParam = (INT_PTR)Object();
			CEventMgr::GetInst()->AddEvent(tEV);
		}
		m_bDead = true;
	}
}

CBugBulletScript::~CBugBulletScript()
{
}
