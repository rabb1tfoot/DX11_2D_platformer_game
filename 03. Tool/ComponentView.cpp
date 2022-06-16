// ComponentView.cpp: 구현 파일
//

#include "stdafx.h"
#include "MainFrm.h"
#include "ComponentView.h"

#include <SceneMgr.h>
#include <GameObj.h>
#include <TimeMgr.h>

#include "TransformDlg.h"
#include "MeshRenderDlg.h"
#include "CameraDlg.h"
#include "Collider2DDlg.h"
#include "AnimatorDlg.h"
#include "ScriptDlg.h"
#include "ResInfoDlg.h"
#include "MaterialDlg.h"
#include "AddComDlg.h"

// CComponentView

IMPLEMENT_DYNCREATE(CComponentView, CView)

CComponentView::CComponentView()
	: m_pTarget(nullptr)
	, m_pCurResInfoDlg(nullptr)
	, m_arrComDlg{}
	, m_arrResInfoDlg{}
	, m_AddCom(nullptr)
{

}

CComponentView::~CComponentView()
{
}


int CComponentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Component Dialog 생성
	m_arrComDlg[(UINT)DLG_TYPE::TRANSFORM] = new CTransformDlg;
	m_arrComDlg[(UINT)DLG_TYPE::TRANSFORM]->Create(IDD_TRANSFORMDLG, this);
	m_arrComDlg[(UINT)DLG_TYPE::TRANSFORM]->ShowWindow(false);

	m_arrComDlg[(UINT)DLG_TYPE::MESHRENDER] = new CMeshRenderDlg;
	m_arrComDlg[(UINT)DLG_TYPE::MESHRENDER]->Create(IDD_MESHRENDERDLG, this);
	m_arrComDlg[(UINT)DLG_TYPE::MESHRENDER]->ShowWindow(false);

	m_arrComDlg[(UINT)DLG_TYPE::CAMERA] = new CCameraDlg;
	m_arrComDlg[(UINT)DLG_TYPE::CAMERA]->Create(IDD_CAMERADLG, this);
	m_arrComDlg[(UINT)DLG_TYPE::CAMERA]->ShowWindow(false);

	m_arrComDlg[(UINT)DLG_TYPE::COLLIDER2D] = new CCollider2DDlg;
	m_arrComDlg[(UINT)DLG_TYPE::COLLIDER2D]->Create(IDD_CCollider2DDlg, this);
	m_arrComDlg[(UINT)DLG_TYPE::COLLIDER2D]->ShowWindow(false);

	m_arrComDlg[(UINT)DLG_TYPE::ANIMATOR2D] = new CAnimatorDlg;
	m_arrComDlg[(UINT)DLG_TYPE::ANIMATOR2D]->Create(IDD_AnimatorDlg, this);
	m_arrComDlg[(UINT)DLG_TYPE::ANIMATOR2D]->ShowWindow(false);

	m_arrComDlg[(UINT)DLG_TYPE::SCRIPT] = new CScriptDlg;
	m_arrComDlg[(UINT)DLG_TYPE::SCRIPT]->Create(IDD_CScriptDlg, this);
	m_arrComDlg[(UINT)DLG_TYPE::SCRIPT]->ShowWindow(false);

	m_AddCom = new CAddComDlg;
	m_AddCom->Create(IDD_CAddComDlg, this);
	m_AddCom->ShowWindow(false);

	//리소스인포 다이얼로그 생성

	m_arrResInfoDlg[(UINT)RES_TYPE::MATERIAL] = new CMaterialDlg;
	m_arrResInfoDlg[(UINT)RES_TYPE::MATERIAL]->Create(IDD_CMaterialDlg, this);
	m_arrResInfoDlg[(UINT)RES_TYPE::MATERIAL]->ShowWindow(false);
	return 0;
}


BEGIN_MESSAGE_MAP(CComponentView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CComponentView 그리기

void CComponentView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CComponentView 진단

#ifdef _DEBUG
void CComponentView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CComponentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

void CComponentView::init()
{
	SetDlgPos();

	for (UINT i = 0; i < (UINT)DLG_TYPE::END; ++i)
	{
		if (nullptr == m_arrComDlg[i])
			continue;

		m_arrComDlg[i]->init(m_pTarget);
	}
	m_AddCom->init(m_pTarget);
}

void CComponentView::update_always()
{
	if (nullptr == m_pTarget)
		return;

	if (m_pTarget->Animator2D())
	{
		((CAnimatorDlg*)m_arrComDlg[(UINT)DLG_TYPE::ANIMATOR2D])->Animator2DStateCheck(m_pTarget);
	}
}

void CComponentView::update()
{

	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();

	//맵 툴이 켜질 경우
	if (TOOL_TYPE::TOOL_MAIN != frame->GetType() && TOOL_TYPE::TOOL_MAP == frame->GetType())
		return;

	//상시 업데이트가 진행됨

	update_always();

	//1초마다 업데이트가 진행됨

	static float fAccTime = 0.f;
	fAccTime += DT;
	if (fAccTime < 1.f)
		return;

	fAccTime = 0.f;

	SetDlgPos();

	if (nullptr == m_pTarget)
		return;

	for (UINT i = 0; i < (UINT)DLG_TYPE::END; ++i)
	{
		if (nullptr == m_pTarget->GetComponent((COMPONENT_TYPE)i) || i == (UINT)DLG_TYPE::SCRIPT)
		{
			m_arrComDlg[(UINT)DLG_TYPE::SCRIPT]->update(m_pTarget);
			continue;
		}
		if (nullptr == m_pTarget->GetComponent((COMPONENT_TYPE)i)
			|| nullptr == m_arrComDlg[i])
			continue;

		m_arrComDlg[i]->update(m_pTarget);
	}
	m_AddCom->update(m_pTarget);
}
void CComponentView::SetTarget(CGameObj * _pTarget)
{
	// ResourceInfoDlg 가 그려지고 있었으면 show false
	if (nullptr != m_pCurResInfoDlg)
	{
		m_pCurResInfoDlg->ShowWindow(false);
	}

	m_pTarget = _pTarget;
	SetDlgPos();
}
void CComponentView::SetResource(CReSource * _pResource)
{
	for (UINT i = 0; i < (UINT)DLG_TYPE::END; ++i)
	{
		if (nullptr != m_arrComDlg[i])
			m_arrComDlg[i]->ShowWindow(false);
	}

	RES_TYPE eType = _pResource->GetType();
	m_pCurResInfoDlg = m_arrResInfoDlg[(UINT)eType];

	if (nullptr == m_pCurResInfoDlg)
		return;

	m_pTarget = nullptr;
	m_pCurResInfoDlg->ShowWindow(true);

	m_pCurResInfoDlg->SetResource(_pResource);

	CRect rtView, rtDlg;
	GetClientRect(rtView);

	m_pCurResInfoDlg->GetWindowRect(rtDlg);
	m_pCurResInfoDlg->SetWindowPos(nullptr, 0, 0, rtView.Width(), rtDlg.Height(), 0);
}
void CComponentView::SetDlgPos()
{
	if (nullptr == m_pTarget)
	{
		for (UINT i = 0; i < (UINT)DLG_TYPE::END; ++i)
		{
			if (nullptr == m_arrComDlg[i])
				continue;

			m_arrComDlg[i]->ShowWindow(false);
		}

		return;
	}

	// Dlg Size 조절
	int iHeight = 0;

	CRect rtView;
	GetClientRect(rtView);

	CRect rtDlg;

	for (UINT i = 0; i < (UINT)DLG_TYPE::END; ++i)
	{
		if (nullptr == m_arrComDlg[i])
			continue;

		if (m_pTarget->GetComponent((COMPONENT_TYPE)i) || i == (UINT)DLG_TYPE::SCRIPT)
		{
			m_arrComDlg[i]->GetWindowRect(rtDlg);
			m_arrComDlg[i]->SetWindowPos(nullptr, 0, iHeight, rtView.Width(), rtDlg.Height(), 0);
			m_arrComDlg[i]->ShowWindow(true);
			iHeight += rtDlg.Height() - 1;
		}
		else
		{
			m_arrComDlg[i]->ShowWindow(false);
		}
	}
	m_AddCom->GetWindowRect(rtDlg);
	m_AddCom->SetWindowPos(nullptr, 0, iHeight, rtView.Width(), rtDlg.Height(), 0);
	m_AddCom->ShowWindow(true);

}


// CComponentView 메시지 처리기
