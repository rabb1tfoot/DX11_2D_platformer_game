#pragma once

class CGameObj;

// CCollisionCheck 대화 상자

class CCollisionCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CCollisionCheck)

public:
	CCollisionCheck(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCollisionCheck();

private:
	int m_iListIdx;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCollisionCheck };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbCheck1;
	CComboBox m_cbCheck2;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDel();
	CListCtrl m_ctrlList;
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void AddComboData();
	void AddListData();
};
