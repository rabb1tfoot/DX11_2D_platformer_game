
// 03. Tool.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ToolApp.h"
#include "MainFrm.h"
#include "BtnView.h"
#include "GameView.h"
#include "HierachyView.h"

#include <Core.h>
#include <TimeMgr.h>
#include <ResMgr.h>
#include <ColliderMgr.h>
#include <Collider2D.h>
#include <Animator2D.h>
#include <Transform.h>
#include <Camera.h>
#include <CameraScipt.h>
#include <SceneMgr.h>
#include <GameObj.h>
#include <Scene.h>
#include <Layer.h>

#include <TraceScript.h>
#include <HilightScript.h>
#include <MonsterSCript.h>
#include <PanelScript.h>
#include <BulletScript.h>
#include <BlackoutScript.h>
#include <DoorScript.h>
#include <SnailScript.h>
#include <HiveScript.h>
#include <BlockScript.h>
#include <WaitCam.h>
#include <TitleGround.h>
#include <InfoScript.h>
#include <GunItemScript.h>
#include <BossScript.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolApp

BEGIN_MESSAGE_MAP(CToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CToolApp::OnAppAbout)
END_MESSAGE_MAP()


// CToolApp 생성

CToolApp::CToolApp() noexcept
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("My03Tool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CToolApp 개체입니다.

CToolApp theApp;


// CToolApp 초기화

BOOL CToolApp::InitInstance()
{
	//_CrtSetBreakAlloc(53608);

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));


	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 프레임을 만들어 리소스와 함께 로드합니다.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	HWND hGameViewHwnd = ((CMainFrame*)pFrame)->GetGameView()->m_hWnd;

	// Engine 초기화
	CCore::GetInst()->Init(hGameViewHwnd, true);

	// Engine 초기화 후에 FrameWindow 초기화
	((CMainFrame*)pFrame)->init();

	CScene* pInfoScene = CSceneMgr::GetInst()->GetInfoScene();
	//플레이어 필요한 정보용 오브젝트 만들기
	CGameObj* pInfoObj = new CGameObj;
	pInfoObj->SetName(L"Info");
	pInfoObj->AddComponent(new CTransform);
	pInfoObj->AddComponent(new CInfoScript);
	pInfoScene->AddObject(0, pInfoObj);

	CreateTestScene();

	((CBtnView*)((CMainFrame*)pFrame)->GetHierachyView())->init();
	((CGameView*)((CMainFrame*)pFrame)->GetHierachyView())->init();

	// HierachyView 초기화
	CHierachyView* pView = (CHierachyView*)((CMainFrame*)pFrame)->GetHierachyView();
	pView->init_object();

	return TRUE;
}

int CToolApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CToolApp 메시지 처리기

BOOL CToolApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CCore::GetInst()->Progress();

	((CMainFrame*)m_pMainWnd)->update();

	return CWinApp::OnIdle(0); // 지연시간 제거
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedFrmPlay();
	afx_msg void OnBnClickedCollisionCheck();
	afx_msg void OnCreateEmptyobject();
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK1, &CAboutDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_CHECK3, &CAboutDlg::OnBnClickedFrmPlay)
	ON_BN_CLICKED(IDC_BUTTON3, &CAboutDlg::OnBnClickedCollisionCheck)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_GAMEOBJECT_EMPTYOBJECT, &CAboutDlg::OnCreateEmptyobject)
END_MESSAGE_MAP()

void CToolApp::CreateTestScene()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->OnLoadScene();
	return;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddLayer(L"Default", 0);
	pCurScene->AddLayer(L"Player", 1);
	pCurScene->AddLayer(L"Bullet", 2);
	pCurScene->AddLayer(L"Monster", 3);
	pCurScene->AddLayer(L"Tile", 4);
	pCurScene->AddLayer(L"UI", 5);
	pCurScene->AddLayer(L"Background", 7);
	CLayer** pLayer = pCurScene->GetAllLayer();
	vector<CGameObj*> vecObj = pLayer[0]->GetParentObject();
	for (int i = 0; i < vecObj.size(); ++i)
	{
		if (!wcscmp(vecObj[i]->GetName().c_str(), L"MainCam"))
		{
			CScript* pScript = vecObj[i]->GetScript<CCameraScipt>();
			Vec2 Bound[2] = { Vec2(-100.f, -4.f), Vec2(-100.f, 560.f) };
			((CCameraScipt*)pScript)->SetBoundVec(Bound);
		}
	}
	return;

	////Camera Object 만들기
	//CGameObj* pCamObj = new CGameObj;
	//pCamObj->SetName(L"MainCam");
	//CTransform* pTransform = new CTransform;
	//CCamera* pCamera = new CCamera;
	//CScript* pScript = new CCameraScipt;
	//pTransform->SetLocalPos(Vec3(-96.f, -1.f, 0.f));
	//pTransform->SetLocalScale(Vec3(0.f, 0.f, 0.f));
	//pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pCamera->SetScale(0.85f);
	//Vec2 vBound[2] = { Vec2(-96.f, -1.f), Vec2(770.f, -1.f) };
	//((CCameraScipt*)pScript)->SetBoundVec(vBound);
	//((CCameraScipt*)pScript)->SetChasing();
	//pCamObj->AddComponent(pTransform);
	//pCamObj->AddComponent(pCamera);
	//pCamObj->AddComponent(pScript);
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Player")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Bullet")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Monster")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Tile")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Background")->GetLayerIdx());
	//pCurScene->AddObject(L"Default", pCamObj);
	//return;
	//return;
	//
	////Camera Object 만들기
	//CGameObj* pCamObj = new CGameObj;
	//pCamObj->SetName(L"UICam");
	//CTransform* pTransform = new CTransform;
	//CCamera* pCamera = new CCamera;
	//pTransform->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//pTransform->SetLocalScale(Vec3(0.f, 0.f, 0.f));
	//pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pCamera->SetScale(0.85f);
	//pCamObj->AddComponent(pTransform);
	//pCamObj->AddComponent(pCamera);
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"UI")->GetLayerIdx());
	//pCurScene->AddObject(L"Default", pCamObj);
	////아이템만들기
	//CGameObj* pItemObj = new CGameObj;
	//pItemObj->SetName(L"Item");
	//pTransform = new CTransform;
	//pTransform->SetLocalPos(Vec3(0.f, 0.f, 500.f));
	//pTransform->SetLocalScale(Vec3(15.f, 14.f, 1));
	//pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//CMeshRender* pMeshRender = new CMeshRender;
	//CScript* pScript = new CGunItemScript;
	//((CGunItemScript*)pScript)->SetGunType(2);
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\IconGun2.png"));
	//pItemObj->AddComponent(pScript);
	//pItemObj->AddComponent(new CCollider2D);
	//pItemObj->AddComponent(pTransform);
	//pItemObj->AddComponent(pMeshRender);
	//pCurScene->AddObject(L"Default", pItemObj);
	////타이틀 이름띄우기
	//CGameObj* pTitle = new CGameObj;
	//pTitle->SetName(L"Title");
	//
	//CTransform* pTransform = new CTransform;
	//CMeshRender* pMeshRender = new CMeshRender;
	//
	//pTransform->SetLocalPos(Vec3(0.f, 0.f, 200.f));
	//pTransform->SetLocalScale(Vec3(480.f, 272.f, 1.f));
	//pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\TitleScreen.png"));
	//
	//
	//pTitle->AddComponent(pTransform);
	//pTitle->AddComponent(pMeshRender);
	//
	//pCurScene->AddObject(L"Default", pTitle);
	//
	////백그라운드 띄우기
	//CGameObj* pBackground = new CGameObj;
	//pBackground->SetName(L"Background");
	//pTransform = new CTransform;
	//pMeshRender = new CMeshRender;
	//pTransform->SetLocalPos(Vec3(0.f, 0.f, 300.f));
	//pTransform->SetLocalScale(Vec3(480.f, 1904.f, 1.f));
	//pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//CScript* pScript = new CTitleGround;
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\TitleScreen1.png"));
	//pBackground->AddComponent(pTransform);
	//pBackground->AddComponent(pMeshRender);
	//pBackground->AddComponent(pScript);
	//pCurScene->AddObject(L"Background", pBackground);
	//
	//CGameObj* pAnyKey = new CGameObj;
	//pAnyKey->SetName(L"PushKey");
	//pTransform = new CTransform;
	//pMeshRender = new CMeshRender;
	//pTransform->SetLocalPos(Vec3(0.f, -100.f, 200.f));
	//pTransform->SetLocalScale(Vec3(80.f, 8.f, 1.f));
	//pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\PushStart2.png"));
	//pAnyKey->AddComponent(pTransform);
	//pAnyKey->AddComponent(pMeshRender);
	//pCurScene->AddObject(L"Default", pAnyKey);
	//
	//CGameObj* pBlackObj = new CGameObj;
	//pBlackObj->SetName(L"BlackImage2");
	//CTransform* pTrans = new CTransform;
	//pMeshRender = new CMeshRender;
	//pTrans->SetLocalPos(Vec3(0.f, 0.f, 500.f));
	//pTrans->SetLocalScale(Vec3(15040.f, 15440.f, 1.f));
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Black.png"));
	//pBlackObj->AddComponent(pTrans);
	//pBlackObj->AddComponent(pMeshRender);
	//pCurScene->AddObject(L"Background", pBlackObj);
	//
	//Camera Object 만들기
	//CGameObj* pCamObj = new CGameObj;
	//pCamObj->SetName(L"MainCam");
	//pScript = new CWaitCam;
	//pTransform = new CTransform;
	//CCamera* pCamera = new CCamera;
	//pTransform->SetLocalPos(Vec3(0.f, 800.f, 0.f));
	//pTransform->SetLocalScale(Vec3(0.f, 0.f, 0.f));
	//pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pCamera->SetScale(0.425f);
	////Vec2 vBound[2] = { Vec2(-103.f, 30.f), Vec2(-103.f, 1378.f) };
	////((CCameraScipt*)pScript)->SetBoundVec(vBound);
	////((CCameraScipt*)pScript)->SetChasing();
	//pCamObj->AddComponent(pTransform);
	//pCamObj->AddComponent(pCamera);
	//pCamObj->AddComponent(pScript);
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Player")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Bullet")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Monster")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Tile")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Background")->GetLayerIdx());
	//pCurScene->AddObject(L"Default", pCamObj);
	//
	//CGameObj* pCamObj2 = new CGameObj;
	//pCamObj2->SetName(L"MainCam2");
	//pTransform = new CTransform;
	//pCamera = new CCamera;
	//pTransform->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//pTransform->SetLocalScale(Vec3(0.f, 0.f, 0.f));
	//pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pCamera->SetScale(0.65f);
	//pCamObj2->AddComponent(pTransform);
	//pCamObj2->AddComponent(pCamera);
	//pCamObj2->Camera()->CheckLayer(pCurScene->FindLayer(L"Default")->GetLayerIdx());
	//pCurScene->AddObject(L"Default", pCamObj2);
	//return;

	//하이브 만들기
	//for (int i = 0; i < 2; ++i)
	//{
	//	CGameObj* pHive = new CGameObj;
	//	pHive->SetName(L"Hive");
	//
	//	CTransform* pTransform = new CTransform;
	//	pTransform->SetLocalPos(Vec3(-90.f, -40.f, 500.f));
	//	pTransform->SetLocalScale(Vec3(64.f, 64.f, 1.f));
	//	CMeshRender* pMeshRender = new CMeshRender;
	//	CCollider2D* pCollider2D = new CCollider2D;
	//	CAnimator2D* pAnim = new CAnimator2D;
	//	CScript* pMonsterScript = new CMonsterScript;
	//	CScript* pHiveScript = new CHiveScript;
	//	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Hit.PNG"));
	//	CResPtr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"Texture\\Animation\nest(clear).png", L"Texture\\Animation\\nest(clear).png");
	//	pAnim->AddAnimation(L"Hive_Idle", pTex, Vec2(0, 0), Vec2(64, 64), 6, 0.2f);
	//	pAnim->AddAnimation(L"Hive_Dead", pTex, Vec2(0, 80), Vec2(64, 70), 5, 0.1f);
	//	pAnim->PlayAnimation(L"Hive_Idle", true);
	//	((CMonsterScript*)pMonsterScript)->SetHp(15);
	//	pHive->AddComponent(pTransform);
	//	pHive->AddComponent(pMeshRender);
	//	pHive->AddComponent(pCollider2D);
	//	pHive->AddComponent(pAnim);
	//	pHive->AddComponent(pMonsterScript);
	//	pHive->AddComponent(pHiveScript);
	//	pCurScene->AddObject(L"Monster", pHive);
	//	//((CHiveScript*)pHiveScript)->Init();
	//}
	//return;
	//
	//	pCurScene->AddObject(L"Monster", pSnail);

	//스네일 만들기
	//for (int i = 0; i < 3; ++i)
	//{
	//	CGameObj* pSnail = new CGameObj;
	//	pSnail->SetName(L"Snail");
	//
	//	CTransform* pTransform = new CTransform;
	//	pTransform->SetLocalPos(Vec3(-90.f, -40.f, 500.f));
	//	pTransform->SetLocalScale(Vec3(32.f, 32.f, 1.f));
	//	CMeshRender* pMeshRender = new CMeshRender;
	//	CCollider2D* pCollider2D = new CCollider2D;
	//	CAnimator2D* pAnim = new CAnimator2D;
	//	CScript* pMonsterScript = new CMonsterScript;
	//	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//	pMeshRender->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\00.png"));
	//	CResPtr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Snail2.png");
	//	pAnim->AddAnimation(L"Snail_Move", pTex, Vec2(0, 0), Vec2(20, 20), 4, 0.3f);
	//	pAnim->PlayAnimation(L"Snail_Move", true);
	//	((CMonsterScript*)pMonsterScript)->SetHp(5);
	//	pSnail->AddComponent(pTransform);
	//	pSnail->AddComponent(pMeshRender);
	//	pSnail->AddComponent(pCollider2D);
	//	pSnail->AddComponent(pAnim);
	//	pSnail->AddComponent(new CSnailScript);
	//	pSnail->AddComponent(pMonsterScript);
	//
	//	pCurScene->AddObject(L"Monster", pSnail);
	//}
	////return;
	

		//충돌 지정
	CColliderMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Player", L"Tile");
	CColliderMgr::GetInst()->CollisionCheck(L"Default", L"Player");
	CColliderMgr::GetInst()->CollisionCheck(L"Default", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Bullet", L"Tile");
	CColliderMgr::GetInst()->CollisionCheck(L"Bullet", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Monster", L"Tile");

	//타일만들기
	//for (int i = 0; i <14; ++i)
	//{
	//	CGameObj* pTile = new CGameObj;
	//	wstring TileName = L"Tile";
	//	wchar_t wstr[6] = L"";
	//	wsprintf(wstr, L"%d", i);
	//	TileName += wstr;
	//	pTile->SetName(TileName.c_str());
	//
	//	CTransform* pTransform = new CTransform;
	//	CMeshRender* pMeshRender = new CMeshRender;
	//	CCollider2D* pCollider2D = new CCollider2D;
	//
	//	pTransform->SetLocalPos(Vec3(0.f, 0.f, 400.f));
	//	pTransform->SetLocalScale(Vec3(32.f, 32.f, 1.f));
	//	pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//
	//	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Transparent.png"));
	//
	//	pTile->AddComponent(pTransform);
	//	pTile->AddComponent(pMeshRender);
	//	pTile->AddComponent(pCollider2D);
	//
	//	pCurScene->AddObject(L"Tile", pTile);
	//}
	//return;

	//블로킹만들기
	for (int i = 0; i < 2; ++i)
	{
		CGameObj* pTileBlock = new CGameObj;
		pTileBlock->SetName(L"TileBlocking");

		CTransform* pTransform = new CTransform;
		CMeshRender* pMeshRender = new CMeshRender;
		CCollider2D* pCollider2D = new CCollider2D;

		pTransform->SetLocalPos(Vec3(0.f, 0.f, 400.f));
		pTransform->SetLocalScale(Vec3(32.f, 32.f, 1.f));
		pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));

		pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Block.png"));

		pTileBlock->AddComponent(pTransform);
		pTileBlock->AddComponent(pMeshRender);
		pTileBlock->AddComponent(pCollider2D);

		pCurScene->AddObject(L"Tile", pTileBlock);
	}
	//
	////언블록킹 만들기
	//for (int i = 0; i < 2; ++i)
	//{
	//	CGameObj* pTileUnBlock = new CGameObj;
	//
	//	pTileUnBlock->SetName(L"TileUnBlocking");
	//
	//	pTransform = new CTransform;
	//	pMeshRender = new CMeshRender;
	//	pCollider2D = new CCollider2D;
	//	CScript* pScript = new CBlockScript;
	//
	//	pTransform->SetLocalPos(Vec3(0.f, 0.f, 400.f));
	//	pTransform->SetLocalScale(Vec3(32.f, 32.f, 1.f));
	//	pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//	((CBlockScript*)(pScript))->SetBlocking(pTileBlock);
	//	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\UnBlock.png"));
	//
	//	pTileUnBlock->AddComponent(pTransform);
	//	pTileUnBlock->AddComponent(pMeshRender);
	//	pTileUnBlock->AddComponent(pCollider2D);
	//	pTileUnBlock->AddComponent(pScript);
	//	pCurScene->AddObject(L"Tile", pTileUnBlock);
	//}

	//pCurScene->AddLayer(L"Background", 7);
	//CGameObj* pBlackObj = new CGameObj;
	//pBlackObj->SetName(L"BlackImage2");
	//CTransform* pTrans = new CTransform;
	//CMeshRender* pMeshRender = new CMeshRender;
	//pTrans->SetLocalPos(Vec3(-100.f, 0.f, 500.f));
	//pTrans->SetLocalScale(Vec3(15040.f, 15440.f, 1.f));
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Black.png"));
	//pBlackObj->AddComponent(pTrans);
	//pBlackObj->AddComponent(pMeshRender);
	//pCurScene->AddObject(L"Background", pBlackObj);
	//
	//CGameObj* pFadeObj = pBlackObj->Clone();
	//int iFadeOff = 6;
	//pFadeObj->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &iFadeOff);
	//pFadeObj->AddComponent(new CBlackoutScript);
	//pCurScene->AddObject(L"Default", pFadeObj);
	//return;
	//
	CGameObj* pDoorObj = new CGameObj;
	pDoorObj->SetName(L"Doorleft");
	CMeshRender* pMeshRender = new CMeshRender;
	CTransform* pTrans = new CTransform;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Door.png"));
	pTrans->SetLocalPos(Vec3(270.f, -96.f, 400.f));
	pTrans->SetLocalScale(Vec3(96.f, 96.f, 1.f));
	pTrans->SetLocalRot(Vec3(0.f, XM_PI, 0.f));
	pDoorObj->AddComponent(pTrans);
	pDoorObj->AddComponent(pMeshRender);
	pCurScene->AddObject(L"Default", pDoorObj);
	
	CGameObj* pDoorObj2 = new CGameObj;
	pDoorObj2->SetName(L"DoorScriptleft");
	pMeshRender = new CMeshRender;
	pTrans = new CTransform;
	CDoorScript* pDoorScript = new CDoorScript;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Door.png"));
	pTrans->SetLocalPos(Vec3(172.f, -96.f, 400.f));
	pTrans->SetLocalScale(Vec3(96.f, 96.f, 1.f));
	pDoorScript->SetMoveScenePoint(Vec2(-957.f, 0.f), L"goUppertoBoss_1.scene");
	pDoorObj2->AddComponent(pTrans);
	pDoorObj2->AddComponent(pMeshRender);
	pDoorObj2->AddComponent(new CCollider2D);
	pDoorObj2->AddComponent(pDoorScript);
	pCurScene->AddObject(L"Default", pDoorObj2);

	//return;
	
	pDoorObj = new CGameObj;
	pDoorObj->SetName(L"DoorRight");
	pMeshRender = new CMeshRender;
	pTrans = new CTransform;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Door.png"));
	pTrans->SetLocalPos(Vec3(270.f, -96.f, 400.f));
	pTrans->SetLocalScale(Vec3(96.f, 96.f, 1.f));
	pTrans->SetLocalRot(Vec3(0.f, XM_PI, 0.f));
	pDoorObj->AddComponent(pTrans);
	pDoorObj->AddComponent(pMeshRender);
	pCurScene->AddObject(L"Default", pDoorObj);
	
	pDoorObj2 = new CGameObj;
	pDoorObj2->SetName(L"DoorScriptRight");
	pMeshRender = new CMeshRender;
	pTrans = new CTransform;
	pDoorScript = new CDoorScript;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Door.png"));
	pTrans->SetLocalPos(Vec3(172.f, -96.f, 400.f));
	pTrans->SetLocalScale(Vec3(96.f, 96.f, 1.f));
	pDoorScript->SetMoveScenePoint(Vec2(955.f, 0.f), L"boss_1.scene");
	pDoorObj2->AddComponent(pTrans);
	pDoorObj2->AddComponent(pMeshRender);
	pDoorObj2->AddComponent(new CCollider2D);
	pDoorObj2->AddComponent(pDoorScript);
	pCurScene->AddObject(L"Default", pDoorObj2);
	//
	//pDoorObj = new CGameObj;
	//pDoorObj->SetName(L"DoorRightUp");
	//pMeshRender = new CMeshRender;
	//pTrans = new CTransform;
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Door.png"));
	//pTrans->SetLocalPos(Vec3(270.f, -96.f, 400.f));
	//pTrans->SetLocalScale(Vec3(96.f, 96.f, 1.f));
	//pTrans->SetLocalRot(Vec3(0.f, XM_PI, 0.f));
	//pDoorObj->AddComponent(pTrans);
	//pDoorObj->AddComponent(pMeshRender);
	//pCurScene->AddObject(L"Default", pDoorObj);
	//
	//pDoorObj2 = new CGameObj;
	//pDoorObj2->SetName(L"DoorScriptRightUp");
	//pMeshRender = new CMeshRender;
	//pTrans = new CTransform;
	//pDoorScript = new CDoorScript;
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Door.png"));
	//pTrans->SetLocalPos(Vec3(172.f, -96.f, 400.f));
	//pTrans->SetLocalScale(Vec3(96.f, 96.f, 1.f));
	//pDoorScript->SetMoveScenePoint(Vec2(955.f, 0.f), L"getSecondWeapon_1.scene");
	//pDoorObj2->AddComponent(pTrans);
	//pDoorObj2->AddComponent(pMeshRender);
	//pDoorObj2->AddComponent(new CCollider2D);
	//pDoorObj2->AddComponent(pDoorScript);
	//pCurScene->AddObject(L"Default", pDoorObj2);
	//
	//return;
	
	//pCurScene->AddLayer(L"Default", 0);
	//pCurScene->AddLayer(L"Player", 1);
	//pCurScene->AddLayer(L"Bullet", 2);
	//pCurScene->AddLayer(L"Monster", 3);
	//pCurScene->AddLayer(L"Tile", 4);
	//pCurScene->AddLayer(L"UI", 5);
	//pCurScene->AddLayer(L"Background", 7);
	//
	//return;
	//
	//Camera Object 만들기
	//CGameObj* pCamObj = new CGameObj;
	//pCamObj->SetName(L"MainCam");
	//CScript* pScript = new CCameraScipt;
	//CTransform* pTransform = new CTransform;
	//CCamera* pCamera = new CCamera;
	//pTransform->SetLocalPos(Vec3(-103.f, 30.f, 0.f));
	//pTransform->SetLocalScale(Vec3(0.f, 0.f, 0.f));
	//pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pCamera->SetScale(0.85f);
	//Vec2 vBound[2] = { Vec2(-103.f, 30.f), Vec2(-103.f, 1378.f) };
	//((CCameraScipt*)pScript)->SetBoundVec(vBound);
	//((CCameraScipt*)pScript)->SetChasing();
	//pCamObj->AddComponent(pTransform);
	//pCamObj->AddComponent(pCamera);
	//pCamObj->AddComponent(pScript);
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Player")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Bullet")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Monster")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Default")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Tile")->GetLayerIdx());
	//pCamObj->Camera()->CheckLayer(pCurScene->FindLayer(L"Background")->GetLayerIdx());
	//pCurScene->AddObject(L"Default", pCamObj);
	//return;
	//Player Object 만들기
	//CGameObj* pParent = new CGameObj;
	//pParent->SetName(L"Player");
	//
	//CTransform* pTransform = new CTransform;
	//pMeshRender = new CMeshRender;
	//CCollider2D* pCollider2D = new CCollider2D;
	//CAnimator2D* pAnimator2D = new CAnimator2D;
	//
	//pTransform->SetLocalPos(Vec3(-161.f, -108.f, 500.f));
	//pTransform->SetLocalScale(Vec3(64.f, 64.f, 1.f));
	//pTransform->SetLocalRot(Vec3(0.f,XM_PI, 0.f));
	//
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pMeshRender->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\00.png"));
	//
	//pCollider2D->SetOffset(Vec3(-8.f, -7.f, 0.f));
	//pCollider2D->SetScale(Vec3(0.7f, 0.9f, 0.f));
	//
	//// Animation 로드
	//CResPtr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Trace\\TraceUnarmed.png");
	//
	//pAnimator2D->AddAnimation(L"UnArmPlayer_Awake", pTex, Vec2(0, 0), Vec2(40, 25), 4, 0.2f);
	//pAnimator2D->AddAnimation(L"UnArmPlayer_Idle", pTex, Vec2(0, 25), Vec2(40, 35), 1, 0.1f);
	//pAnimator2D->AddAnimation(L"UnArmPlayer_Walk", pTex, Vec2(0, 70), Vec2(40, 35), 8, 0.1f);
	//pAnimator2D->AddAnimation(L"UnArmPlayer_Jump", pTex, Vec2(0, 105), Vec2(40, 40), 6, 0.1f);
	//pAnimator2D->AddAnimation(L"UnArmPlayer_Landing", pTex, Vec2(0, 145), Vec2(40, 35), 2, 0.1f);
	//pAnimator2D->AddAnimation(L"UnArmPlayer_Crouch", pTex, Vec2(0, 200), Vec2(40, 25), 1, 0.1f);
	//pAnimator2D->PlayAnimation(L"UnArmPlayer_Walk", false);
	//
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Trace\\Trace.png");
	//pAnimator2D->AddAnimation(L"Player_Awake", pTex, Vec2(0, 0), Vec2(40, 35), 3, 0.2f);
	//pAnimator2D->AddAnimation(L"Player_Idle", pTex, Vec2(0, 40), Vec2(40, 35), 4, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Walk", pTex, Vec2(0, 80), Vec2(40, 35), 8, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Dead", pTex, Vec2(0, 120), Vec2(40, 35), 5, 0.2f);
	//pAnimator2D->AddAnimation(L"Player_Crouch", pTex, Vec2(0, 160), Vec2(40, 35), 1, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Jump", pTex, Vec2(0, 200), Vec2(40, 35), 4, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Landing", pTex, Vec2(0, 240), Vec2(40, 35), 2, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Upaim_jump", pTex, Vec2(0, 275), Vec2(40, 40), 4, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Upaim_land", pTex, Vec2(0, 320), Vec2(40, 35), 2, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Upaim_walk", pTex, Vec2(0, 360), Vec2(40, 35), 8, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Downaim_jump", pTex, Vec2(0, 440), Vec2(40, 35), 4, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Downaim_land", pTex, Vec2(0, 480), Vec2(40, 35), 2, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Downaim_walk", pTex, Vec2(0, 360), Vec2(40, 35), 8, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Up_idle", pTex, Vec2(0, 560), Vec2(40, 45), 4, 0.1f);
	//pAnimator2D->AddAnimation(L"Player_Up_jump", pTex, Vec2(0, 655), Vec2(40, 50), 5, 0.1f);
	//pAnimator2D->SetAnimPlayFirst(true);
	//pParent->AddComponent(pTransform);
	//pParent->AddComponent(pMeshRender);
	//pParent->AddComponent(pCollider2D);
	//pParent->AddComponent(pAnimator2D);
	//pParent->AddComponent(new CHilightScript);
	//CScript* pScript = new CTraceScript;
	//((CTraceScript*)pScript)->Armed();
	//CScene* pInfoScene = CSceneMgr::GetInst()->GetInfoScene();
	//CLayer** pLayer = pInfoScene->GetAllLayer();
	//vector<CGameObj*> vecInfoObj = pLayer[0]->GetParentObject();
	//CScript* pScriptInfo = vecInfoObj[0]->GetScript<CInfoScript>();
	//((CTraceScript*)pScript)->SetInfoScript(pScriptInfo);
	//pParent->AddComponent(pScript);
	//pCurScene->AddObject(L"Player", pParent);
	////pAnimator2D->AddAnimation(L"Explosion", L"Texture\\Animation\\Explosion\\", 0.03f);

	//////타일만들기
	//CGameObj* pTile = new CGameObj;
	//pTile->SetName(L"Tile");
	//
	//pTransform = new CTransform;
	//pMeshRender = new CMeshRender;
	//pCollider2D = new CCollider2D;
	//
	//pTransform->SetLocalPos(Vec3(0.f, -150.f, 500.f));
	//pTransform->SetLocalScale(Vec3(1500.f, 32.f, 1.f));
	//pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TextureMaterial"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Ghost1.png"));
	//
	//pTile->AddComponent(pTransform);
	//pTile->AddComponent(pMeshRender);
	//pTile->AddComponent(pCollider2D);
	//
	//pCurScene->AddObject(L"Tile", pTile);
	//
	//총알만들기 
	//CGameObj* pBullet = new CGameObj;
	//pBullet->SetName(L"Bullet");
	//
	//pTransform = new CTransform;
	//pMeshRender = new CMeshRender;
	//pAnimator2D = new CAnimator2D;
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\bullet1.PNG");
	//pAnimator2D->AddAnimation(L"Gun_Bullet1", pTex, Vec2(0, 0), Vec2(44, 20), 1, 0.1f);
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\bullet1.PNG"));
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\FireLeft.png");
	//pAnimator2D->AddAnimation(L"Gun_FireLeft", pTex, Vec2(0, 0), Vec2(50, 53), 5, 0.1f);
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\FireRight.png");
	//pAnimator2D->AddAnimation(L"Gun_FireRight", pTex, Vec2(0, 0), Vec2(50, 53), 5, 0.1f);
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\FireMid.png");
	//pAnimator2D->AddAnimation(L"Gun_FireMid", pTex, Vec2(0, 0), Vec2(50, 53), 5, 0.1f);
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Weapon\\Bullet2.PNG");
	//pAnimator2D->AddAnimation(L"Gun_Bullet2", pTex, Vec2(0, 0), Vec2(28, 28), 1, 0.1f);
	//pAnimator2D->PlayAnimation(L"Gun_Bullet1", false);
	//
	//pBullet->AddComponent(pTransform);
	//pBullet->AddComponent(pMeshRender);
	//pBullet->AddComponent(pAnimator2D);
	//pBullet->SetActive(false);
	//pCurScene->AddObject(L"Bullet", pBullet);
	//
	////보스만들기
	//
	//CGameObj* pBoss = new CGameObj;
	//pBoss->SetName(L"Boss");
	//
	//pTransform = new CTransform;
	//pTransform->SetLocalPos(Vec3(-250.f, -40.f, 500.f));
	//pTransform->SetLocalScale(Vec3(100.f, 120.f, 1.f));
	//pMeshRender = new CMeshRender;
	//pCollider2D = new CCollider2D;
	//CAnimator2D* pAnim = new CAnimator2D;
	//CScript* pMonsterScript = new CMonsterScript;
	//pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\00.png"));
	//pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Animation\\Boss2.png");
	//pAnim->AddAnimation(L"Boss_Rand", pTex, Vec2(0, 0), Vec2(100.f, 120.f), 4, 0.2f);
	//pAnim->AddAnimation(L"Boss_Des", pTex, Vec2(0, 120), Vec2(100.f, 120.f), 2, 0.2f);
	//pAnim->PlayAnimation(L"Boss_Rand", true);
	//((CMonsterScript*)pMonsterScript)->SetHp(50.f);
	//pScript = new CBossScript;
	//pBoss->AddComponent(pTransform);
	//pBoss->AddComponent(pMeshRender);
	//pBoss->AddComponent(pCollider2D);
	//pBoss->AddComponent(pAnim);
	//pBoss->AddComponent(pScript);
	//pBoss->AddComponent(pMonsterScript);
	//
	//pCurScene->AddObject(L"Monster", pBoss);

	return;
}

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CAboutDlg::OnBnClickedPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAboutDlg::OnBnClickedFrmPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAboutDlg::OnBnClickedCollisionCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAboutDlg::OnCreateEmptyobject()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
