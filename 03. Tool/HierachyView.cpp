// HierachyView.cpp: 구현 파일
//

#include "stdafx.h"
#include "HierachyView.h"

#include "GameObjectDlg.h"
#include "ResDlg.h"


// CHierachyView

IMPLEMENT_DYNCREATE(CHierachyView, CView)

CHierachyView::CHierachyView()
{

}

CHierachyView::~CHierachyView()
{
}

void CHierachyView::init()
{
	m_pResourceDlg->init();
}

void CHierachyView::init_object()
{
	m_pGameObjDlg->init();
}

BEGIN_MESSAGE_MAP(CHierachyView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CHierachyView 그리기

void CHierachyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CHierachyView 진단

#ifdef _DEBUG
void CHierachyView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierachyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHierachyView 메시지 처리기


int CHierachyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	UINT iHeight = 0;

	CRect rt;
	GetClientRect(rt);

	CRect rtDlg;

	m_pGameObjDlg = new CGameObjectDlg;
	m_pGameObjDlg->Create(IDD_CGameObjectDlg, this);
	m_pGameObjDlg->GetWindowRect(rtDlg);
	m_pGameObjDlg->SetWindowPos(nullptr, 0, iHeight, rt.Width(), rtDlg.Height(), 0);
	m_pGameObjDlg->ShowWindow(true);

	iHeight += rtDlg.Height();

	m_pResourceDlg = new CResDlg;
	m_pResourceDlg->Create(IDD_CResDlg, this);
	m_pResourceDlg->GetWindowRect(rtDlg);
	m_pResourceDlg->SetWindowPos(nullptr, 0, iHeight, rt.Width(),  rt.Height() - iHeight, 0);
	m_pResourceDlg->ShowWindow(true);

	return 0;
}
