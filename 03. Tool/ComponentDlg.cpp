// ComponentDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ComponentDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"


// CComponentDlg 대화 상자

IMPLEMENT_DYNAMIC(CComponentDlg, CDialogEx)

CComponentDlg::CComponentDlg(UINT nIDTemplate, CWnd * pParent)
	: CDialogEx(nIDTemplate, pParent)
	, m_pTarget(nullptr)
	, m_bEditActive(false)
	, m_bCbActive(false)
{
}

CComponentDlg::~CComponentDlg()
{
}

void CComponentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CComponentDlg::OnEnSetfocusEdit(UINT _id)
{
	m_bEditActive = true;
}

void CComponentDlg::OnEnKillfocusEdit(UINT _id)
{
	m_bEditActive = false;
}
void CComponentDlg::OnCbSetfocusEdit(UINT _id)
{
	m_bCbActive = true;
}

void CComponentDlg::OnCbKillfocusEdit(UINT _id)
{
	m_bCbActive = false;
}

BEGIN_MESSAGE_MAP(CComponentDlg, CDialogEx)
END_MESSAGE_MAP()

void CComponentDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}

void CComponentDlg::OnOk()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	::SetFocus(pMainFrm->GetGameView()->m_hWnd);
}



// CComponentDlg 메시지 처리기

