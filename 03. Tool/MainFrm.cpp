
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "ToolApp.h"

#include "MainFrm.h"
#include "GameView.h"
#include "HierachyView.h"
#include "ComponentView.h"
#include "BtnView.h"
#include "DebugView.h"
#include "ResDlg.h"
#include "GameObjectDlg.h"

#include <Core.h>
#include <ResPtr.h>
#include <Material.h>
#include <ResMgr.h>
#include <SceneMgr.h>
#include <Scene.h>
#include <Layer.h>
#include <GameObj.h>
#include <Transform.h>
#include <MeshRender.h>
#include <Mesh.h>
#include <ColliderMgr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_RESOURCE_MATERIAL, &CMainFrame::OnResourceMaterial)
	ON_COMMAND(ID_32771, &CMainFrame::OnSaveScene)
	ON_COMMAND(ID_32772, &CMainFrame::OnLoadScene)
	ON_COMMAND(ID_GAMEOBJECT_EMPTYOBJECT, &CMainFrame::OnCreateEmptyobject)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rt = {};
	GetWindowRect(&rt);
	SetWindowPos(nullptr, 50, 25, rt.right - rt.left, rt.bottom - rt.top, 0);

	//작업영역 분할
	m_mainSplitter.SetBorder(POINT{ 0,0 }, POINT{ 1, 1 });
	m_mainSplitter.CreateStatic(this, 1, 3);

	m_mainSplitter.SetColumnInfo(0, GAMEVIEW_X, GAMEVIEW_X);
	m_mainSplitter.SetColumnInfo(1, HIERACHYVIEW_X, HIERACHYVIEW_X);

	m_mainSplitter.CreateView(0, 1, RUNTIME_CLASS(CHierachyView), CSize(HIERACHYVIEW_X, FRAME_Y), nullptr);
	m_mainSplitter.CreateView(0, 2, RUNTIME_CLASS(CComponentView), CSize(COMVIEW_X, FRAME_Y), nullptr);
	
	m_subSplitter.CreateStatic(&m_mainSplitter, 3, 1, WS_CHILD | WS_VISIBLE, m_mainSplitter.IdFromRowCol(0, 0));
	m_subSplitter.SetBorder(POINT{ 0, 0 }, POINT{ 1,1 });
	m_subSplitter.CreateView(0, 0, RUNTIME_CLASS(CBtnView), CSize(BTNVIEW_X, BTNVIEW_Y), nullptr);
	m_subSplitter.CreateView(1, 0, RUNTIME_CLASS(CGameView), CSize(GAMEVIEW_X, GAMEVIEW_Y), nullptr);
	m_subSplitter.CreateView(2, 0, RUNTIME_CLASS(CDebugView), CSize(DBGVIEW_X, DBGVIEW_Y), nullptr);

	return 0;
}

void CMainFrame::init()
{
	SetType(TOOL_TYPE::TOOL_MAIN);

	((CComponentView*)GetComView())->init();
	((CHierachyView*)GetHierachyView())->init();
}

void CMainFrame::update()
{
	// CompnentView update
	((CComponentView*)GetComView())->update();

	//GameView update
	((CGameView*)GetGameView())->update();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	RECT rt = { 0, 0, FRAME_X, FRAME_Y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);

	cs.cx = rt.right - rt.left;
	cs.cy = rt.bottom - rt.top;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnResourceMaterial()
{
	CResPtr<CMaterial> pMtrl = new CMaterial(RES_TYPE::MATERIAL);

	CString strKey = L"Material\\New Material.mtrl";

	CResPtr<CMaterial> pFindMtrl = nullptr;
	int iNum = 0;

	//pFindMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strKey.GetBuffer());
	//
	//if (nullptr != pFindMtrl)
	//{
	//	++iNum;
	//	strKey.Format(L"Material\\New Material %d.mtrl", iNum);
	//}

	while (true)
	{
		pFindMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strKey.GetBuffer());

		if (nullptr != pFindMtrl)
		{
			++iNum;
			strKey.Format(L"Material\\New Material %d.mtrl", iNum);
		}
		else
		{
			break;
		}
	}

	CResMgr::GetInst()->AddRes(strKey.GetBuffer(), pMtrl);

	CHierachyView* pView = (CHierachyView*)GetHierachyView();
	pView->GetResDlg()->Renew();

	pMtrl->Save();

}

#include<SaveLoadMgr.h>

void CMainFrame::OnSaveScene()
{
	static wchar_t sz_Filter[] = L"Scene 파일(*.scene)|*.scene|모든파일(*.*)|*.*||";

	CFileDialog dlg(FALSE, nullptr, nullptr, OFN_HIDEREADONLY, sz_Filter);

	CString strInitPath = CPathMgr::GetResPath();
	strInitPath += L"Scene\\";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	CString pathName;
	if (IDOK == dlg.DoModal())
	{
		pathName = dlg.GetPathName();
	}
	CSaveLoadMgr::SaveScene(pathName.GetBuffer());
}


void CMainFrame::OnLoadScene()
{
	static wchar_t sz_Filter[] = L"Scene 파일(*.scene)|*.scene|모든파일(*.*)|*.*||";

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY, sz_Filter);

	CString strInitPath = CPathMgr::GetResPath();
	strInitPath += L"Scene\\";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	CString pathName;
	if (IDOK == dlg.DoModal())
	{
		pathName = dlg.GetPathName();
	}
	CCore::GetInst()->SetState(SCENE_STATE::LOAD);

	CSaveLoadMgr::LoadScene(pathName.GetBuffer());

	// GameObject 트리컨트롤 목록 갱신
	CGameObjectDlg* ObjectDlg = ((CHierachyView*)GetHierachyView())->GetGameObjDlg();
	ObjectDlg->init();

	// ResDlg 트리컨트롤 리소스 목록 갱신
	CResDlg* pResDlg = ((CHierachyView*)GetHierachyView())->GetResDlg();
	pResDlg->Renew();

	CColliderMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Player", L"Tile");
	CColliderMgr::GetInst()->CollisionCheck(L"Default", L"Player");
	CColliderMgr::GetInst()->CollisionCheck(L"Default", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Bullet", L"Tile");
	CColliderMgr::GetInst()->CollisionCheck(L"Bullet", L"Monster");
	CColliderMgr::GetInst()->CollisionCheck(L"Monster", L"Tile");

	CCore::GetInst()->SetState(SCENE_STATE::STOP);
}


void CMainFrame::OnCreateEmptyobject()
{
	//빈오브젝트 만들기
	CGameObj* pObj = new CGameObj;
	static UINT iNumName = 0;
	CString cName = L"";
	cName.Format(L"Empty Object %d", iNumName);
	++iNumName;
	wstring wName = cName.GetBuffer();
	pObj->SetName(wName);

	CTransform* pTransform = new CTransform;
	pTransform->SetLocalPos(Vec3(0.f, 0.f, 500.f));
	pTransform->SetLocalScale(Vec3(100.f, 100.f, 1.f));
	pTransform->SetLocalRot(Vec3(0.f, 0.f, 0.f));

	pObj->AddComponent(pTransform);
	CSceneMgr::GetInst()->GetCurScene()->AddObject(L"Default", pObj);

	((CHierachyView*)GetHierachyView())->GetGameObjDlg()->AddGameObject(pObj, nullptr);
}
