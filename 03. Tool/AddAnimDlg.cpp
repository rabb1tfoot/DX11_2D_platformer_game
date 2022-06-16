// AddAnimDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AddAnimDlg.h"
#include "afxdialogex.h"

#include <Animator2D.h>
#include <Animation2D.h>
#include <GameObj.h>
#include <PathMgr.h>

// CAddAnimDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddAnimDlg, CDialogEx)

CAddAnimDlg::CAddAnimDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDDCAddAnimDlg, pParent)
{

}

CAddAnimDlg::~CAddAnimDlg()
{
}

void CAddAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editAnimKey);
	DDX_Control(pDX, IDC_EDIT2, m_editTerm);
	DDX_Control(pDX, IDC_EDIT3, m_editFolderPath);
	DDX_Control(pDX, IDC_LIST2, m_ctrlAnimList);
}


BEGIN_MESSAGE_MAP(CAddAnimDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddAnimDlg::OnBnClickedSetFolder)

	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST2, &CAddAnimDlg::OnLcKeydown)
END_MESSAGE_MAP()


// CAddAnimDlg 메시지 처리기


void CAddAnimDlg::OnBnClickedSetFolder()
{
	const wchar_t* pInitPath = CPathMgr::GetResPath();

	CFolderPickerDialog Picker(pInitPath, OFN_FILEMUSTEXIST);

	CString path;
	if (IDOK == Picker.DoModal())
	{
		path = Picker.GetPathName();
		path = CPathMgr::GetRelativePath(path);
		m_editFolderPath.SetWindowTextW(path);
	}
}

void CAddAnimDlg::OnOK()
{
	CString strKey;
	m_editAnimKey.GetWindowTextW(strKey);

	CString strPath;
	m_editFolderPath.GetWindowTextW(strPath);

	CString strTerm;
	m_editTerm.GetWindowTextW(strTerm);
	float fTerm = (float)_wtof(strTerm);

	m_pTarget->Animator2D()->AddAnimation(strKey.GetBuffer(), strPath.GetBuffer(), fTerm);

	// 리스트 컨트롤 갱신
	m_ctrlAnimList.DeleteAllItems();

	const map<wstring, CAnimation2D*>& mapAnim = m_pTarget->Animator2D()->GetAnimList();

	int i = 0;
	for (const auto& pair : mapAnim)
	{
		m_ctrlAnimList.InsertItem(i, pair.first.c_str());
		++i;
	}
}

BOOL CAddAnimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const map<wstring, CAnimation2D*>& mapAnim = m_pTarget->Animator2D()->GetAnimList();

	int i = 0;
	for (const auto& pair : mapAnim)
	{
		m_ctrlAnimList.InsertItem(i, pair.first.c_str());
		++i;
	}

	return TRUE;
}

void CAddAnimDlg::OnLcKeydown(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		CString strKey;

		POSITION pos = m_ctrlAnimList.GetFirstSelectedItemPosition();

		int i = m_ctrlAnimList.GetNextSelectedItem(pos);

		strKey = m_ctrlAnimList.GetItemText(i, 0);

		m_ctrlAnimList.DeleteItem(i);

		m_pTarget->Animator2D()->DeleteAnimation(strKey.GetBuffer());
	}

	*pResult = 0;
}
