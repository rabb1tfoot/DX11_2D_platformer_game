#pragma once


// CAddAnimDlg 대화 상자

class CGameObj;

class CAddAnimDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAnimDlg)

private:
	CGameObj* m_pTarget;

public:
	CAddAnimDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddAnimDlg();

public:
	void SetTarget(CGameObj* _target) { m_pTarget = _target; }

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDCAddAnimDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editAnimKey;
	CEdit m_editTerm;
	CEdit m_editFolderPath;
	afx_msg void OnBnClickedSetFolder();
	virtual void OnOK();
	CListCtrl m_ctrlAnimList;

	virtual BOOL OnInitDialog();

	afx_msg void OnLcKeydown(NMHDR *pNMHDR, LRESULT *pResult);
};
