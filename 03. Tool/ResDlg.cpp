// ResDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ResDlg.h"
#include "afxdialogex.h"
#include "ComponentView.h"
#include "MainFrm.h"

#include <ResMgr.h>
#include <PathMgr.h>
// CResDlg 대화 상자

IMPLEMENT_DYNAMIC(CResDlg, CDialogEx)

CResDlg::CResDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CResDlg, pParent)
	, m_bInit(false)
{

}

CResDlg::~CResDlg()
{
}

void CResDlg::init()
{
	// Resource폴더의 리소스 로딩
	CString strContent = CPathMgr::GetResPath();
	LoadResource(strContent);

	//트리 컨트롤 갱신
	Renew();
}

void CResDlg::Renew()
{

	m_ctrlTree.DeleteAllItems();

	//항목 추가
	HTREEITEM arrItem[(UINT)RES_TYPE::END] = {
		AddItem(L"Material", nullptr, 0, TVI_LAST)
		, AddItem(L"Prefab", nullptr, 0, TVI_LAST)
		, AddItem(L"Mesh", nullptr, 0, TVI_LAST)
		, AddItem(L"Shader", nullptr, 0, TVI_LAST)
		, AddItem(L"Texture", nullptr, 0, TVI_LAST)
		, AddItem(L"Sound", nullptr, 0, TVI_LAST)
	};

	//가져온뒤 리소스 목록을 가져온다.
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, CReSource*>& map = CResMgr::GetInst()->GetResources(RES_TYPE(i));

		for (const auto& pair : map)
		{
			CString strName = CPathMgr::GetFileName(pair.first.c_str());
			AddItem(strName, arrItem[i], (DWORD_PTR)pair.second, TVI_SORT);
		}
	}
}

void CResDlg::LoadResource(CString _strFolderPath)
{
	wstring strFolderPath = _strFolderPath + L"\\*.*";

	WIN32_FIND_DATA tData = {};
	HANDLE h = (HANDLE)FindFirstFile(strFolderPath.c_str(), &tData);
	UINT err = GetLastError();

	while (true)
	{
		if (FILE_ATTRIBUTE_DIRECTORY & tData.dwFileAttributes)
		{
			if (wcscmp(tData.cFileName, L".") && wcscmp(tData.cFileName, L".."))
			{
				wstring temp = _strFolderPath + tData.cFileName;
				temp += L'\\';
				LoadResource(temp.c_str());
			}
		}
		else
		{
			const wchar_t* pExt = CPathMgr::GetExt(tData.cFileName);
			wstring strPath = _strFolderPath + tData.cFileName;
			wstring strKey = CPathMgr::GetRelativePath(strPath.c_str());

			if (!wcscmp(pExt, L".png")
				|| !wcscmp(pExt, L".PNG")
				|| !wcscmp(pExt, L".bmp")
				|| !wcscmp(pExt, L".jpg")
				|| !wcscmp(pExt, L".jpeg"))
			{
				CResMgr::GetInst()->Load<CTexture>(strKey, strKey);
			}
			else if (!wcscmp(pExt, L".mp3")
				|| !wcscmp(pExt, L".ogg")
				|| !wcscmp(pExt, L".wav"))
			{
				CResMgr::GetInst()->Load<CSound>(strKey, strKey);
			}
			else if (!wcscmp(pExt, L".mtrl"))
			{
				CResMgr::GetInst()->Load<CMaterial>(strKey, strKey);
			}
			else if (!wcscmp(pExt, L".mdat"))
			{
				//CResMgr::GetInst()->Load<CMeshData>(strKey, strPath);
			}
			else if (!wcscmp(pExt, L".prf"))
			{
				CResMgr::GetInst()->Load<CPrefab>(strKey, strKey);
			}
		}
		//로딩 실패
		if(false == FindNextFile(h, &tData))
		{
			err = GetLastError();
			break;
		}
	}

}

void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);

	m_bInit = true;
}

HTREEITEM CResDlg::AddItem(CString _str, HTREEITEM _hParent, DWORD_PTR _dwData, HTREEITEM _hSort)
{
	TVINSERTSTRUCT tInsert = {};

	tInsert.hParent = _hParent;
	tInsert.hInsertAfter = _hSort;
	tInsert.item.pszText = _str.GetBuffer();
	tInsert.item.mask = TVIF_TEXT;

	HTREEITEM hItem = m_ctrlTree.InsertItem(&tInsert);
	m_ctrlTree.SetItemData(hItem, _dwData);

	return hItem;
}

void CResDlg::MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem)
{
	wchar_t szName[255] = {};

	TV_ITEMW item = {};
	item.hItem = _hSrcItem;
	item.pszText = szName;
	item.cchTextMax = 255;
	item.mask = TVIF_TEXT | TVIF_HANDLE;

	m_ctrlTree.GetItem(&item);

	DWORD_PTR dwData = m_ctrlTree.GetItemData(_hSrcItem);
	
	HTREEITEM hCopyItem = AddItem(szName, _hDestItem, dwData, TVI_SORT);

	vector<HTREEITEM> vecChild;
	GetChildItem(_hSrcItem, vecChild);

	for(UINT i=0; i = vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], hCopyItem);
	}

	m_ctrlTree.DeleteItem(_hSrcItem);
	m_ctrlTree.Expand(_hDestItem, TVE_EXPAND);
}

void CResDlg::GetChildItem(HTREEITEM _hItem, vector<HTREEITEM>& _vecChild)
{
	if (!m_ctrlTree.ItemHasChildren(_hItem))
		return;

	HTREEITEM hChildItem = m_ctrlTree.GetChildItem(_hItem);
	_vecChild.push_back(hChildItem);

	while (true)
	{
		hChildItem = m_ctrlTree.GetNextSiblingItem(hChildItem);

		if (nullptr == hChildItem)
			break;

		_vecChild.push_back(hChildItem);
	}
}


BEGIN_MESSAGE_MAP(CResDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, &CResDlg::OnTvnBeginlabeledit)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CResDlg::OnTvnEndlabeledit)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, &CResDlg::OnTvnKeydown)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CResDlg::OnNMClickTree)
END_MESSAGE_MAP()


// CResDlg 메시지 처리기


void CResDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_bInit)
	{
		m_ctrlTree.SetWindowPos(nullptr, 0, 0, cx, cy, 0);
	}
}

void CResDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}

void CResDlg::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hParent = m_ctrlTree.GetParentItem(pTVDispInfo->item.hItem);

	//최상위 부모는 카탈로그라서 변경 불가능
	if (nullptr == hParent)
		*pResult = 1;
	else
		*pResult = 0;
}


void CResDlg::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	//Tree내 이름변경
	CEdit* edit = m_ctrlTree.GetEditControl();
	CString str;
	edit->GetWindowTextW(str);
	m_ctrlTree.SetItemText(pTVDispInfo->item.hItem, str);

	//파일이름 변경 + 킷값 페스값 변경
	CReSource* pRes = (CReSource*)m_ctrlTree.GetItemData(pTVDispInfo->item.hItem);
	if (RES_TYPE::MATERIAL == pRes->GetType())
	{
		wstring wPath = CPathMgr::GetResPath();
		wstring wOldPath = wPath;
		wstring wPath2 = L"Material\\"; wPath2 += str.GetBuffer(); wPath2 += L".mtrl";
		wPath += wPath2;
		wOldPath += pRes->GetPath();
		_wrename(wOldPath.c_str(), wPath.c_str());

		pRes->SetName(wPath2);
		pRes->SetPath(wPath2);

	}

	*pResult = 0;
}



void CResDlg::OnOK()
{

}


void CResDlg::OnCancel()
{

}


void CResDlg::OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CResDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT pt;
	::GetCursorPos(&pt);
	m_ctrlTree.ScreenToClient(&pt);

	HTREEITEM hTargetItem = m_ctrlTree.HitTest(pt);

	//최상위부모(카테고리) 클릭한경우
	if (nullptr == m_ctrlTree.GetParentItem(hTargetItem))
	{
		*pResult = 0;
		return;
	}

	if (nullptr != hTargetItem)
	{
		CReSource* pResource = (CReSource*)m_ctrlTree.GetItemData(hTargetItem);

		CComponentView* pConView = (CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComView();
		pConView->SetResource(pResource);
	}

	*pResult = 0;
}
