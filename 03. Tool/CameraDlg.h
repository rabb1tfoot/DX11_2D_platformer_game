#pragma once

#include "ComponentDlg.h"

// CCameraDlg 대화 상자

class CCameraDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	virtual void update(CGameObj* _pTarget);

public:
	CCameraDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERADLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComboDC;
	CEdit m_editFar;
	CEdit m_editScale;
	CEdit m_editFOV;
	afx_msg void OnEnChangeFar();
	afx_msg void OnEnChangeScale();
	afx_msg void OnEnChangeFOV();
	afx_msg void OnCbnSelchangeCombo();
};
