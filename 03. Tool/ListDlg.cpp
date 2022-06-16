// ListDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ListDlg.h"
#include "afxdialogex.h"


// CListDlg 대화 상자

IMPLEMENT_DYNAMIC(CListDlg, CDialogEx)

CListDlg::CListDlg(CString _strCaption
	, vector<CString>& _vecFileName
	, vector<CString>& _vecResName
	, CWnd * pParent)
	: CDialogEx(IDD_LISTDLG, pParent)
	, m_strCaption(_strCaption)
	, m_vecFileName(_vecFileName)
	, m_vecResName(_vecResName)
{
}

CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrl);
}


BEGIN_MESSAGE_MAP(CListDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CListDlg::OnDbClicked)
END_MESSAGE_MAP()


// CListDlg 메시지 처리기


BOOL CListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW(m_strCaption);

	// 리스트 컨트롤에 목록 채우기
	for (UINT i = 0; i < m_vecFileName.size(); ++i)
	{
		m_ctrl.InsertItem(i, m_vecFileName[i]);
	}

	return TRUE;
}

void CListDlg::OnDbClicked(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (-1 == pNMItemActivate->iItem)
		return;

	m_strSelect = m_ctrl.GetItemText(pNMItemActivate->iItem, 0);
	m_strKey = m_vecResName[pNMItemActivate->iItem];

	*pResult = 0;

	EndDialog(IDOK);
}

void CListDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ctrl.GetFirstSelectedItemPosition();

	POSITION pos = m_ctrl.GetFirstSelectedItemPosition();

	int i = m_ctrl.GetNextSelectedItem(pos);

	m_strSelect = m_ctrl.GetItemText(i, 0);
	m_strKey = m_vecResName[i];

	EndDialog(IDOK);
}
