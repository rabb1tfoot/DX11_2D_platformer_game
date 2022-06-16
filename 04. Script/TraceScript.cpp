#include "stdafx.h"
#include "TraceScript.h"
#include "Device.h"
#include "ResMgr.h"
#include "Collider2D.h"
#include "SceneMgr.h"
#include "Scene.h"
#include <Transform.h>
#include "Animator2D.h"
#include "HilightScript.h"
#include "BulletScript.h"
#include <func.h>
#include <SceneMgr.h>
#include <Layer.h>
#include <EventMgr.h>
#include "InfoScript.h"


CTraceScript::CTraceScript()
	: CScript((UINT)SCRIPT_TYPE::TRACESCRIPT)
	, m_eType(PLAYER_DIR::END)
	, m_eTypeEyesight(PLAYER_DIR::END)
	, m_eOldKey(PLAYER_KEY::END)
	, m_fAccHitTime(0.f)
	, m_fSpeed(532.f)
	, m_iTouchCount(0)
	, m_fHp(5.f)
	, m_bIsAir(true)
	, m_bHitted(false)
	, m_bPause(false)
	, m_bArmed(false)
	, m_bUpAnimed(false)
	, m_bDownJump(false)
	, m_bDead(false)
	, m_vForce{}
	, m_iMaxhp(-1)
	, m_iCurhp(-1)
	, m_pScript(nullptr)
	, m_pHpObject(nullptr)
	, m_pGunUI(nullptr)
	, m_pBullet(nullptr)
	, m_AbleGunIcon(nullptr)
	, m_pSound(nullptr)
	, m_iGunType(-1)
	, m_iTryShootTime(0)
{
}


CTraceScript::~CTraceScript()
{
}
void CTraceScript::awake()
{
	Animator2D()->PlayAnimation(L"UnArmPlayer_Awake", false);
	CScene* pInfoScene = CSceneMgr::GetInst()->GetInfoScene();
	CLayer** pLayer = pInfoScene->GetAllLayer();
	vector<CGameObj*> vecObj = pLayer[0]->GetParentObject();
	if (vecObj.size() != 0)
	{
		m_pScript = vecObj[0]->GetScript<CInfoScript>();
		m_fHp = ((CInfoScript*)m_pScript)->GetPlayerHp();
		m_iGunType = ((CInfoScript*)m_pScript)->GetPlayerWeapon();
	}

	CreateHpBar();
	CreateGunUI();

	m_pSound = CSceneMgr::GetInst()->GetSound();

	if (m_pSound == nullptr)
	{
		m_pSound = CResMgr::GetInst()->Load<CSound>(L"Sound\\Otherworld.mp3", L"Sound\\Otherworld.mp3");
		m_pSound->Play(-1);
		m_pSound->SetName(L"MainTheme");
		CSceneMgr::GetInst()->SetSound(m_pSound);
	}
	else
	{
		if (!wcscmp(m_pSound->GetName().c_str(), L"MainTheme") || !wcscmp(m_pSound->GetName().c_str(), L"BossTheme"))
			return;
		m_pSound->Stop();
		m_pSound = CResMgr::GetInst()->Load<CSound>(L"Sound\\Otherworld.mp3", L"Sound\\Otherworld.mp3");
		m_pSound->Play(-1);
		m_pSound->SetName(L"MainTheme");
		CSceneMgr::GetInst()->SetSound(m_pSound);
	}
}
void CTraceScript::Update()
{

	CGameObj* pObj = Object();
	Vec3 vPos = pObj->Transform()->GetLocalPos();
	Vec3 vRot = pObj->Transform()->GetLocalRot();
	if (m_fHp < 0)
	{
		GameOver();
	}

	//퍼즈시
	if (true == m_bPause)
	{
		return;
	}

	//타일위(on the Tile)가 아니라면 중력이 적용한다.
	if (m_bIsAir)
	{
		if (m_vForce.y > -300.f)
			m_vForce += Vec3(0.f, -1 * 400.f * DT * 2, 0.f);
	}
	else
	{
		m_vForce = Vec3(0.f, 0.f, 0.f);
	}

	vPos.y += m_vForce.y * DT;

	//맞을 경우
	if (true == m_bHitted)
	{
		m_fAccHitTime += DT;
		CScript* pScript = Object()->GetScript<CHilightScript>();
		((CHilightScript*)pScript)->EffectHittedOn();
		if (!m_bIsAir)
		{
			m_bIsAir = true;
			m_vForce = Vec3(0.f, m_fSpeed / 2.f, 0.f);
		}

		if (m_eTypeEyesight == PLAYER_DIR::LEFT)
		{
			vPos.x += 150.f * DT;
			pObj->Transform()->SetLocalPos(vPos);
		}
		else if (m_eTypeEyesight == PLAYER_DIR::RIGHT)
		{
			vPos.x -= 150.f * DT;
			pObj->Transform()->SetLocalPos(vPos);
		}
		if (m_fAccHitTime > 0.5f)
		{
			m_fAccHitTime = 0;
			m_bHitted = false;
		}
		pObj->Transform()->SetLocalPos(vPos);
		pObj->Transform()->SetLocalRot(vRot);
	}
	//통상 시
	else if (false == m_bHitted)
	{

		if (KEYHOLD(KEY_TYPE::KEY_LEFT))
		{
			if (m_eOldKey == PLAYER_KEY::BOTTOM)
				return;
			vPos.x -= 300.f * DT;
			m_eType = PLAYER_DIR::LEFT;
			m_eTypeEyesight = PLAYER_DIR::LEFT;
			if(m_eOldKey == PLAYER_KEY::END)
				m_eOldKey = PLAYER_KEY::LEFT;
		}
		if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
		{
			if (m_eOldKey == PLAYER_KEY::BOTTOM)
				return;
			vPos.x += 300.f * DT;
			m_eType = PLAYER_DIR::RIGHT;
			m_eTypeEyesight = PLAYER_DIR::RIGHT;
			if (m_eOldKey == PLAYER_KEY::END)
				m_eOldKey = PLAYER_KEY::RIGHT;
		}
		if (KEYHOLD(KEY_TYPE::KEY_UP))
		{
			if (m_eOldKey == PLAYER_KEY::END)
				m_eOldKey = PLAYER_KEY::TOP;
		}
		if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			if (m_eOldKey == PLAYER_KEY::END)
				m_eOldKey = PLAYER_KEY::BOTTOM;
		}

		if (KEYAWAY(KEY_TYPE::KEY_LEFT) || KEYAWAY(KEY_TYPE::KEY_RIGHT) ||
			KEYAWAY(KEY_TYPE::KEY_UP) || KEYAWAY(KEY_TYPE::KEY_DOWN))
		{
			m_eOldKey = PLAYER_KEY::END;
		}

		if (KEYENTER(KEY_TYPE::KEY_SPACE))
		{
			if (!m_bIsAir)
			{
				m_bIsAir = true;
				m_vForce = Vec3(0.f, m_fSpeed, 0.f);
			}
		}
		if (KEYENTER(KEY_TYPE::KEY_1))
		{
			//총1번
			CScene* pInfoScene = CSceneMgr::GetInst()->GetInfoScene();
			CLayer** pLayer = pInfoScene->GetAllLayer();
			vector<CGameObj*> vecObj = pLayer[0]->GetParentObject();
			CScript* pScript = vecObj[0]->GetScript<CInfoScript>();
			if (((CInfoScript*)pScript)->GetActiveWeapon() < 1)
				return;
			((CInfoScript*)m_pScript)->SetPlayerWeapon(1);
			m_iGunType = 1;
			m_pGunUI->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\IconGun1.png"));
		}
		if (KEYENTER(KEY_TYPE::KEY_2))
		{
			//총2번
			CScene* pInfoScene = CSceneMgr::GetInst()->GetInfoScene();
			CLayer** pLayer = pInfoScene->GetAllLayer();
			vector<CGameObj*> vecObj = pLayer[0]->GetParentObject();
			CScript* pScript = vecObj[0]->GetScript<CInfoScript>();
			if (((CInfoScript*)pScript)->GetActiveWeapon() != 2)
				return;
			((CInfoScript*)m_pScript)->SetPlayerWeapon(2);
			m_iGunType = 2;
			m_pGunUI->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\IconGun2.png"));
		}
		if (KEYENTER(KEY_TYPE::KEY_3))
		{
			((CInfoScript*)m_pScript)->SetImmune();
		}

		if (KEYENTER(KEY_TYPE::KEY_F))
		{
			////깜빡임 효과
			//CHilightScript* pScript = (CHilightScript*)Object()->GetScript<CHilightScript>();
			//pScript->EffectHittedOn();
			//
			////총알발사
			////CreateBullet();
			//
			////맞을 경우
			//Hitted();
			//
			//m_bArmed = true;
			CreateBullet();
		}
		if (KEYENTER(KEY_TYPE::KEY_G))
		{
			//CreateBullet();
		}

		if (m_eType == PLAYER_DIR::LEFT)
		{
			vRot = Vec3(0.f, XM_PI, 0.f);
			Collider2D()->SetOffset(Vec3(8.f, -7.f, 0));
		}
		if (m_eType == PLAYER_DIR::RIGHT)
		{
			vRot = Vec3(0.f, 0.f, 0.f);
			Collider2D()->SetOffset(Vec3(-8.f, -7.f, 0));
		}

		pObj->Transform()->SetLocalPos(vPos);
		pObj->Transform()->SetLocalRot(vRot);

		CheckAnim();
	}

}

void CTraceScript::SaveToScene(FILE * _pFile)
{
	fwrite(&m_bArmed, sizeof(bool), 1, _pFile);
}

CComponent * CTraceScript::LoadFromScene(FILE * _pFile)
{
	fread(&m_bArmed, sizeof(bool), 1, _pFile);
	return nullptr;
}

void CTraceScript::Hitted()
{
	((CInfoScript*)m_pScript)->SetPlayerHpdown();
	m_fHp = ((CInfoScript*)m_pScript)->GetPlayerHp();
	m_pHpObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fHp);
	m_bHitted = true;
}

void CTraceScript::CheckAnim()
{
	if (m_bArmed == false && m_iGunType == -1)
	{
		if (m_eOldKey == PLAYER_KEY::BOTTOM)
		{
			Animator2D()->PlayAnimation(L"UnArmPlayer_Crouch", false);
		}
		if (KEYENTER(KEY_TYPE::KEY_SPACE) && (m_eOldKey == PLAYER_KEY::RIGHT || m_eOldKey == PLAYER_KEY::LEFT))
		{
			Animator2D()->PlayAnimation(L"UnArmPlayer_Jump",false);
		}
		if ((m_eOldKey == PLAYER_KEY::RIGHT || m_eOldKey == PLAYER_KEY::LEFT) && !m_bIsAir)
		{
			Animator2D()->PlayAnimation(L"UnArmPlayer_Walk", true);
		}

		if ((!m_bIsAir) &&(KEYAWAY(KEY_TYPE::KEY_UP) || KEYAWAY(KEY_TYPE::KEY_DOWN) || KEYAWAY(KEY_TYPE::KEY_LEFT) || KEYAWAY(KEY_TYPE::KEY_RIGHT)))
		{
			Animator2D()->PlayAnimation(L"UnArmPlayer_Idle",true);
		}
	}
	else if (m_bArmed == true && m_iGunType != -1)
	{
		if (!m_bIsAir)
		{
			if (m_eOldKey == PLAYER_KEY::TOP && (KEYENTER(KEY_TYPE::KEY_LEFT) || KEYENTER(KEY_TYPE::KEY_RIGHT)))
			{
				Animator2D()->PlayAnimation(L"Player_Upaim_walk", true);
			}

			if (KEYENTER(KEY_TYPE::KEY_UP))
			{
				if ((m_eOldKey == PLAYER_KEY::RIGHT || m_eOldKey == PLAYER_KEY::LEFT))
				{
					Animator2D()->PlayAnimation(L"Player_Upaim_walk", true);
					m_bUpAnimed = true;
					return;
				}
				else
				{
					Animator2D()->PlayAnimation(L"Player_Up_idle", false);
				}
			}
			if (m_eOldKey == PLAYER_KEY::BOTTOM)
			{
				Animator2D()->PlayAnimation(L"Player_Crouch", false);
			}
			if (KEYENTER(KEY_TYPE::KEY_SPACE))
			{
				Animator2D()->PlayAnimation(L"Player_Jump", false);
			}
			if ((m_eOldKey == PLAYER_KEY::RIGHT || m_eOldKey == PLAYER_KEY::LEFT))
			{
				//대각선에임일때는 넘어간다.
				if(!m_bUpAnimed)
					Animator2D()->PlayAnimation(L"Player_Walk", true);
			}
			if (m_bUpAnimed)
			{
				if (KEYAWAY(KEY_TYPE::KEY_LEFT) || KEYAWAY(KEY_TYPE::KEY_RIGHT) || KEYAWAY(KEY_TYPE::KEY_UP))
				{
					m_bUpAnimed = false;
				}
			}
		}
		if (m_bIsAir)
		{
			if (KEYENTER(KEY_TYPE::KEY_UP))
			{
				Animator2D()->PlayAnimation(L"Player_Up_jump", false);
			}
			if (KEYENTER(KEY_TYPE::KEY_DOWN))
			{
				Animator2D()->PlayAnimation(L"Player_Downaim_jump", false);
			}
			if (((m_eOldKey == PLAYER_KEY::RIGHT || m_eOldKey == PLAYER_KEY::LEFT) && KEYENTER(KEY_TYPE::KEY_UP))
				|| m_eOldKey == PLAYER_KEY::TOP && (KEYENTER(KEY_TYPE::KEY_LEFT) || KEYENTER(KEY_TYPE::KEY_RIGHT)))
			{
				Animator2D()->PlayAnimation(L"Player_Upaim_jump", false);
			}
			if (((m_eOldKey == PLAYER_KEY::RIGHT || m_eOldKey == PLAYER_KEY::LEFT) && KEYENTER(KEY_TYPE::KEY_DOWN))
				|| m_eOldKey == PLAYER_KEY::BOTTOM && (KEYENTER(KEY_TYPE::KEY_LEFT) || KEYENTER(KEY_TYPE::KEY_RIGHT)))
			{

				Animator2D()->PlayAnimation(L"Player_Downaim_jump", false);
			}
		}

		if (KEYAWAY(KEY_TYPE::KEY_UP) || KEYAWAY(KEY_TYPE::KEY_DOWN) || KEYAWAY(KEY_TYPE::KEY_LEFT) || KEYAWAY(KEY_TYPE::KEY_RIGHT))
		{
			Animator2D()->PlayAnimation(L"Player_Idle", true);
		}
	}
}

void CTraceScript::CreateBullet()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	if (m_iGunType == 2)
	{
		if (m_iTryShootTime == 1)
		{
			m_iTryShootTime = 0.f;
			for (int i = 0; i < 6; ++i)
			{
				//여섯갈래로
				if (m_pBullet == nullptr)
					return;
				CGameObj* pBullet2 = m_pBullet->Clone();
				CBulletScript* pBulletScript = (CBulletScript*)pBullet2->GetScript<CBulletScript>();
				pBulletScript->SetDir((PLAYER_DIR)i);
				pBulletScript->laterDestroy();
				pBullet2->SetActive(true);
				pCurScene->AddObject(L"Bullet", pBullet2);
			}
			tEvent tEV;
			tEV.eType = EVENT_TYPE::DELETE_OBJECT;
			tEV.lParam = (INT_PTR)m_pBullet;
			CEventMgr::GetInst()->AddEvent(tEV);
			return;
		}
	}

	Vec3 vPPos = Object()->Transform()->GetLocalPos();
	CLayer* pLayer = pCurScene->FindLayer(L"Bullet");
	const vector<CGameObj*>& pVecObj = pLayer->GetParentObject();
	CGameObj* pBullet = pVecObj[0]->Clone();
	pBullet->AddComponent(new CBulletScript);
	pBullet->AddComponent(new CCollider2D);
	pBullet->Transform()->SetLocalScale(Vec3(44.f, 20.f, 1.f));
	CBulletScript* pBulletScript = (CBulletScript*)pBullet->GetScript<CBulletScript>();
	pBulletScript->SetPlayer(Object());
	//애니메이션 정보로 에임알아내기
	wstring AnimStr = Animator2D()->GetCurAnim()->GetName();

	if (!wcscmp(AnimStr.c_str(), L"Player_Walk") || !wcscmp(AnimStr.c_str(), L"Player_Idle"))
	{
		if (m_eType == PLAYER_DIR::LEFT)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x - 10.f, vPPos.y, vPPos.z));
			pBulletScript->SetDir(PLAYER_DIR::LEFT);
		}
		if (m_eType == PLAYER_DIR::RIGHT)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x + 10.f, vPPos.y, vPPos.z));
			pBulletScript->SetDir(PLAYER_DIR::RIGHT);
		}
	}
	else if (!wcscmp(AnimStr.c_str(), L"Player_Upaim_jump") || !wcscmp(AnimStr.c_str(), L"Player_Upaim_walk"))
	{
		if (m_eType == PLAYER_DIR::LEFT)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x - 5.f, vPPos.y + 5.f, vPPos.z));
			pBulletScript->SetDir(PLAYER_DIR::LT);
		}
		if (m_eType == PLAYER_DIR::RIGHT)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x + 5.f, vPPos.y + 5.f, vPPos.z));
			pBulletScript->SetDir(PLAYER_DIR::RT);
		}
	}
	else if (!wcscmp(AnimStr.c_str(), L"Player_Downaim_walk") || !wcscmp(AnimStr.c_str(), L"Player_Downaim_jump"))
	{
		if (m_eType == PLAYER_DIR::LEFT)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x - 5.f, vPPos.y - 5.f, vPPos.z));
			pBulletScript->SetDir(PLAYER_DIR::LB);
		}
		if (m_eType == PLAYER_DIR::RIGHT)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x + 5.f, vPPos.y - 5.f, vPPos.z));
			pBulletScript->SetDir(PLAYER_DIR::RB);
		}
	}
	else if (!wcscmp(AnimStr.c_str(), L"Player_Up_idle") || !wcscmp(AnimStr.c_str(), L"Player_Up_jump"))
	{
		pBullet->Transform()->SetLocalPos(Vec3(vPPos.x + 10.f, vPPos.y + 10.f, vPPos.z));
		pBulletScript->SetDir(PLAYER_DIR::TOP);
	}
	else
	{
		if (m_eAim == PLAYER_DIR::TOP)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x + 10.f, vPPos.y + 10.f, vPPos.z));
		}
		if (m_eAim == PLAYER_DIR::BOTTOM)
		{
			pBullet->Transform()->SetLocalPos(Vec3(vPPos.x + 10.f, vPPos.y - 10.f, vPPos.z));
		}
		pBulletScript->SetDir(m_eAim);

	}
	pBullet->SetActive(true);
	m_pBullet = pBullet;

	//기본탄
	if (m_iGunType == 1)
	{
		pCurScene->AddObject(L"Bullet", pBullet);
	}

	//노바탄
	else if (m_iGunType == 2)
	{
		if (m_iTryShootTime == 1)
		{
			m_iTryShootTime = 0.f;
		}
		pBullet->Animator2D()->PlayAnimation(L"Gun_Bullet2", true);
		++m_iTryShootTime;
		pCurScene->AddObject(L"Bullet", pBullet);
	
	}
}

void CTraceScript::GameOver()
{
	Animator2D()->PlayAnimation(L"Player_Dead", false);
}

void CTraceScript::CreateHpBar()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	//Hp바 만들기
	CGameObj* pHpBarObjct = new CGameObj;
	pHpBarObjct->SetName(L"HpBar");
	CTransform* pTransform = new CTransform;
	CMeshRender* pMeshRender = new CMeshRender;
	pTransform->SetLocalPos(Vec3(-350.f, 220.f, 200.f));
	pTransform->SetLocalScale(Vec3(160.f, 32.f, 1.f));
	pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Hpbar3.png"));
	pHpBarObjct->AddComponent(pTransform);
	pHpBarObjct->AddComponent(pMeshRender);
	pCurScene->AddObject(L"UI", pHpBarObjct);

	m_pHpObject = new CGameObj;
	m_pHpObject->SetName(L"Hp");
	int HpShader = 3;
	pTransform = new CTransform;
	pMeshRender = new CMeshRender;
	pTransform->SetLocalPos(Vec3(-350.f, 220.f, 300.f));
	pTransform->SetLocalScale(Vec3(180.f, 32.f, 1.f));
	pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Hpbar2.png"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &HpShader);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fHp);
	m_pHpObject->AddComponent(pTransform);
	m_pHpObject->AddComponent(pMeshRender);
	pCurScene->AddObject(L"UI", m_pHpObject);
}

void CTraceScript::CreateGunUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//Gun UI틀 만들기
	CGameObj* pGunUIObjct = new CGameObj;
	pGunUIObjct->SetName(L"GunFrame");
	CTransform* pTransform = new CTransform;
	CMeshRender* pMeshRender = new CMeshRender;
	pTransform->SetLocalPos(Vec3(400.f, 220.f, 200.f));
	pTransform->SetLocalScale(Vec3(30.f, 30.f, 1.f));
	pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\GunUI.PNG"));
	pGunUIObjct->AddComponent(pTransform);
	pGunUIObjct->AddComponent(pMeshRender);
	pCurScene->AddObject(L"UI", pGunUIObjct);

	CreateGunIcon();
}

void CTraceScript::CreateGunIcon()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	m_iGunType = ((CInfoScript*)m_pScript)->GetPlayerWeapon();
	if (m_iGunType == 1 || m_iGunType == 2)
	{
		//Gun Icon만들기
		m_pGunUI = new CGameObj;
		m_pGunUI->SetName(L"GunIcon");
		CTransform* pTransform = new CTransform;
		CMeshRender* pMeshRender = new CMeshRender;
		pTransform->SetLocalPos(Vec3(402.f, 218.f, 200.f));
		pTransform->SetLocalScale(Vec3(15.f, 14.f, 1.f));
		pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
		if (m_iGunType == 1)
			pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\IconGun1.png"));
		else if (m_iGunType == 2)
			pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\IconGun2.png"));
		m_pGunUI->AddComponent(pTransform);
		m_pGunUI->AddComponent(pMeshRender);
		pCurScene->AddObject(L"UI", m_pGunUI);
		m_AbleGunIcon = true;
	}
}

void CTraceScript::OnCollisionEnter(CCollider2D * _pOther)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//타일과 충돌할 경우
	if (_pOther->Object()->GetLayerIdx() == pCurScene->FindLayer(L"Tile")->GetLayerIdx())
	{
		CTransform* pPlayerTrans = this->Object()->Transform();
		CCollider2D* pPlayerColl = this->Object()->Collider2D();
		CTransform* pTileTrans = _pOther->Object()->Transform();
		const Vec3 pPlayerPos = pPlayerTrans->GetLocalPos();
		const Vec3 pPlayerLT = pPlayerColl->GetLT();
		const Vec3 pPlayerRB = pPlayerColl->GetRB();
		const Vec3 pTilePos = pTileTrans->GetLocalPos();
		const Vec3 pTileSize = pTileTrans->GetLocalScale();

		++m_iTouchCount;

		if (m_bIsAir)
		{
			//착지하는 경우
			if (pTilePos.y + pTileSize.y / 2 - 10.f <= pPlayerRB.y)
			{
				pPlayerTrans->SetLocalPos(Vec3(pPlayerPos.x
					, pPlayerPos.y + (pTilePos.y + pTileSize.y / 2 - pPlayerRB.y)
					, pPlayerPos.z));
				m_bIsAir = false;
				return;
			}
			//플레이어 머리에 부뒷쳐서 내려가는경우
			if (pTilePos.y + pTileSize.y / 2 >= pPlayerLT.y)
			{
				if (!m_bDownJump)
				{
					m_vForce = Vec3(0.f, 0.f, 0.f);
					return;
				}
			}
			//타일에 옆쪽에 부뒷쳐서 내려가는경우
			if (pTilePos.x + pTileSize.x /2 <= pPlayerRB.x
				|| pTilePos.x - pTileSize.x / 2 >= pPlayerLT.x)
			{
				if (pPlayerRB.x <= pTilePos.x && (m_eType == PLAYER_DIR::RIGHT))
				{
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x - pTileSize.x / 2 - 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}
				if (pPlayerLT.x >= pTilePos.x && (m_eType == PLAYER_DIR::LEFT))
				{
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x + pTileSize.x / 2 + 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}
				return;
			}
		}
		else
		{
			if (m_iTouchCount >= 2)
			{
				if (pPlayerRB.x <= pTilePos.x - pTileSize.x /2 + 2.f && (m_eType == PLAYER_DIR::RIGHT))
				{
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x - pTileSize.x / 2 - 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}

				if (pPlayerLT.x >= pTilePos.x + pTileSize.x / 2 + 2.f && (m_eType == PLAYER_DIR::LEFT))
				{
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x + pTileSize.x / 2 + 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}
			}
		}
	}
}

void CTraceScript::OnCollision(CCollider2D * _pOther)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	if (_pOther->Object()->GetLayerIdx() == pCurScene->FindLayer(L"Tile")->GetLayerIdx())
	{
		CTransform* pPlayerTrans = this->Object()->Transform();
		CCollider2D* pPlayerColl = this->Object()->Collider2D();
		CTransform* pTileTrans = _pOther->Object()->Transform();
		const Vec3 pPlayerPos = pPlayerTrans->GetLocalPos();
		const Vec3 pPlayerLT = pPlayerColl->GetLT();
		const Vec3 pPlayerRB = pPlayerColl->GetRB();
		const Vec3 pTilePos = pTileTrans->GetLocalPos();
		const Vec3 pTileSize = pTileTrans->GetLocalScale();
	
		if (m_bIsAir)
		{
			if (pTilePos.x + pTileSize.x / 2 <= pPlayerRB.x
				|| pTilePos.x - pTileSize.x / 2 >= pPlayerLT.x)
			{
				if (pPlayerRB.x < pTilePos.x && (m_eType == PLAYER_DIR::RIGHT))
				{
					if (PLAYER_KEY::END == m_eOldKey)
						return;
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x - pTileSize.x / 2 - 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}
				if (pPlayerLT.x > pTilePos.x && (m_eType == PLAYER_DIR::LEFT))
				{
					if (PLAYER_KEY::END == m_eOldKey)
						return;
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x + pTileSize.x / 2 + 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}
				return;
			}
		}

		//player blocking
		if (!m_bIsAir)
		{
			//플레이어 발 밑은 적용하지 않는다.
			if (m_iTouchCount >= 2 && pPlayerRB.y + 2.f <= pTilePos.y + pTileSize.y / 2)
			{
				if (pPlayerRB.x < pTilePos.x && (m_eType == PLAYER_DIR::RIGHT))
				{
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x - pTileSize.x / 2 - 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}

				if (pPlayerLT.x > pTilePos.x && (m_eType == PLAYER_DIR::LEFT))
				{
					pPlayerTrans->SetLocalPos(Vec3(pTilePos.x + pTileSize.x / 2 + 15, pPlayerPos.y, pPlayerPos.z));
					return;
				}
			}
		}

	}
}

void CTraceScript::OnCollisionExit(CCollider2D * _pOther)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//타일과 충돌에서 벗어나는 경우
	if (_pOther->Object()->GetLayerIdx() == pCurScene->FindLayer(L"Tile")->GetLayerIdx())
	{
		--m_iTouchCount;
		if (0 == m_iTouchCount)
			m_bIsAir = true;
	}
}

