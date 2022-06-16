#pragma once


// CBtnView 보기

class CBtnView : public CFormView
{
	DECLARE_DYNCREATE(CBtnView)

private:
	bool m_bMapToolOn;
	Vec2 m_iMapSize;

protected:
	CBtnView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBtnView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void DoDataExchange(CDataExchange* pDX);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMap();
	afx_msg void OnBnClickedButtonMain();
	void SetMapToolOn(bool _bool) { m_bMapToolOn = _bool; }
	bool GetMapToolOn() { return m_bMapToolOn; }
	void SetMapSize(Vec2 _vSize) { m_iMapSize = _vSize; }
	Vec2 GetMapSize() { return m_iMapSize; }
	void init();

private:
	void CreateDefaultTileMap();
public:
	CButton m_btnPlay;
	CButton m_btnPause;
	CButton m_btnNextFrm;
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedCollisionCheck();
};


