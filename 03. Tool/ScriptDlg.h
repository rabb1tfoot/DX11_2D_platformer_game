#pragma once

#include "ComponentDlg.h"

// CScriptDlg 대화 상자

class CScriptDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	virtual void init(CGameObj* _pTarget);
	virtual void update(CGameObj* _pTarget);

private:
	bool m_bSelect;
	int m_iSelScriptList;

public:
	CScriptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScriptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CScriptDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlScript;
	CComboBox m_cbScript;
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusList2(NMHDR *pNMHDR, LRESULT *pResult);
};
