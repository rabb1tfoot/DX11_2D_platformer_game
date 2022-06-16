#pragma once


// CGameObjectDlg 대화 상자

class CGameObj;

class CGameObjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameObjectDlg)
private:
	HTREEITEM m_hDragItem;
	HTREEITEM m_hTargetItem;

	bool m_bInit;

public:
	void init();
	void AddGameObject(CGameObj* _pObj, HTREEITEM _hParent);
private:
	void MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem);
	void GetChildItem(HTREEITEM _hItem, vector<HTREEITEM>& _vecChild);

private:
	HTREEITEM AddItem(CString _str, HTREEITEM _hParent, DWORD_PTR _dwData);

public:
	CGameObjectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGameObjectDlg();
	virtual void OnOK();
	virtual void OnCancel();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGameObjectDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_ctrlTree;
	afx_msg void OnTvnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydownDel(NMHDR *pNMHDR, LRESULT *pResult);
};
