#include "stdafx.h"
#include "BugScript.h"
#include "MonsterScript.h"
#include "BugBulletScript.h"
#include <Animator2D.h>

CBugScript::CBugScript()
	:CScript((UINT)SCRIPT_TYPE::BUGSCRIPT)
	, m_pHive(nullptr)
	, m_fColliderTime(0.f)
	, m_fAttackTime(0.f)
	, m_fAngle(0.f)
	, m_fSpeed(1.f)
	, m_bAttacking(false)
{
}


void CBugScript::Update()
{
	Vec3 VecPos = Transform()->GetLocalPos();

	if (!m_bAttacking)
	{
		m_fAngle += XM_PI * DT * m_fSpeed;
		VecPos.x += sinf(m_fAngle) * 4;
		VecPos.y += cosf(m_fAngle) * 4;
		Transform()->SetLocalPos(Vec3(VecPos.x, VecPos.y, VecPos.z));

		if (m_fAnimTime > 0.4f);
		{
			m_fAnimTime = 0.f;
			Animator2D()->PlayAnimation(L"Bug_Move", true);
		}
	}
	else
	{
		m_fAttackTime += DT;
		if (m_fAttackTime > 0.3f)
		{
			m_bAttacking = false;
			m_fAttackTime = 0.f;
		}
	}
	if ((m_pPlayer->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x > -300.f
		&& m_pPlayer->Transform()->GetLocalPos().x < Transform()->GetLocalPos().x)
		|| -m_pPlayer->Transform()->GetLocalPos().x + Transform()->GetLocalPos().x > -300.f
		&& m_pPlayer->Transform()->GetLocalPos().x > Transform()->GetLocalPos().x)
	{
		m_fColliderTime += DT;
	}

	if (m_fColliderTime > 1.f)
	{
		m_bAttacking = true;
		AttackBullet(m_pPlayer);
		m_fColliderTime = 0.f;
	}
}

void CBugScript::OnCollision(CCollider2D * _pOther)
{
	//if (!wcscmp(_pOther->Object()->GetName().c_str(), L"Player"))
	//{
	//	m_fColliderTime += DT;
	//}
	//
	////Á¢ÃËÀÌ 1ÃÊÀÌ»óµÇ¸é ÅºÈ¯À» ½ð´Ù.
	//if (m_fColliderTime > 1.f)
	//{
	//	m_bAttacking = true;
	//	AttackBullet(_pOther);
	//	m_fColliderTime = 0.f;
	//}
}


void CBugScript::AttackBullet(CGameObj * _pOther)
{
	CGameObj* pBugBullet = new CGameObj;
	pBugBullet->SetName(L"BugBullet");
	Vec3 BugPos= Object()->Transform()->GetLocalPos();
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CTransform* pTransform = new CTransform;
	pTransform->SetLocalPos(BugPos);
	pTransform->SetLocalScale(Vec3(16.f, 16.f, 1.f));
	CMeshRender* pMeshRender = new CMeshRender;
	CCollider2D* pCollider2D = new CCollider2D;
	CScript* pMonsterScript = new CMonsterScript;
	CScript* pBugBulletScript = new CBugBulletScript;
	((CMonsterScript*)pMonsterScript)->SetHp(1000.f);
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->Load<CTexture>(L"Texture\\BugBullet(clear).png", L"Texture\\BugBullet(clear).png"));
	pBugBullet->AddComponent(pTransform);
	pBugBullet->AddComponent(pMeshRender);
	pBugBullet->AddComponent(pCollider2D);
	pBugBullet->AddComponent(pBugBulletScript);
	pBugBullet->AddComponent(pMonsterScript);
	pCurScene->AddObject(L"Monster", pBugBullet);
	((CBugBulletScript*)pBugBulletScript)->Init(_pOther);
}


CBugScript::~CBugScript()
{
}