// CreateTile.cpp: 구현 파일
//

#include "stdafx.h"
#include "CreateTile.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "BtnView.h"


// CCreateTile 대화 상자

IMPLEMENT_DYNAMIC(CCreateTile, CDialogEx)

CCreateTile::CCreateTile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateTile, pParent)
{

}

CCreateTile::~CCreateTile()
{
}

void CCreateTile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditX);
	DDX_Control(pDX, IDC_EDIT2, m_EditY);
}


BEGIN_MESSAGE_MAP(CCreateTile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCreateTile::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateTile 메시지 처리기


void CCreateTile::OnBnClickedOk()
{
	CString str;
	m_EditX.GetWindowTextW(str);
	float iMapX = _wtof(str);
	m_EditY.GetWindowTextW(str);
	float iMapY = _wtof(str);

	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	((CBtnView*)frame->GetBtnView())->SetMapSize(Vec2(iMapX, iMapY));

	CDialogEx::OnOK();
}
