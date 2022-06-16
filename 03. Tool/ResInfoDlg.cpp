#include "stdafx.h"
#include "ResInfoDlg.h"


CResInfoDlg::CResInfoDlg(UINT _iResID, CWnd * pParent)
	: CDialogEx(_iResID, pParent)
	, m_pResource(nullptr)
{
}



CResInfoDlg::~CResInfoDlg()
{
}

void CResInfoDlg::OnOK(){}

void CResInfoDlg::OnCancel(){}


void CResInfoDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}
