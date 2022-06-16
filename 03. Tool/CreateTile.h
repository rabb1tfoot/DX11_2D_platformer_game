#pragma once


// CCreateTile 대화 상자

class CCreateTile : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateTile)

public:
	CCreateTile(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCreateTile();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateTile };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditX;
	CEdit m_EditY;
	afx_msg void OnBnClickedOk();
};
