#pragma once

#include "ComponentDlg.h"

// CCollider2DDlg 대화 상자

class CCollider2DDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCollider2DDlg)

public:
	virtual void update(CGameObj* _Target);
	CCollider2DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCollider2DDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCollider2DDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit[6];
	CComboBox m_cbType;

	afx_msg void OnEnChangeEdit(UINT _id);
	afx_msg void OnCbnSelchangeCombo();

	virtual BOOL OnInitDialog();
};
