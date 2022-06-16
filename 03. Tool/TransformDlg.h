#pragma once

#include "ComponentDlg.h"

class CGameObj;

// CTransformDlg 대화 상자

class CTransformDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTransformDlg)


public:
	virtual void init(CGameObj* _pTarget);

	virtual void update(CGameObj* _pTarget);

public:
	CTransformDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTransformDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_edit[9];
	afx_msg void OnEnChangeEdit(UINT _id);
	afx_msg void OnBnClickedActive();
	BOOL m_bActive;
	CComboBox m_cbLayer;
	afx_msg void OnCbnSelchangeComboLayer();
	afx_msg void OnCbnDropdownCombo();
};
