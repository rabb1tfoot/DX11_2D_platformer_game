// GameObjectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "GameObjectDlg.h"
#include "ComponentView.h"
#include "MainFrm.h"
#include "afxdialogex.h"

#include <Scene.h>
#include <GameObj.h>
#include <SceneMgr.h>
#include <Layer.h>
#include <EventMgr.h>


// CGameObjectDlg 대화 상자

IMPLEMENT_DYNAMIC(CGameObjectDlg, CDialogEx)

CGameObjectDlg::CGameObjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGameObjectDlg, pParent)
	, m_bInit(false)
{

}

CGameObjectDlg::~CGameObjectDlg()
{
}

void CGameObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);

	m_bInit = true;
}


void CGameObjectDlg::init()
{
	//신에서 게임오브젝트를 가져와서 트리에 추가한다.

	m_ctrlTree.DeleteAllItems();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);

		if (nullptr == pLayer)
			continue;

		const vector<CGameObj*>& vecParentObj = pLayer->GetParentObject();

		for (UINT j = 0; j < vecParentObj.size(); ++j)
		{
			//트리에 최상위 부모를 넣는다.
			AddGameObject(vecParentObj[j], nullptr);
		}
	}
}

void CGameObjectDlg::AddGameObject(CGameObj * _pObj, HTREEITEM _hParent)
{
	HTREEITEM hItem = AddItem(_pObj->GetName().c_str(), _hParent, (DWORD_PTR)_pObj);

	const vector<CGameObj*>& vecChild = _pObj->GetChild();

	//재귀적으로 자식을 넣는다.
	for (UINT i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(vecChild[i], hItem);
	}
}

void CGameObjectDlg::MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem)
{
	wchar_t sz_name[255] = {};

	TV_ITEM item = {};

	item.hItem = _hSrcItem;
	item.pszText = sz_name;
	item.cchTextMax = 255;
	item.mask = TVIF_TEXT;

	m_ctrlTree.GetItem(&item);
	DWORD_PTR dwData = m_ctrlTree.GetItemData(_hSrcItem);

	HTREEITEM hCopyItem = AddItem(sz_name, _hDestItem, dwData);

	vector<HTREEITEM> vecChild;
	GetChildItem(hCopyItem, vecChild);

	for (UINT i = 0; i < vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], hCopyItem);
	}

	m_ctrlTree.DeleteItem(_hSrcItem);

	m_ctrlTree.Expand(_hDestItem, TVE_EXPAND);
}

void CGameObjectDlg::GetChildItem(HTREEITEM _hItem, vector<HTREEITEM>& _vecChild)
{
	if (false == m_ctrlTree.ItemHasChildren(_hItem))
		return;

	HTREEITEM hChildItem = m_ctrlTree.GetChildItem(_hItem);
	_vecChild.push_back(hChildItem);

	while (true)
	{
		hChildItem = m_ctrlTree.GetChildItem(hChildItem);

		if (nullptr ==  hChildItem)
			break;
		_vecChild.push_back(hChildItem);
	}
}


HTREEITEM CGameObjectDlg::AddItem(CString _str, HTREEITEM _hParent, DWORD_PTR _dwData)
{
	//트리에 넣을 구조체 선언
	TVINSERTSTRUCT tInsert = {};

	tInsert.hParent = _hParent;
	tInsert.hInsertAfter = TVI_SORT;
	tInsert.item.pszText = _str.GetBuffer();
	tInsert.item.mask = TVIF_TEXT;

	HTREEITEM hItem = m_ctrlTree.InsertItem(&tInsert);
	m_ctrlTree.SetItemData(hItem, _dwData);

	return hItem;
}

BEGIN_MESSAGE_MAP(CGameObjectDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CGameObjectDlg::OnTvnSelchangedTree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CGameObjectDlg::OnTvnBegindragTree)
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, &CGameObjectDlg::OnTvnBeginlabeledit)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CGameObjectDlg::OnTvnEndlabeledit)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, &CGameObjectDlg::OnTvnKeydownDel)
END_MESSAGE_MAP()


// CGameObjectDlg 메시지 처리기


void CGameObjectDlg::OnOK()
{
}

void CGameObjectDlg::OnCancel()
{
}



void CGameObjectDlg::PostNcDestroy()
{
	delete this;

	CDialogEx::PostNcDestroy();
}


BOOL CGameObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGameObjectDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_bInit)
	{
		// Tree Control Size 를 Dialog Size 로 맞춘다.
		//m_ctrlTree.SetWindowPos(nullptr, 0, 0, cx, cy, 0);
	}
}


void CGameObjectDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	CGameObj* pTargetObj = (CGameObj*)m_ctrlTree.GetItemData(pNMTreeView->itemNew.hItem);
	CComponentView* pComView = (CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComView();
	pComView->SetTarget(pTargetObj);
	*pResult = 0;
}


void CGameObjectDlg::OnTvnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	SetCapture();

	m_hDragItem = pNMTreeView->itemNew.hItem;

	*pResult = 0;
}


void CGameObjectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	// 드래그, 타겟 아이템이 없는 경우
	if (nullptr == m_hDragItem)
	{
		CDialogEx::OnLButtonUp(nFlags, point);
		return;
	}

	CRect rt;
	GetWindowRect(rt);

	//벗어나는 곳을 클릭하며 넘어간다.
	if (point.y > rt.Width())
		return;

	CGameObj* pSrcObj = (CGameObj*)m_ctrlTree.GetItemData(m_hDragItem);
	CGameObj* pDestObj = nullptr;

	if (nullptr != m_hTargetItem)
		pDestObj = (CGameObj*)m_ctrlTree.GetItemData(m_hTargetItem);

	//목적지가 가장 바깥일경우(없는 경우)
	if (nullptr == pDestObj)
	{
		if (nullptr != pSrcObj->GetParent())
		{
			tEvent event{};
			event.eType = EVENT_TYPE::CLEAR_PARANT;
			event.lParam = (DWORD_PTR)pSrcObj;

			CEventMgr::GetInst()->AddEvent(event);

			MoveItem(m_hDragItem, m_hTargetItem);
		}
	}
	//목적지가 있는경우
	else
	{
		//타깃이 자기 자신이거나 자기 자신보다 하위 자식이면 안된다.
		if (m_hDragItem != m_hTargetItem && !pDestObj->IsAncestor(pSrcObj))
		{
			MoveItem(m_hDragItem, m_hTargetItem);
			pDestObj->AddChild(pSrcObj);
		}
	}

	m_hTargetItem = nullptr;
	m_hDragItem = nullptr;
	m_ctrlTree.SelectDropTarget(nullptr);

	CDialogEx::OnLButtonUp(nFlags, point);
}



void CGameObjectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nullptr == m_hDragItem)
	{
		CDialogEx::OnMouseMove(nFlags, point);
		return;
	}
	//tree기준으로 좌표 변경
	::ClientToScreen(m_hWnd, &point);
	m_ctrlTree.ScreenToClient(&point);

	//현재 마우스에있는 오브젝트 타겟지정
	m_hTargetItem = m_ctrlTree.HitTest(point);

	if (nullptr != m_hTargetItem)
	{
		//하이라이트 설정
		m_ctrlTree.SelectDropTarget(m_hTargetItem);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CGameObjectDlg::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CGameObjectDlg::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	//Tree내 이름변경
	CEdit* edit = m_ctrlTree.GetEditControl();
	CString str;
	edit->GetWindowTextW(str);
	m_ctrlTree.SetItemText(pTVDispInfo->item.hItem, str);

	//태그값 변경
	CGameObj* pObj = (CGameObj*)m_ctrlTree.GetItemData(pTVDispInfo->item.hItem);
	pObj->SetName(str.GetBuffer());


	*pResult = 0;
}


void CGameObjectDlg::OnTvnKeydownDel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pTVKeyDown->wVKey == VK_DELETE)
	{
		//실제 오브젝트 삭제
		CComponentView* pComView = (CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComView();
		tEvent tEv ={};
		tEv.eType = EVENT_TYPE::DELETE_OBJECT;
		tEv.lParam = INT_PTR(pComView->GetTarget());
		CEventMgr::GetInst()->AddEvent(tEv);

		//Tree내 삭제
		HTREEITEM hItem = m_ctrlTree.GetSelectedItem();
		m_ctrlTree.DeleteItem(hItem);
	}
	*pResult = 0;
}
