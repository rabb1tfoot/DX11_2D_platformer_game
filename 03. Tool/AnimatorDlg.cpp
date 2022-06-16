// AnimatorDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimatorDlg.h"
#include "afxdialogex.h"
#include "AddAnimDlg.h"
#include "AddAnimTexDlg.h"

#include <GameObj.h>
#include <Animator2D.h>
#include <Animation2D.h>


// CAnimatorDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimatorDlg, CDialogEx)

CAnimatorDlg::CAnimatorDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_AnimatorDlg, pParent)
	, m_bRepeat(false)
{

}

CAnimatorDlg::~CAnimatorDlg()
{
}

void CAnimatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbAnimList);
	DDX_Control(pDX, IDC_BUTTON1, m_btnPlay);
	DDX_Check(pDX, IDC_CHECK2, m_bRepeat);
}

void CAnimatorDlg::init(CGameObj * _Target)
{
	// GameObject 의 Animator2D 컴포넌트로 부터 Animation 목록을 읽어온다.
	SetTarget(_Target);
	
	if (nullptr == _Target || nullptr == _Target->Animator2D())
		return;
	
	CAnimator2D* pAnimator = GetTarget()->Animator2D();
	
	const map<wstring, CAnimation2D*>& mapAnim = pAnimator->GetAnimList();
	
	for (const auto& pair : mapAnim)
	{
		m_cbAnimList.AddString(pair.first.c_str());
	}
}

void CAnimatorDlg::update(CGameObj * _Target)
{
	SetTarget(_Target);
	
	if (IsCbActive())
		return;

	CAnimator2D* pAnimator = GetTarget()->Animator2D();
	
	const map<wstring, CAnimation2D*>& mapAnim = pAnimator->GetAnimList();

	m_cbAnimList.ResetContent();

	for (const auto& pair : mapAnim)
	{
		m_cbAnimList.AddString(pair.first.c_str());
	}

	CAnimation2D* pCurAnim = pAnimator->GetCurAnim();
	
	if (nullptr != pCurAnim)
	{
		CString strCurName = pCurAnim->GetName().c_str();
		int idx = m_cbAnimList.FindString(0, strCurName);
	
		//if(-1 == idx)
	
		m_cbAnimList.SetCurSel(idx);
	}
}

void CAnimatorDlg::Animator2DStateCheck(CGameObj * _pTarget)
{
	SetTarget(_pTarget);

	CAnimation2D* pCurAnim = _pTarget->Animator2D()->GetCurAnim();
	if (nullptr == pCurAnim)
		return;

	// 반복재생 아님, Animation 이 종료됨 
	if (pCurAnim->IsFinish() && !_pTarget->Animator2D()->IsRepeat())
	{
		m_btnPlay.SetWindowTextW(L"재생");
	}
}


BEGIN_MESSAGE_MAP(CAnimatorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimatorDlg::OnBnClickedPlay)

	ON_CONTROL_RANGE(CBN_SETFOCUS, IDC_COMBO1, IDC_COMBO1, &CComponentDlg::OnCbSetfocusEdit)
	ON_CONTROL_RANGE(CBN_KILLFOCUS, IDC_COMBO1, IDC_COMBO1, &CComponentDlg::OnCbKillfocusEdit)

	ON_BN_CLICKED(IDC_BUTTON2, &CAnimatorDlg::OnBnClickedAddAnim)
	ON_BN_CLICKED(IDC_CHECK2, &CAnimatorDlg::OnBnClickedRepeat)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAnimatorDlg::OnCbnSelchangeAnim)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimatorDlg::OnBnClickedAddAnimTexture)
END_MESSAGE_MAP()


// CAnimatorDlg 메시지 처리기


void CAnimatorDlg::OnBnClickedPlay()
{
	CAnimation2D* pAnim = GetTarget()->Animator2D()->GetCurAnim();

	OnOk();

	if (nullptr == pAnim)
		return;
	
	if (pAnim->IsPlay())
	{
		pAnim->Pause();
		m_btnPlay.SetWindowTextW(L"재생");
	}
	else
	{
		GetTarget()->Animator2D()->SetRepeat(m_bRepeat);
		pAnim->Play();
		m_btnPlay.SetWindowTextW(L"정지");
	}
}


void CAnimatorDlg::OnBnClickedAddAnim()
{
	CAddAnimDlg AnimDlg;

	AnimDlg.SetTarget(GetTarget());

	AnimDlg.DoModal();
}


void CAnimatorDlg::OnBnClickedRepeat()
{
	UpdateData();

	CAnimation2D* pAnim = GetTarget()->Animator2D()->GetCurAnim();
	if (pAnim->IsPlay())
	{
		GetTarget()->Animator2D()->SetRepeat(m_bRepeat);
	}
}


void CAnimatorDlg::OnCbnSelchangeAnim()
{
	int iIdx = m_cbAnimList.GetCurSel();
	CString str;
	m_cbAnimList.GetLBText(iIdx, str);
	GetTarget()->Animator2D()->PlayAnimation(str.GetBuffer(), m_bRepeat);
	GetTarget()->Animator2D()->Pause();

	m_btnPlay.SetWindowTextW(L"재생");
}


void CAnimatorDlg::OnBnClickedAddAnimTexture()
{
	CAddAnimTexDlg AnimDlg;

	AnimDlg.SetTarget(GetTarget());

	AnimDlg.DoModal();
}
