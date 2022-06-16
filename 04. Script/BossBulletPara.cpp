#include "stdafx.h"
#include "BossBulletPara.h"
#include <EventMgr.h>
#include <Transform.h>

CBossBulletPara::CBossBulletPara()
	: CScript((UINT)SCRIPT_TYPE::BOSSBULLETPARA)
	, m_bDead(false)
	, m_bIsAir(true)
	, m_bDown(false)
	, m_fForce(0.f)
	, m_fAccTime(0.f)
	, m_eDIR(PLAYER_DIR::END)
{
}


CBossBulletPara::~CBossBulletPara()
{
}


void CBossBulletPara::Update()
{
	Vec3 vecPos = Transform()->GetLocalPos();

	if (m_bIsAir && !m_bDown)
	{
		if (m_fForce > -300.f)
			m_fForce += -1 * 400.f * DT * 2;

		if (m_eDIR == PLAYER_DIR::LEFT)
		{
			vecPos.x -= 300.f * DT;
		}
		if (m_eDIR == PLAYER_DIR::RIGHT)
		{
			vecPos.x += 300.f * DT;
		}
	}
	else if(!m_bIsAir && m_bDown)
	{
		m_fForce = 0.f;
	}
	vecPos.y += m_fForce * DT;


	Transform()->SetLocalPos(vecPos);

}

void CBossBulletPara::OnCollisionEnter(CCollider2D * _pOther)
{
	//타일레이어 일 경우
	if (_pOther->Object()->GetLayerIdx() == 4)
	{
		if (Transform()->GetLocalPos().y > -70.f)
		{
			m_bDown = true;
			return;
		}
		//m_fForce = 0.f;
		//Vec3 TilePos = _pOther->Object()->Transform()->GetLocalPos();
		//Vec3 TileSize = _pOther->Object()->Transform()->GetLocalScale();
		//
		//Transform()->SetLocalPos(Vec3(TilePos.x + TileSize.x / 2, TilePos.y + TileSize.y / 2, 0.f));
		m_bDown = true;
		m_bIsAir = false;
	}
}

void CBossBulletPara::OnCollision(CCollider2D * _pOther)
{
	//타일레이어 일 경우
	if (_pOther->Object()->GetLayerIdx() == 4)
	{
		//m_fForce = 0.f;
		//Vec3 TilePos = _pOther->Object()->Transform()->GetLocalPos();
		//Vec3 TileSize = _pOther->Object()->Transform()->GetLocalScale();
		//
		//Transform()->SetLocalPos(Vec3(TilePos.x + TileSize.x / 2, TilePos.y + TileSize.y / 2, 0.f));
		if (Transform()->GetLocalPos().y > -70.f)
		{
			m_bDown = true;
			return;
		}
		m_bDown = true;
		m_bIsAir = false;
		if (!m_bDead)
		{
			m_fAccTime += DT;
			if (m_fAccTime > 2.5f)
			{
				//터지는 애니메이션 실행
				Animator2D()->PlayAnimation(L"Gun_Des", false);
				Transform()->SetLocalScale(Vec3(80.f, 50.f, 1.f));
			}
			if (m_fAccTime > 3.f)
			{
				tEvent tEV;
				tEV.eType = EVENT_TYPE::DELETE_OBJECT;
				tEV.lParam = (INT_PTR)Object();
				CEventMgr::GetInst()->AddEvent(tEV);
				m_bDead = true;
			}
		}
	}
}

