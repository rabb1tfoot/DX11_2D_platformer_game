#include "stdafx.h"
#include "HiveScript.h"
#include <SceneMgr.h>
#include <Animation2D.h>
#include "MonsterScript.h"
#include "BugScript.h"
CHiveScript::CHiveScript()
	: CScript((UINT)SCRIPT_TYPE::HIVESCRIPT)
	, m_pBug{}
	, m_fAccSpawnTime{}
	, m_fAnimTime(0.f)
{
}

void CHiveScript::awake()
{
	Init();
}

void CHiveScript::Update()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pBug[i] == nullptr)
		{
			m_fAccSpawnTime[i] += DT;
		}

		if (m_fAccSpawnTime[i] > 5.f)
		{
			//시간이 지나고 죽은 벌레생성
		}
	}
	if (m_fAnimTime > 0.6f)
	{
		m_fAnimTime = 0.f;
		Animator2D()->PlayAnimation(L"Hive_Idle", true);
	}
	++m_fAnimTime;
}

void CHiveScript::Init()
{
	//벌레3마리 만들기
	for (int i = 0; i < 3; ++i)
	{
		CreateBug(i + 1);
	}
}

void CHiveScript::CreateBug(float _Pos)
{
	 Vec3 vMyPos= Transform()->GetLocalPos();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObj* pBug = new CGameObj;
	pBug->SetName(L"Bug");
	
	CTransform* pTransform = new CTransform;
	pTransform->SetLocalPos(Vec3(vMyPos.x - 100.f, vMyPos.y, vMyPos.z));
	pTransform->SetLocalScale(Vec3(32.f, 32.f, 1.f));
	CMeshRender* pMeshRender = new CMeshRender;
	CCollider2D* pCollider2D = new CCollider2D;
	CAnimator2D* pAnim = new CAnimator2D;
	CScript* pMonsterScript = new CMonsterScript;
	CScript* pBugScript = new CBugScript;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\00.png"));
	CResPtr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"Texture\\Animation\\bug(clear).png", L"Texture\\Animation\\bug(clear).png");
	pAnim->AddAnimation(L"Bug_Move", pTex, Vec2(0, 0), Vec2(20, 20), 4, 0.1f);
	pAnim->PlayAnimation(L"Bug_Move", true);
	((CMonsterScript*)pMonsterScript)->SetHp(5);
	((CBugScript*)pBugScript)->SetSpeed(_Pos);
	pCollider2D->SetScale(Vec3(1.f, 1.f, 1.f));
	pBug->AddComponent(pTransform);
	pBug->AddComponent(pMeshRender);
	pBug->AddComponent(pCollider2D);
	pBug->AddComponent(pAnim);
	pBug->AddComponent(pBugScript);
	pBug->AddComponent(pMonsterScript);
	CLayer** pLayer = pCurScene->GetAllLayer();
	vector<CGameObj*> pObj = pLayer[1]->GetParentObject();
	((CBugScript*)pBugScript)->SetPlayer(pObj[0]);
	
	pCurScene->AddObject(L"Monster", pBug);
	//CGameObj* pFollowedCollider = new CGameObj;
	//pTransform = new CTransform;
	//pTransform->SetLocalPos(Vec3(vMyPos.x - 100.f, vMyPos.y, vMyPos.z));
	//pTransform->SetLocalScale(Vec3(32.f, 32.f, 1.f));
	//pFollowedCollider->AddComponent(new)
}

CHiveScript::~CHiveScript()
{
}
