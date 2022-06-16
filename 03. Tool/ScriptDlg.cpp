// ScriptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ScriptDlg.h"
#include "afxdialogex.h"

#include <GameObj.h>
#include <ScriptMgr.h>
#include <Script.h>
#include <EventMgr.h>


// CScriptDlg 대화 상자

IMPLEMENT_DYNAMIC(CScriptDlg, CDialogEx)

CScriptDlg::CScriptDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_CScriptDlg, pParent)
{

}

CScriptDlg::~CScriptDlg()
{
}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ctrlScript);
	DDX_Control(pDX, IDC_COMBO1, m_cbScript);

	m_bSelect = false;
	m_iSelScriptList = -1;
}

void CScriptDlg::init(CGameObj * _pTarget)
{
	vector<wstring> vecName;
	CScriptMgr::GetScriptInfo(vecName);

	for (UINT i = 0; i < vecName.size(); ++i)
	{
		m_cbScript.AddString(vecName[i].c_str());
	}
}

void CScriptDlg::update(CGameObj * _pTarget)
{
	SetTarget(_pTarget);

	if (!m_bSelect)
	{
		m_ctrlScript.DeleteAllItems();

		vector<CScript*>& vecScript = _pTarget->GetScripts();

		for (UINT i = 0; i < vecScript.size(); ++i)
		{
			CString strName = CScriptMgr::GetScriptName(vecScript[i]);
			m_ctrlScript.InsertItem(i, strName);
		}
	}
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CScriptDlg::OnBnClickedDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CScriptDlg::OnNMClickList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST2, &CScriptDlg::OnNMKillfocusList2)
END_MESSAGE_MAP()


// CScriptDlg 메시지 처리기



void CScriptDlg::OnBnClickedButton()
{
	CString strScriptName;
	int iSel = m_cbScript.GetCurSel();
	m_cbScript.GetLBText(iSel, strScriptName);

	CScript* pNewScript = CScriptMgr::GetScript(strScriptName.GetBuffer());
	vector<CScript*>& TargetScript = GetTarget()->GetScripts();

	//이미 있는 스크립트를 추가하려는 경우
	for (UINT i = 0; i < TargetScript.size(); ++i)
	{
		if (TargetScript[i]->GetScriptType() == pNewScript->GetScriptType())
		{
			delete pNewScript;
			return;
		}
	}
	GetTarget()->AddComponent(pNewScript);
}


void CScriptDlg::OnBnClickedDelete()
{
	tEvent myEvent = {};

	if (m_bSelect)
	{
		vector<CScript*>& TargetScript = GetTarget()->GetScripts();
		int iSel = TargetScript[m_iSelScriptList]->GetScriptType();	

		myEvent.eType = EVENT_TYPE::DEL_SCRIPT;
		myEvent.lParam = (INT_PTR)GetTarget();
		myEvent.wParam = (INT_PTR)iSel;

		CEventMgr::GetInst()->AddEvent(myEvent);

		m_bSelect = false;
	}
	else
	{
		//CString strScriptName;
		//int iSel = m_cbScript.GetCurSel();
		//
		//myEvent.eType = EVENT_TYPE::DEL_SCRIPT;
		//myEvent.lParam = (INT_PTR)GetTarget();
		//myEvent.wParam = (INT_PTR)iSel;
		//
		//CEventMgr::GetInst()->AddEvent(myEvent);
	}
}

void CScriptDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_bSelect = true;

	*pResult = 0;
}


void CScriptDlg::OnNMKillfocusList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION    pos;
	pos = m_ctrlScript.GetFirstSelectedItemPosition();
	m_iSelScriptList = m_ctrlScript.GetNextSelectedItem(pos);

	if(m_iSelScriptList == -1)
		m_bSelect = false;

	*pResult = 0;
}
