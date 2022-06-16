#pragma once


class CGameObj;
class CTexture;
class CMainFrame;
class CLayer;

#include <ResPtr.h>

// CGameView 보기

class CGameView : public CView
{
	DECLARE_DYNCREATE(CGameView)

private:
	CMainFrame* m_pframe;
	CPoint m_pMousePos;
	CGameObj* m_pTargetTileObj;
	CGameObj* m_pTileUIObj;
	CGameObj* m_pMapCamObj;
	CGameObj* m_pMapTileObj;
	Vec2 m_vTileLT;
	Vec2 m_vTileSize;
	Vec2 m_vTileParam[900];
	Vec2 m_vTileTexParam[900];
	int m_iTexIdx[900];
	int m_iSetTexNum;
	CResPtr<CTexture> m_pTex;
	vector<CGameObj*> m_vCollObj;
	bool m_bSetCollider;

public:
	void init();
	void update();
	void SetTargetTileObj(CGameObj* _pTarget) { m_pTargetTileObj = _pTarget; }
	void SetMapCamObj(CGameObj* _pTarget) { m_pMapCamObj = _pTarget; }
	void SetMapTileObj(CGameObj* _pTarget) { m_pMapTileObj = _pTarget; }
	void SetTileUI(CGameObj* _pTarget) { m_pTileUIObj = _pTarget; }
	void SetTileTex(CResPtr<CTexture> _pTex) { m_pTex = _pTex; }
	CGameObj* GetTargetTileObj() { return m_pTargetTileObj; }

protected:
	CGameView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGameView();

private:
	void ChangeTileUI();
	void ChangeGameObjTarget(CLayer** ppAllLayer);
	void ChangeTileTarget(CLayer** ppAllLayer, Vec3 vCamPos);
	void ChangeBigTileTarget(CLayer** ppAllLayer);
	void DrawTile(CLayer** ppAllLayer, Vec3 vCamPos, Vec2 vMapSize);
	void DrawCOllTile(CLayer**ppAllLayer, Vec3 vCamPos, Vec2 vMapSize);

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
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSaveMap();
};


