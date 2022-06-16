// CollisionCheck.cpp: 구현 파일
//

#include "stdafx.h"
#include "CollisionCheck.h"
#include "afxdialogex.h"

#include <ColliderMgr.h>
#include <SceneMgr.h>

#include <Collider2D.h>
#include <Scene.h>
#include <Layer.h>

// CCollisionCheck 대화 상자

IMPLEMENT_DYNAMIC(CCollisionCheck, CDialogEx)

CCollisionCheck::CCollisionCheck(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCollisionCheck, pParent)
{

}

CCollisionCheck::~CCollisionCheck()
{
}

void CCollisionCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbCheck1);
	DDX_Control(pDX, IDC_COMBO2, m_cbCheck2);
	DDX_Control(pDX, IDC_LIST2, m_ctrlList);

	m_iListIdx = 0;

	//레이어의 목록을 가져와서 1,2번 박스에다 넣는다.
	AddComboData();

	//이미 만들어진 충돌체크값을 리스트에다 넣는다.
	AddListData();

}


BEGIN_MESSAGE_MAP(CCollisionCheck, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCollisionCheck::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &CCollisionCheck::OnBnClickedDel)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CCollisionCheck 메시지 처리기


void CCollisionCheck::OnBnClickedAdd()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	CString strScriptName;
	int iSel = m_cbCheck1.GetCurSel();
	m_cbCheck1.GetLBText(iSel, strScriptName);

	CString strScriptName2;
	int iSel2 = m_cbCheck2.GetCurSel();
	m_cbCheck2.GetLBText(iSel2, strScriptName2);

	CLayer* pLayer1 = pCurScene->FindLayer(strScriptName.GetBuffer());
	UINT LayerIdx = pLayer1->GetLayerIdx();

	CLayer* pLayer2 = pCurScene->FindLayer(strScriptName2.GetBuffer());
	UINT LayerIdx2 = pLayer2->GetLayerIdx();

	CColliderMgr::GetInst()->CollisionCheck(LayerIdx, LayerIdx2);

	wstring str1 = strScriptName.GetBuffer();
	wstring str2 = strScriptName2.GetBuffer();
	wstring str3 = str1 + L"-" + str2;

	m_ctrlList.InsertItem(m_iListIdx, str3.c_str());
	++m_iListIdx;
}


void CCollisionCheck::OnBnClickedDel()
{
	POSITION    pos;
	pos = m_ctrlList.GetFirstSelectedItemPosition();
	int iSelList = m_ctrlList.GetNextSelectedItem(pos);

	wstring str = m_ctrlList.GetItemText(iSelList, 0).GetBuffer();
	int isubIdx = -1;
	for (UINT i = 0; i < str.length(); ++i)
	{
		if (str[i] == '-')
		{
			isubIdx = i;
			break;
		}

	}
	wstring str1 = str.substr(0, isubIdx);
	wstring str2 = str.substr(isubIdx+1, str.length());

	CColliderMgr::GetInst()->DelCollisionCheck(str1, str2);

	m_ctrlList.DeleteItem(iSelList);
}


BOOL CCollisionCheck::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}


int CCollisionCheck::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CCollisionCheck::AddComboData()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer** pLayer = pCurScene->GetAllLayer();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (pLayer[i] == nullptr)
			continue;
		m_cbCheck1.AddString(pLayer[i]->GetName().c_str());
		m_cbCheck2.AddString(pLayer[i]->GetName().c_str());
	}
}

void CCollisionCheck::AddListData()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	UINT* CollisionCheckArr = CColliderMgr::GetInst()->GetCollisionCheckArr();
	CLayer** pLayer = pCurScene->GetAllLayer();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			//비트 곱연산으로 마킹이 되어있는지 체크한다.
			if (CollisionCheckArr[i] & (1 << j))
			{
				// 리스트에 추가한다.
				wstring str1 = pLayer[i]->GetName();
				wstring str2 = pLayer[j]->GetName();

				wstring str3 = str1 + L"-" + str2;

				m_ctrlList.InsertItem(m_iListIdx, str3.c_str());
				++m_iListIdx;
			}
		}
	}
}
