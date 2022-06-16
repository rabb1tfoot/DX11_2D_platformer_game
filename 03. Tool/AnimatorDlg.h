#pragma once


#include "ComponentDlg.h"

class CAnimatorDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimatorDlg)

public:

	virtual void init(CGameObj* _Target);
	virtual void update(CGameObj* _Target);

	void Animator2DStateCheck(CGameObj* _pTarget);

	CAnimatorDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimatorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AnimatorDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
	CComboBox m_cbAnimList;
	CButton m_btnPlay;
	BOOL	m_bRepeat;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedAddAnim();
	afx_msg void OnBnClickedRepeat();
	afx_msg void OnCbnSelchangeAnim();
	afx_msg void OnBnClickedAddAnimTexture();
};
