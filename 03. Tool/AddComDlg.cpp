// AddComDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AddComDlg.h"
#include "afxdialogex.h"

#include "ComponentView.h"
#include <GameObj.h>
#include <Transform.h>
#include <MeshRender.h>
#include <Camera.h>
#include <Collider2D.h>
#include <Animator2D.h>
#include <Script.h>
#include <Material.h>

// CAddComDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddComDlg, CDialogEx)

CAddComDlg::CAddComDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_AnimatorDlg, pParent)
{

}

CAddComDlg::~CAddComDlg()
{
}

void CAddComDlg::init(CGameObj * _pTarget)
{
	wstring ComName = L"";
	for (UINT i = 1; i < 5; ++i)
	{
		switch (i)
		{
		case 1:
			ComName = L"CAMERA";
			break;
		case 2:
			ComName = L"COLLIDER2D";
			break;
		case 3:
			ComName = L"ANIMATOR2D";
			break;
		case 4:
			ComName = L"MESHRENDER";
			break;
		}
		m_cbSelect.AddString(ComName.c_str());
	}
	m_cbSelect.SetItemData(0, (DWORD_PTR)m_Comtype0);
	m_cbSelect.SetItemData(1, (DWORD_PTR)m_Comtype1);
	m_cbSelect.SetItemData(2, (DWORD_PTR)m_Comtype2);
	m_cbSelect.SetItemData(3, (DWORD_PTR)m_Comtype3);
}

void CAddComDlg::update(CGameObj * _Target)
{
	SetTarget(_Target);
}


void CAddComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbSelect);

	m_Comtype0 = COMPONENT_TYPE::ANIMATION2D;
	m_Comtype1 = COMPONENT_TYPE::CAMERA;
	m_Comtype2 = COMPONENT_TYPE::COLLIDER2D;
	m_Comtype3 = COMPONENT_TYPE::MESHRENDER;
}


BEGIN_MESSAGE_MAP(CAddComDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddComDlg::OnBnClickedAddCom)
END_MESSAGE_MAP()


// CAddComDlg 메시지 처리기


void CAddComDlg::OnBnClickedAddCom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIdx = m_cbSelect.GetCurSel();
	CGameObj* pTarget = GetTarget();
	COMPONENT_TYPE eType = (COMPONENT_TYPE)m_cbSelect.GetItemData(iIdx);

	switch (eType)
	{
		if (nullptr == pTarget->GetComponent(eType))
			return;

		case COMPONENT_TYPE::CAMERA:
		{
			CCamera* pCam = new CCamera;
			pCam->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
			pCam->SetFOV(XM_PI / 4.f);
			pCam->SetScale(1.f);

			pTarget->AddComponent(pCam);
		}
			break;
		case COMPONENT_TYPE::COLLIDER2D:
		{
			CCollider2D* pCol = new CCollider2D;
			pCol->SetColliderType(COLLIDER2D_TYPE::RECT);
			pTarget->AddComponent(pCol);
		}
			break;
		case COMPONENT_TYPE::ANIMATION2D:
		{
			CAnimator2D* pAnim = new CAnimator2D;
			pAnim->SetCurAnim(nullptr);
			pTarget->AddComponent(pAnim);
		}
			break;
		case COMPONENT_TYPE::MESHRENDER:
		{
			CMeshRender* pRender = new CMeshRender;
			pRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
			pRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pTarget->AddComponent(pRender);
		}
			break;
	}
}
