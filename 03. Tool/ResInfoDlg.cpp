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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CDialogEx::PostNcDestroy();
}
