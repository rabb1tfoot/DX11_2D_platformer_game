// BtnView.cpp: 구현 파일
//

#include "stdafx.h"
#include "BtnView.h"
#include "MainFrm.h"
#include "ComponentView.h"
#include "HierachyView.h"
#include "CreateTile.h"
#include "CollisionCheck.h"
#include "GameView.h"
#include "GameObjectDlg.h"

#include <Core.h>
#include <TimeMgr.h>
#include <SceneMgr.h>
#include <ColliderMgr.h>
#include <Scene.h>
#include <GameObj.h>
#include <Camera.h>
#include <DialogueMgr.h>
#include <Transform.h>
#include <MeshRender.h>
#include <Mesh.h>
#include <ResMgr.h>
#include <Texture.h>
#include <PanelScript.h>
#include <Collider2D.h>
#include <CameraScipt.h>
#include <Layer.h>


// CBtnView

IMPLEMENT_DYNCREATE(CBtnView, CView)

CBtnView::CBtnView()
	: CFormView(IDD_BTNVIEW)
	, m_bMapToolOn(false)
{

}

CBtnView::~CBtnView()
{
}

BEGIN_MESSAGE_MAP(CBtnView, CView)
	ON_BN_CLICKED(IDC_BUTTON1, &CBtnView::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BUTTON2, &CBtnView::OnBnClickedButtonMain)
	ON_BN_CLICKED(IDC_CHECK2, &CBtnView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_CHECK1, &CBtnView::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON3, &CBtnView::OnBnClickedCollisionCheck)
END_MESSAGE_MAP()


// CBtnView 그리기

void CBtnView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}

void CBtnView::DoDataExchange(CDataExchange * pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK1, m_btnPlay);
	DDX_Control(pDX, IDC_CHECK2, m_btnPause);
	DDX_Control(pDX, IDC_CHECK3, m_btnNextFrm);

	CRect rt1, rt2, rt3;
	m_btnPlay.GetWindowRect(rt1);
	m_btnPlay.GetWindowRect(rt2);
	m_btnPlay.GetWindowRect(rt3);

	float fAccWidth = rt1.Width() + rt2.Width() + rt3.Width();

	CRect rtBtnView;
	GetClientRect(rtBtnView);

	float fStartX = (rtBtnView.Width() / 2.f) - (fAccWidth / 2.f);

	m_btnPlay.SetWindowPos(nullptr, fStartX, 10, rt1.Width(), rt1.Height(), 0);

	fStartX += rt1.Width();

	m_btnPause.SetWindowPos(nullptr, fStartX, 10, rt2.Width(), rt2.Height(), 0);

	fStartX += rt2.Width();

	m_btnNextFrm.SetWindowPos(nullptr, fStartX, 10, rt3.Width(), rt3.Height(), 0);
}


// CBtnView 진단

#ifdef _DEBUG
void CBtnView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBtnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBtnView 메시지 처리기


void CBtnView::init()
{
}

void CBtnView::OnBnClickedButtonMap()
{
	if (GetMapToolOn())
		return;

	//맵 툴 기능으로 변경
	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	frame->SetType(TOOL_TYPE::TOOL_MAP);

	//맵 크기를 받아서 디폴트 맵을 만든다.
	CCreateTile TileDlg;
	INT_PTR  CreateId = TileDlg.DoModal();
	if (IDOK == CreateId)
	{
		SetMapToolOn(true);

		CreateDefaultTileMap();

		//맵 툴 카메라로 변경
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		vector<CGameObj*> VecCam;
		pCurScene->FindGameObject(L"MapCam", VecCam);
		CDialogueMgr::GetInst()->SetString(L"맵 툴");
		CDialogueMgr::GetInst()->SetLoc(Vec2(500.f, 600.f));

		((CGameView*)frame->GetGameView())->SetMapCamObj(VecCam[0]);

		//하이어라키 갱신
		CHierachyView* pComView = (CHierachyView*)((CMainFrame*)AfxGetMainWnd())->GetHierachyView();
		pComView->GetGameObjDlg()->init();

	}
}

void CBtnView::OnBnClickedButtonMain()
{
	if (!GetMapToolOn())
		return;

	//메인 기능으로 변경
	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	frame->SetType(TOOL_TYPE::TOOL_MAIN);

	SetMapToolOn(false);
	CScene* pMapScene = CSceneMgr::GetInst()->GetCurScene();
	SAFE_DELETE(pMapScene);

	CSceneMgr::GetInst()->ChangeCurScene(CSceneMgr::GetInst()->GetToolScene());

	//메인 카메라로 변경
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CGameObj*> VecObj;
	CGameObj* CamObj = nullptr;
	CLayer** pLayer = pCurScene->GetAllLayer();
	VecObj = pLayer[0]->GetParentObject();

	for (int i = 0; i < VecObj.size(); ++i)
	{
		if (!wcscmp(L"MainCam", VecObj[i]->GetName().c_str()))
			CamObj = VecObj[i];
	}

	CamObj->Camera()->SetRender(true);

	CDialogueMgr::GetInst()->SetString(L"");//L"메인 툴: "
	CDialogueMgr::GetInst()->SetLoc(Vec2(500.f, 600.f));

	// 충돌 지정
	CColliderMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Player", L"Tile");
	CColliderMgr::GetInst()->CollisionCheck(L"Default", L"Player");
	CColliderMgr::GetInst()->CollisionCheck(L"Default", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Bullet", L"Tile");

	//하이어라키 갱신
	CHierachyView* pComView = (CHierachyView*)((CMainFrame*)AfxGetMainWnd())->GetHierachyView();
	pComView->GetGameObjDlg()->init();
}


void CBtnView::OnBnClickedCollisionCheck()
{
	CCollisionCheck CollisionDlg;
	INT_PTR  CreateId = CollisionDlg.DoModal();
}


void CBtnView::OnBnClickedPlay()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	::SetFocus(pMainFrm->GetGameView()->m_hWnd);

	if (!!m_btnPlay.GetCheck())
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		pCurScene->Awake();
		pCurScene->Start();

		if (m_btnPause.GetCheck())
			CCore::GetInst()->Play(false);
		else
			CCore::GetInst()->Play(true);
	}
}


void CBtnView::OnBnClickedPause()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	::SetFocus(pMainFrm->GetGameView()->m_hWnd);

	if (!m_btnPlay.GetCheck())
		return;

	if (!m_btnPause.GetCheck())
	{
		CCore::GetInst()->Play(true);
	}
	else
	{
		CCore::GetInst()->Play(false);
	}
}



void CBtnView::CreateDefaultTileMap()
{
	CScene* pCurScene;
	pCurScene = new CScene;
	pCurScene->AddLayer(L"Default", 0);
	pCurScene->AddLayer(L"Tile", 4);
	pCurScene->AddLayer(L"UI", 5);
	pCurScene->AddLayer(L"SelectTile", 6);

	CGameObj* pCamObj = new CGameObj;
	pCamObj->SetName(L"MapCam");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraScipt);
	pCamObj->Camera()->SetRender(true);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetFOV(XM_PI / 4.f);
	pCamObj->Camera()->SetScale(1.f);
	pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Tile")->GetLayerIdx());

	pCurScene->AddObject(L"Default", pCamObj);

	pCamObj = new CGameObj;
	pCamObj->SetName(L"UICam");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Camera()->SetRender(true);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetFOV(XM_PI / 4.f);
	pCamObj->Camera()->SetScale(1.f);
	pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"UI")->GetLayerIdx());
	pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"SelectTile")->GetLayerIdx());

	pCurScene->AddObject(L"Default", pCamObj);

	CColliderMgr::GetInst()->ClearCheck();

	// 패널 만들기 //디폴트 타일 만들기
	CGameObj* pObj = new CGameObj;
	pObj->SetName(L"TilePanel");

	CTransform* pTrans = new CTransform;
	CMeshRender* pMeshRender = new CMeshRender;
	CResPtr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Area1(clear)2.png");
	CResPtr<CTexture> pTex2 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Area2(clear)2.png");

	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	pTrans->SetLocalPos(Vec3(411.f, 180.f, 500.f));
	pTrans->SetLocalScale(Vec3(pTex->GetWidth(), pTex->GetHeight(), 1.f));

	pObj->AddComponent(pTrans);
	pObj->AddComponent(pMeshRender);
	pObj->AddComponent(new CPanelScript);

	pCurScene->AddObject(L"UI", pObj);

	//게임뷰에서 관리
	CGameView* pGameView = (CGameView*)(((CMainFrame*)AfxGetMainWnd())->GetGameView());
	pGameView->SetTileUI(pObj);

	CGameObj* pTargetTile = new CGameObj;
	pTargetTile->SetName(L"TargetTile");

	pTrans = new CTransform;
	pMeshRender = new CMeshRender;

	int RenderTile = 1;
	float fWidth = pTex->GetWidth();
	float fHeight = pTex->GetHeight();
	Vec2 TileLT = Vec2(CLEAR_TILE_X / fWidth, (CLEAR_TILE_Y) / fHeight);
	Vec2 TileSize = Vec2(TILE_SIZE / fWidth, TILE_SIZE / fHeight);

	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &RenderTile);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_0, &TileLT);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_1, &TileSize);

	pTrans->SetLocalPos(Vec3(0.f, 0.f, 500.f));
	pTrans->SetLocalScale(Vec3(TILE_SIZE * 2, TILE_SIZE * 2, 1.f));

	pTargetTile->AddComponent(pTrans);
	pTargetTile->AddComponent(pMeshRender);

	pCurScene->AddObject(L"SelectTile", pTargetTile);

	//게임뷰에서 관리

	pGameView->SetTargetTileObj(pTargetTile);
	pGameView->SetTileTex(pTex);

	//전체 맵 타일 만들기
	CGameObj* pMap = new CGameObj;
	pObj->SetName(L"MapTile");
	int TileTexShader[900];
	Vec2 TileMapLT[900];
	for (UINT i = 0; i < 900; ++i)
	{
		TileTexShader[i] = 1;
		TileMapLT[i] = Vec2(256 / fWidth, 0.f / fHeight);
	}
	RenderTile = 2;
	
	pTrans = new CTransform;
	pMeshRender = new CMeshRender;
	
	TileSize = Vec2(TILE_SIZE * m_iMapSize.x / fWidth, TILE_SIZE * m_iMapSize.y / fHeight);
	Vec2 TileNum = Vec2(m_iMapSize.x, m_iMapSize.y);
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_1, &pTex2);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &RenderTile);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::INT_ARRAY, TileTexShader);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_ARRAY, TileMapLT);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_1, &TileSize);
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_2, &TileNum);
	
	pTrans->SetLocalPos(Vec3(-100.f + 0, 0, 500.f));
	pTrans->SetLocalScale(Vec3(m_iMapSize.x * TILE_SIZE * 2, m_iMapSize.y * TILE_SIZE * 2, 1.f));
	
	pMap->AddComponent(pTrans);
	pMap->AddComponent(pMeshRender);
	
	pCurScene->AddObject(L"Tile", pMap);
	pGameView->SetMapTileObj(pMap);
	
	CSceneMgr::GetInst()->SetMapScene(pCurScene);
	CSceneMgr::GetInst()->ChangeCurScene(CSceneMgr::GetInst()->GetMapScene());

}