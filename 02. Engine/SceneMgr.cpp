#include "stdafx.h"
#include "SceneMgr.h"

#include "Scene.h"
#include "Layer.h"

#include "GameObj.h"

#include "MeshRender.h"
#include "Transform.h"
#include "Camera.h"
#include "Collider2D.h"
#include "Animator2D.h"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Sound.h"
#include "Texture.h"

#include "ResMgr.h"
#include "ColliderMgr.h"
#include "EventMgr.h"

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
	, m_pInfoScene(nullptr)
	, m_pSound(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	if (m_pCurScene == m_pMapScene)
		m_pMapScene = nullptr;
	else if (m_pCurScene == m_pToolScene)
	{
		m_pToolScene = nullptr;
		m_pMapScene = nullptr;
	}

	SAFE_DELETE(m_pCurScene);
	SAFE_DELETE(m_pMapScene);
	SAFE_DELETE(m_pToolScene);
	SAFE_DELETE(m_pInfoScene);
}

void CSceneMgr::RegisterCamera(CCamera * _pCam)
{
	m_pCurScene->AddCamera(_pCam);
}

void CSceneMgr::FindGameObject(const wstring & _strTagName, vector<CGameObj*>& _vecOut)
{
	m_pCurScene->FindGameObject(_strTagName, _vecOut);
}


void CSceneMgr::Init()
{
	//// Resource Load
	//CreatePrefab();
	//LoadTexture();

	//임시 Scene 생성
	//m_pMapScene = new CScene;
	m_pToolScene = new CScene;
	m_pInfoScene = new CScene;
	m_pCurScene = m_pToolScene;

	// Layer 추가하기
	//m_pMapScene->AddLayer(L"Default", 0);
	m_pToolScene->AddLayer(L"Default", 0);
	m_pInfoScene->AddLayer(L"Default", 0);
}

void CSceneMgr::progress()
{
	m_pCurScene->Update();
	m_pCurScene->Lateupdate();
	m_pCurScene->Finalupdate();

	CColliderMgr::GetInst()->Update();
	CEventMgr::GetInst()->Update();

	m_pInfoScene->Update();
}

void CSceneMgr::progress_pause()
{
	m_pCurScene->Finalupdate();
	CEventMgr::GetInst()->Update();
}

void CSceneMgr::Render()
{
	m_pCurScene->Render();
}

void CSceneMgr::CreatePrefab()
{
	//Monster Object 만들기
	//CGameObj* pObj = new CGameObj;
	//
	//CTransform* pTransform = new CTransform;
	//CMeshRender* pMeshRender = new CMeshRender;
	//
	//pTransform->SetLocalPos(Vec3(100.f, 100.f, 500.f));
	//pTransform->SetLocalScale(Vec3(50.f, 50.f, 1.f));
	//pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TextureShader"));
	//
	//pObj->AddComponent(pTransform);
	//pObj->AddComponent(pMeshRender);
	//pObj->AddComponent(new CMonsterScript);
	//
	//CMonsterScript* MonsterScript = (CMonsterScript*)pObj->GetScript<CMonsterScript>();
	//MonsterScript->SetSize(Vec2(50.f, 50.f));
	//
	//CResMgr::GetInst()->AddRes<CPrefab>(L"MonsterPrefab", new CPrefab(pObj));
}

void CSceneMgr::LoadTexture()
{
	CResMgr::GetInst()->Load<CTexture>(L"Player", L"Texture\\00.png");
	CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Ghost1.png");
	CResMgr::GetInst()->Load<CTexture>(L"Link", L"Texture\\Animation\\Link\\link_0.png");
	CResMgr::GetInst()->Load<CTexture>(L"TraceUnarmed", L"Texture\\Animation\\Trace\\TraceUnarmed.png");
	CResMgr::GetInst()->Load<CTexture>(L"TileMap", L"Texture\\Area1(clear).png");
}
