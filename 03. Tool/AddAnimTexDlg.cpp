// AddAnimTexDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AddAnimTexDlg.h"
#include "afxdialogex.h"

#include <GameObj.h>
#include <Animation2D.h>
#include <Animator2D.h>
#include <ResMgr.h>
#include <Texture.h>
#include <PathMgr.h>

// CAddAnimTexDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddAnimTexDlg, CDialogEx)

CAddAnimTexDlg::CAddAnimTexDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddAnimTexDlg, pParent)
{

}

CAddAnimTexDlg::~CAddAnimTexDlg()
{
}

void CAddAnimTexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlAnimList);
	DDX_Control(pDX, IDC_EDIT1, m_editKey);
	DDX_Control(pDX, IDC_EDIT7, m_editFrmSize);
	DDX_Control(pDX, IDC_EDIT8, m_editTerm);
	DDX_Control(pDX, IDC_EDIT2, m_editTex);

	for (UINT i = 0; i < 5; ++i)
	{
		DDX_Control(pDX, 2003+i, m_edit[i]);
	}
}


BEGIN_MESSAGE_MAP(CAddAnimTexDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddAnimTexDlg::OnBnClickedSelectTex)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CAddAnimTexDlg::OnLvnKeydownList)
END_MESSAGE_MAP()


// CAddAnimTexDlg 메시지 처리기


void CAddAnimTexDlg::OnBnClickedSelectTex()
{
	static wchar_t szFilter[] = L"이미지 파일(*.BMP, *.GIF, *.JPG, *PNG) | *.BMP;*.GIF;*.JPG;*.PNG;*.bmp;*.jpg;*.gif;*.png |모든파일(*.*)|*.*||";
	
	// True 지정한 파일까지의 경로
	// False 지정한 경로에서 내가 작성한 이름까지의 경로
	CFileDialog dlg(FALSE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);
	
	CString pathName;
	wstring wPathName;
	CString KeyName;
	if (IDOK == dlg.DoModal())
	{
		KeyName = dlg.GetFileName();
		pathName = dlg.GetPathName();

		pathName = CPathMgr::GetRelativePath(pathName);

		wPathName = pathName.GetBuffer();
		wPathName = wPathName.substr(0, wcslen(wPathName.c_str()) - 1);
	}

	m_editTex.SetWindowTextW(KeyName.GetBuffer());

	CResMgr::GetInst()->Load<CTexture>(KeyName.GetBuffer(), wPathName.c_str());

	m_pTex = CResMgr::GetInst()->FindRes<CTexture>(KeyName.GetBuffer());
}


void CAddAnimTexDlg::OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult)
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


BOOL CAddAnimTexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const map<wstring, CAnimation2D*>& mapAnim = m_pTarget->Animator2D()->GetAnimList();

	int i = 0;
	for (const auto& pair : mapAnim)
	{
		m_ctrlAnimList.InsertItem(i, pair.first.c_str());
		++i;
	}
	m_editKey.SetWindowTextW(L"Player_Down_Walk");
	m_edit[0].SetWindowTextW(L"0");
	m_edit[1].SetWindowTextW(L"260");
	m_edit[2].SetWindowTextW(L"60");
	m_edit[3].SetWindowTextW(L"65");
	m_editFrmSize.SetWindowTextW(L"10");
	m_editTerm.SetWindowTextW(L"0.08");
	

	return TRUE;
				 
}

void CAddAnimTexDlg::OnOK()
{
	CString strEdit[4];
	int iEdit[4];
	for (UINT i = 0; i < 4; ++i)
	{
		m_edit[i].GetWindowTextW(strEdit[i]);
		iEdit[i] = _ttoi(strEdit[i]);
	}

	CString strFrmSize;
	int iFrmSize;
	m_editFrmSize.GetWindowTextW(strFrmSize);
	iFrmSize = _ttoi(strFrmSize);

	CString strTerm;
	float iTerm;
	m_editTerm.GetWindowTextW(strTerm);
	iTerm = (float)_ttof(strTerm);

	CString strKey;
	m_editKey.GetWindowTextW(strKey);

	m_pTarget->Animator2D()->AddAnimation(strKey.GetBuffer(), m_pTex,
		Vec2(iEdit[0], iEdit[1]), Vec2(iEdit[2], iEdit[3]), iFrmSize, iTerm);

	//목록 갱신

	m_ctrlAnimList.DeleteAllItems();
	const map<wstring, CAnimation2D*>& mapAnim = m_pTarget->Animator2D()->GetAnimList();

	int i = 0;
	for (const auto& pair : mapAnim)
	{
		m_ctrlAnimList.InsertItem(i, pair.first.c_str());
		++i;
	}

}
