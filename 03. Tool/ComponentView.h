#pragma once


// CComponentView 보기


enum class DLG_TYPE
{
	TRANSFORM = 0,
	MESHRENDER,
	CAMERA,
	COLLIDER2D,
	//COLLIDER3D,
	ANIMATOR2D = 5,
	//ANIMATOR3D,
	SCRIPT = 7,
	END = 8,
};


class CGameObj;
class CComponentDlg;
class CResInfoDlg;
class CReSource;

class CComponentView : public CView
{
	DECLARE_DYNCREATE(CComponentView)

protected:
	CComponentView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CComponentView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	CGameObj*			m_pTarget;
	CComponentDlg*		m_arrComDlg[(UINT)DLG_TYPE::END];	
	CComponentDlg*		m_AddCom;

	CReSource*			m_pResource;
	CResInfoDlg*		m_arrResInfoDlg[(UINT)RES_TYPE::END];
	CResInfoDlg*		m_pCurResInfoDlg;

public:
	void init();
	void update();
	void SetTarget(CGameObj* _pTarget);
	CGameObj* GetTarget() { return m_pTarget; }
	void SetResource(CReSource* _pResource);

private:
	void SetDlgPos();
	void update_always();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


