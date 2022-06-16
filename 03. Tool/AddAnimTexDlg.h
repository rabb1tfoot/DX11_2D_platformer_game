#pragma once


// CAddAnimTexDlg 대화 상자
#include <ResPtr.h>
#include <GameObj.h>
#include <Texture.h>


class CAddAnimTexDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAnimTexDlg)

private:
	CGameObj* m_pTarget;
	CResPtr<CTexture> m_pTex;

public:
	void SetTarget(CGameObj* _target) { m_pTarget = _target; }

public:
	CAddAnimTexDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddAnimTexDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddAnimTexDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlAnimList;
	CEdit m_editKey;
	CEdit m_editFrmSize;
	CEdit m_editTerm;
	CEdit m_edit[4];
	CEdit m_editTex;

	afx_msg void OnBnClickedSelectTex();
	afx_msg void OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
