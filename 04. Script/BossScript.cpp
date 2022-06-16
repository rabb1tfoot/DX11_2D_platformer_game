#include "stdafx.h"
#include "BossScript.h"
#include <Transform.h>
#include <MeshRender.h>
#include <Collider2D.h>
#include <ResMgr.h>
#include <Texture.h>
#include <Scene.h>
#include <SceneMgr.h>
#include "BossBulletPara.h"
#include "BossBulletRand.h"
#include "MonsterScript.h"
#include <GameObj.h>
#include <time.h>
#include <stdlib.h>

CBossScript::CBossScript()
	:CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_fAccTransTime(0.f)
	, m_fAccBulletTime(0.f)
	, m_fModeChange(10.f)
	, m_iTakeDmg(0)
	, m_fSpeedX(-200.f)
	, m_fSpeedY(-50.f)
	, m_eType(BossState::STATE_RAND)
	, m_eDir(PLAYER_DIR::LEFT)
	, m_pSound(nullptr)
{
}


CBossScript::~CBossScript()
{
}


void CBossScript::Init()
{
	m_vFirstPos = Transform()->GetLocalPos();
}

void CBossScript::awake()
{
	m_pSound = CSceneMgr::GetInst()->GetSound();

	if (m_pSound == nullptr)
	{
		m_pSound = CResMgr::GetInst()->Load<CSound>(L"Sound\\Trace Rising.mp3", L"Sound\\Trace Rising.mp3");
		m_pSound->Play(-1);
		m_pSound->SetName(L"BossTheme");
		CSceneMgr::GetInst()->SetSound(m_pSound);
	}
	else
	{
		if (!wcscmp(m_pSound->GetName().c_str(), L"BossTheme"))
			return;
		m_pSound->Stop();
		m_pSound = CResMgr::GetInst()->Load<CSound>(L"Sound\\Trace Rising.mp3", L"Sound\\Trace Rising.mp3");
		m_pSound->Play(-1);
		m_pSound->SetName(L"BossTheme");
		CSceneMgr::GetInst()->SetSound(m_pSound);
	}
}

void CBossScript::Update()
{
	if (m_pSound == nullptr)
	{
		m_pSound = CResMgr::GetInst()->Load<CSound>(L"Sound\\Trace Rising.mp3", L"Sound\\Trace Rising.mp3");
		m_pSound->Play(-1);
	}
	//총알생성 스크립트
	m_fAccTransTime += DT;
	m_fAccBulletTime += DT;
	if (m_eType == BossState::STATE_RAND)
	{
		if (m_fAccTransTime > m_fModeChange)
		{
			m_eType = BossState::STATE_BOMB;
			m_fAccTransTime = 0.f;
		}

		if (m_fAccBulletTime > 0.2f)
		{
			CreateBullet();
			m_fAccBulletTime = 0.f;
		}
		Animator2D()->PlayAnimation(L"Boss_Rand",true);
	}

	else if (m_eType == BossState::STATE_BOMB)
	{
		if (m_fAccTransTime > m_fModeChange)
		{
			m_eType = BossState::STATE_RAND;
			m_fAccTransTime = 0.f;
		}
		if (m_fAccBulletTime > 0.7f)
		{
			CreateBullet();
			m_fAccBulletTime = 0.f;
		}
		Animator2D()->PlayAnimation(L"Boss_Des", true);
	}

	//이동 스크립트
	Vec3 myPos = Transform()->GetLocalPos();
	myPos.x += m_fSpeedX * DT;
	myPos.y += m_fSpeedY * DT * 3;

	if (myPos.y < 50.f ||  myPos.y > 100.f)
	{
		m_fSpeedY *= -1;
		if (myPos.y < 50.f)
			myPos.y = 50.f;
		else if (myPos.y > 100.f)
			myPos.y = 100.f;
	}

	if (myPos.x < -300.f || myPos.x > 100.f)
	{
		m_fSpeedX *= -1;
		if (myPos.x < -300.f)
		{
			m_eDir = PLAYER_DIR::RIGHT;
			Vec3 vRot = Vec3(0.f, XM_PI, 0.f);
			Transform()->SetLocalRot(vRot);
			myPos.x = -300.f;
		}
		if (myPos.x > 100.f)
		{
			m_eDir = PLAYER_DIR::LEFT;
			Vec3 vRot = Vec3(0.f, 0.f, 0.f);
			Transform()->SetLocalRot(vRot);
			myPos.x = 100.f;
		}
	}
	Transform()->SetLocalPos(myPos);

}

void CBossScript::OnCollisionEnter(CCollider2D* _pOther)
{
	//총알에 맞는경우 속도가 빨라진다.
	++m_iTakeDmg;
	if (_pOther->Object()->GetLayerIdx() == 2)
	{
		if (m_fSpeedX < 0)
		{
			m_fSpeedX -= 1.f;
		}
		if (m_fSpeedX > 0)
		{
			m_fSpeedX += 1.f;
		}
		if (m_fSpeedY < 0)
		{
			m_fSpeedY -= 1.f;
		}
		if (m_fSpeedY > 0)
		{
			m_fSpeedY += 1.f;
		}
		m_fModeChange -= 0.1f;
	}
}

void CBossScript::CreateBullet()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (m_eType == BossState::STATE_BOMB)
	{
		CGameObj* pBullet = new CGameObj;
		pBullet->SetName(L"BossBomb");
		CTransform* pTransform = new CTransform;
		Vec3 BossPos = Transform()->GetLocalPos();
		pTransform->SetLocalPos(Vec3(BossPos.x + 20.f, BossPos.y + 50.f, BossPos.z));
		pTransform->SetLocalScale(Vec3(19.f, 19.f, 1.f));
		CMeshRender* pMeshRender = new CMeshRender;
		CCollider2D* pCollider2D = new CCollider2D;
		pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(
			L"Texture\\BossBullet1.PNG"));
		CScript* pScript = new CBossBulletPara;
		CScript* pScript2 = new CMonsterScript;
		((CBossBulletPara*)pScript)->SetDir(m_eDir);
		((CMonsterScript*)pScript2)->SetHp(1000.f);
		CAnimator2D* pAnim = new CAnimator2D;
		CResPtr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\FireMid.png");
		pAnim->AddAnimation(L"Gun_Des", pTex, Vec2(0.f, 0.f), Vec2(50.f, 53.f), 5, 0.1f);
		pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\BossBullet1.PNG");
		pAnim->AddAnimation(L"Gun_Idle", pTex, Vec2(0.f, 0.f), Vec2(19.f, 19.f), 1, 10.f);
		pAnim->PlayAnimation(L"Gun_Idle", true);
		pBullet->AddComponent(pTransform);
		pBullet->AddComponent(pMeshRender);
		pBullet->AddComponent(pCollider2D);
		pBullet->AddComponent(pAnim);
		pBullet->AddComponent(pScript);
		pBullet->AddComponent(pScript2);

		pCurScene->AddObject(L"Monster", pBullet);
	}
	else if (m_eType == BossState::STATE_RAND)
	{
		CGameObj* pBullet = new CGameObj;
		pBullet->SetName(L"BossBullet");
		CTransform* pTransform = new CTransform;
		Vec3 BossPos = Transform()->GetLocalPos();
		pTransform->SetLocalPos(Vec3(BossPos.x + 20.f, BossPos.y + 50.f, BossPos.z));
		pTransform->SetLocalScale(Vec3(19.f, 19.f, 1.f));
		CMeshRender* pMeshRender = new CMeshRender;
		CCollider2D* pCollider2D = new CCollider2D;
		pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(
			L"Texture\\BossBullet2.PNG"));
		CScript* pScript = new CBossBulletRand;
		CScript* pScript2 = new CMonsterScript;
		((CBossBulletRand*)pScript)->SetDir(m_eDir);
		((CMonsterScript*)pScript)->SetHp(1000.f);
		pBullet->AddComponent(pTransform);
		pBullet->AddComponent(pMeshRender);
		pBullet->AddComponent(pCollider2D);
		pBullet->AddComponent(pScript);
		pBullet->AddComponent(pScript2);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer** pLayer = pCurScene->GetAllLayer();
		vector<CGameObj*> vecObj = pLayer[1]->GetParentObject();
		srand(time(NULL));
		if(rand() % 4 == 0)
			((CBossBulletRand*)pScript)->Init(vecObj[0]);
		else
			((CBossBulletRand*)pScript)->Init2();

		pCurScene->AddObject(L"Monster", pBullet);
	}
}
