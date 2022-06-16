#pragma once

#include "ComponentDlg.h"

// CAddComDlg 대화 상자

class CAddComDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAddComDlg)

private:
	COMPONENT_TYPE m_Comtype0;
	COMPONENT_TYPE m_Comtype1;
	COMPONENT_TYPE m_Comtype2;
	COMPONENT_TYPE m_Comtype3;

public:
	virtual void init(CGameObj* _pTarget);
	virtual void update(CGameObj* _Target);
	CAddComDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddComDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAddComDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbSelect;
	afx_msg void OnBnClickedAddCom();
};
