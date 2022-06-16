#pragma once

#include "ComponentDlg.h"

class CGameObj;

// CMeshRenderDlg 대화 상자

class CMeshRenderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CMeshRenderDlg)

public:
	CMeshRenderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeshRenderDlg();

public:
	virtual void update(CGameObj* _pTarget);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHRENDERDLG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editMesh;
	CEdit m_editMtrl;
	afx_msg void OnBtnMesh();
	afx_msg void OnBtnMtrl();
};
