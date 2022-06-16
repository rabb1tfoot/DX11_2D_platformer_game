// MapDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapDlg.h"
#include "afxdialogex.h"


// CMapDlg 대화 상자

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MapDlg, pParent)
{

}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
END_MESSAGE_MAP()


// CMapDlg 메시지 처리기
