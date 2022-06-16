// TransformDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TransformDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include <SceneMgr.h>
#include <EventMgr.h>
#include <GameObj.h>
#include <Transform.h>
#include <Scene.h>
#include <Layer.h>


// CTransformDlg 대화 상자

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_TRANSFORMDLG, pParent)
	, m_bActive(FALSE)
{

}

CTransformDlg::~CTransformDlg()
{
}


void CTransformDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialogEx::DoDataExchange(pDX);

	for (UINT i = 0; i < 9; ++i)
	{
		DDX_Control(pDX, 2001 + i, m_edit[i]);
	}
	DDX_Check(pDX, IDC_CHECK1, m_bActive);
	DDX_Control(pDX, IDC_COMBO1, m_cbLayer);
}

BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)

	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnEnSetfocusEdit)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnEnKillfocusEdit)

	ON_CONTROL_RANGE(CBN_SETFOCUS, IDC_COMBO1, IDC_COMBO1, &CComponentDlg::OnCbSetfocusEdit)
	ON_CONTROL_RANGE(CBN_KILLFOCUS, IDC_COMBO1, IDC_COMBO1, &CComponentDlg::OnCbKillfocusEdit)


	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT9, &CTransformDlg::OnEnChangeEdit)

	ON_BN_CLICKED(IDC_CHECK1, &CTransformDlg::OnBnClickedActive)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTransformDlg::OnCbnSelchangeComboLayer)
	ON_CBN_DROPDOWN(IDC_COMBO1, &CTransformDlg::OnCbnDropdownCombo)
END_MESSAGE_MAP()


void CTransformDlg::init(CGameObj * _pTarget)
{
	m_cbLayer.ResetContent();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0, j = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);

		if (nullptr ==  pLayer)
			continue;

		m_cbLayer.InsertString(j, pLayer->GetName().c_str());

		//아이템에 실제 레이어의 인덱스를 넣는다.
		m_cbLayer.SetItemData(j, i);
		++j;
	}
}

void CTransformDlg::update(CGameObj * _pTarget)
{
	if (IsEditActive() || IsCbActive())
		return;
	SetTarget(_pTarget);

	CString str;

	CTransform* pTrans = GetTarget()->Transform();

	Vec3 arrData[3] = { pTrans->GetLocalPos(), pTrans->GetLocalScale(), pTrans->GetLocalRot() };

	arrData[2].x = (arrData[2].x * 180.f) / XM_PI;
	arrData[2].y = (arrData[2].y * 180.f) / XM_PI;
	arrData[2].z = (arrData[2].z * 180.f) / XM_PI;

	for (UINT i = 0; i < 9; ++i)
	{
		str.Format(L"%f", arrData[i / 3][i % 3]);
		m_edit[i].SetWindowTextW(str);
	}
	//레이어 확인
	CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(GetTarget()->GetLayerIdx());
	UINT iSel = m_cbLayer.FindString(0, pLayer->GetName().c_str());
	m_cbLayer.SetCurSel(iSel);

	m_bActive = GetTarget()->GetActive();
	UpdateData(false);
}


void CTransformDlg::OnEnChangeEdit(UINT _id)
{
	if (!IsEditActive())
		return;

	CTransform* pTrans = GetTarget()->Transform();
	CString str;
	Vec3 arrData[3] = {};

	for (UINT i = 0; i < 9; ++i)
	{
		m_edit[i].GetWindowTextW(str);

		float fData = (float)_wtof(str);
		arrData[i / 3][i % 3] = fData;
	}

	arrData[2].x = arrData[2].x / 180.f * XM_PI;
	arrData[2].y = arrData[2].y / 180.f * XM_PI;
	arrData[2].z = arrData[2].z / 180.f * XM_PI;

	pTrans->SetLocalPos(arrData[0]);
	pTrans->SetLocalScale(arrData[1]);
	pTrans->SetLocalRot(arrData[2]);
}



// CTransformDlg 메시지 처리기


void CTransformDlg::OnBnClickedActive()
{
	UpdateData(true);
	GetTarget()->SetActive(m_bActive);
}


void CTransformDlg::OnCbnSelchangeComboLayer()
{
	int iSel = m_cbLayer.GetCurSel();
	DWORD_PTR iLayerIdx = m_cbLayer.GetItemData(iSel);
	int iCurIdx = GetTarget()->GetLayerIdx();

	//동일한 레이어로 변경시 변화없음
	if (iSel == iCurIdx)
		return;

	//자식도 같이 레이어 변경할 것인지 확인
	bool bMoveAll = true;
	if (!GetTarget()->GetChild().empty())
	{
		int id = MessageBox(L"하위 자식도 layer를 변경하시겠습니까?", L"레이어 변경", MB_OKCANCEL);
		if (id == IDCANCEL)
			bMoveAll = false;
	}
	tEvent event = {};
	event.eType = EVENT_TYPE::CHANGE_LAYER;
	event.lParam = (DWORD_PTR)GetTarget();
	event.wParam = (DWORD_PTR)MAKELONG(bMoveAll, iLayerIdx);

	CEventMgr::GetInst()->AddEvent(event);
}


void CTransformDlg::OnCbnDropdownCombo()
{
	init(GetTarget());
}
