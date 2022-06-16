#pragma once


// CComponentDlg 대화 상자

class CGameObj;

class CComponentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CComponentDlg)

public:
	CComponentDlg(UINT nIDTemplate, CWnd* pParent = nullptr);
	virtual ~CComponentDlg();

private:
	CGameObj*	m_pTarget;
	bool		m_bEditActive;
	bool		m_bCbActive;

public:
	virtual void init(CGameObj* _pTarget) {};
	virtual void update(CGameObj* _pTarget) = 0;

public:
	CGameObj*	GetTarget() { return m_pTarget; }
	void SetTarget(CGameObj* _pTarget) { m_pTarget = _pTarget; }

	bool IsEditActive() { return m_bEditActive; }
	bool IsCbActive() { return m_bCbActive; }


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CComponentDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg void OnEnSetfocusEdit(UINT _id);
	afx_msg void OnEnKillfocusEdit(UINT _id);

	afx_msg void OnCbSetfocusEdit(UINT _id);
	afx_msg void OnCbKillfocusEdit(UINT _id);

	DECLARE_MESSAGE_MAP()

	virtual void PostNcDestroy();
	virtual void OnOk();
};
