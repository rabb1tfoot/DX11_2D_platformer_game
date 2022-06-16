#include "stdafx.h"
#include "SnailScript.h"

CSnailScript::CSnailScript()
	: CScript((UINT)SCRIPT_TYPE::SNAILSCRIPT)
	, m_eRotType(RotType::TOP)
{
}


CSnailScript::~CSnailScript()
{
}


void CSnailScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	if (m_eRotType == RotType::TOP)
	{
		vPos.x += 20 * DT;
	}
	if (m_eRotType == RotType::RIGHT)
	{
		vPos.y -= 20 * DT;
	}
	if (m_eRotType == RotType::DOWN)
	{
		vPos.x -= 20 * DT;
	}
	if (m_eRotType == RotType::LEFT)
	{
		vPos.y += 20 * DT;
	}

	Transform()->SetLocalPos(Vec3(vPos.x, vPos.y, vPos.z));
}

void CSnailScript::OnCollisionExit(CCollider2D * _pOther)
{
	//타일에서 계속 닿아야한다.
	if (4 == _pOther->Object()->GetLayerIdx())
	{
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vSize = Transform()->GetLocalScale();
		Vec3 TilePos = _pOther->Object()->Transform()->GetLocalPos();
		Vec3 TileSize = _pOther->Object()->Transform()->GetLocalScale();

		if (m_eRotType == RotType::TOP)
		{
			if (vPos.x - vSize.x / 2 >= TilePos.x + TileSize.x / 2)
			{
				m_eRotType = RotType::RIGHT;
				Transform()->SetLocalPos(Vec3(vPos.x - 5.f, vPos.y, vPos.z));
				Transform()->SetLocalRot(Vec3(0.f,0.f, XM_PI / 2.f + XM_PI));
				Animator2D()->PlayAnimation(L"Snail_Move", true);
			}
		}
		if (m_eRotType == RotType::RIGHT)
		{
			if (vPos.y + vSize.y / 2 <= TilePos.y - TileSize.y / 2)
			{
				m_eRotType = RotType::DOWN;
				Transform()->SetLocalPos(Vec3(vPos.x, vPos.y + 5, vPos.z));
				Transform()->SetLocalRot(Vec3(0.f, 0.f, XM_PI));
				Animator2D()->PlayAnimation(L"Snail_Move", true);
			}
		}
		if (m_eRotType == RotType::DOWN)
		{
			if (vPos.x + vSize.x / 2 <= TilePos.x - TileSize.x / 2)
			{
				m_eRotType = RotType::LEFT;
				Transform()->SetLocalPos(Vec3(vPos.x + 5, vPos.y, vPos.z));
				Transform()->SetLocalRot(Vec3(0.f, 0.f, XM_PI + XM_PI / 2.f + XM_PI));
				Animator2D()->PlayAnimation(L"Snail_Move", true);
			}
		}
		if (m_eRotType == RotType::LEFT)
		{
			if (vPos.y - vSize.y / 2 >= TilePos.y + TileSize.y / 2)
			{
				m_eRotType = RotType::TOP;
				Transform()->SetLocalPos(Vec3(vPos.x, vPos.y - 5, vPos.z));
				Transform()->SetLocalRot(Vec3(0.f, 0.f, XM_PI * 2.f));
				Animator2D()->PlayAnimation(L"Snail_Move", true);
			}
		}
	}
}
