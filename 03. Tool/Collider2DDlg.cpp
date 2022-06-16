// Collider2DDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Collider2DDlg.h"
#include "afxdialogex.h"

#include <GameObj.h>
#include <Collider2D.h>


// CCollider2DDlg 대화 상자

IMPLEMENT_DYNAMIC(CCollider2DDlg, CDialogEx)

void CCollider2DDlg::update(CGameObj * _Target)
{
	SetTarget(_Target);

	if (IsEditActive() || IsCbActive())
		return;

	CCollider2D* pCol = GetTarget()->Collider2D();

	Vec3 vOffset = pCol->GetOffset();
	Vec3 vScale = pCol->GetScale();

	Vec3 vArrData[2] = { vOffset, vScale };
	COLLIDER2D_TYPE eType = pCol->GetCollider2DType();

	CString str;
	for (UINT i = 0; i < 6; ++i)
	{
		float fInputData = vArrData[i / 3][i % 3];
		str.Format(L"%f", fInputData);
		m_edit[i].SetWindowTextW(str);
	}
	m_cbType.SetCurSel((UINT)eType);
}

CCollider2DDlg::CCollider2DDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_CCollider2DDlg, pParent)
{

}

CCollider2DDlg::~CCollider2DDlg()
{
}

void CCollider2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbType);
	for (UINT i = 0; i < 6; ++i)
	{
		DDX_Control(pDX, 2001 + i, m_edit[i]);
	}
}



BEGIN_MESSAGE_MAP(CCollider2DDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT6, &CComponentDlg::OnEnSetfocusEdit)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT6, &CComponentDlg::OnEnKillfocusEdit)

	ON_CONTROL_RANGE(CBN_SETFOCUS, IDC_COMBO1, IDC_COMBO1, &CComponentDlg::OnCbSetfocusEdit)
	ON_CONTROL_RANGE(CBN_KILLFOCUS, IDC_COMBO1, IDC_COMBO1, &CComponentDlg::OnCbKillfocusEdit)

	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT6, &CCollider2DDlg::OnEnChangeEdit)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCollider2DDlg::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


// CCollider2DDlg 메시지 처리기


void CCollider2DDlg::OnEnChangeEdit(UINT _id)
{
	if (!IsEditActive())
		return;

	CCollider2D* pCol = GetTarget()->Collider2D();
	CString str;
	Vec3 arrData[2] = {};

	for (UINT i = 0; i < 6; ++i)
	{
		m_edit[i].GetWindowTextW(str);

		float fData = (float)_wtof(str);

		arrData[i / 3][i % 3] = fData;
	}
	pCol->SetOffset(arrData[0]);
	pCol->SetScale(arrData[1]);
}

BOOL CCollider2DDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_cbType.AddString(L"RECT");
	m_cbType.AddString(L"CIRCLE");

	return TRUE;
}


void CCollider2DDlg::OnCbnSelchangeCombo()
{
	CComponentDlg::OnOk();
	CCollider2D* pCol = GetTarget()->Collider2D();
	int iIdx = m_cbType.GetCurSel();

	pCol->SetColliderType(COLLIDER2D_TYPE(iIdx));
}
