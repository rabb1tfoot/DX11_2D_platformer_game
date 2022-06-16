#include "stdafx.h"
#include "Core.h"

#include "Device.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "DialogueMgr.h"

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Sound.h"
#include "Material.h"
#include "GameObj.h"

#include "Component.h"
#include "MeshRender.h"
#include "Transform.h"
#include <Script/InfoScript.h>
#include <Script/SaveLoadMgr.h>


CCore::CCore()
{
}

CCore::~CCore()
{
}

int CCore::Init(HWND _hWnd, bool _bWindowed)
{
	m_hWnd = _hWnd;

	RECT rt = {};
	GetClientRect(m_hWnd, &rt);

	m_tRes.fWidth = (float)(rt.right - rt.left);
	m_tRes.fHeight = (float)(rt.bottom - rt.top);

	// Manager 초기화
	if(FAILED(CDevice::GetInst()->Init(m_hWnd, m_tRes, _bWindowed)))
		return E_FAIL;
	CDevice::GetInst()->Init2D(m_hWnd, m_tRes, _bWindowed);
	CPathMgr::init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CRenderMgr::GetInst()->Init();
	CResMgr::GetInst()->Init();
	CSceneMgr::GetInst()->Init();
	CDialogueMgr::GetInst()->Init();
	
	// 상수버퍼 등록
	CDevice::GetInst()->CreateConstBuffer(L"Transform", sizeof(tTransform), 0);
	CDevice::GetInst()->CreateConstBuffer(L"ShaderParam", sizeof(tShaderParam), 1);
	CDevice::GetInst()->CreateConstBuffer(L"Anim2D", sizeof(tAnim2D), 2);
	CDevice::GetInst()->CreateConstBuffer(L"Tile", sizeof(tTileParam), 3);
	CDevice::GetInst()->CreateConstBuffer(L"Global", sizeof(tGlobalValue), 4);

	SetRenderMode(false);
	Play(false);

	CScene* pInfoScene = CSceneMgr::GetInst()->GetInfoScene();
	//플레이어 필요한 정보용 오브젝트 만들기
	CGameObj* pInfoObj = new CGameObj;
	pInfoObj->SetName(L"Info");
	pInfoObj->AddComponent(new CTransform);
	pInfoObj->AddComponent(new CInfoScript);
	pInfoScene->AddObject(0, pInfoObj);

	//LoadScene();

	return S_OK;
}

void CCore::Progress()
{
	CKeyMgr::GetInst()->Update();
	CTimeMgr::GetInst()->Update();

	//Scene Progress
	(CSceneMgr::GetInst()->*scene_progress)();

	// Scene Render	
	(CRenderMgr::GetInst()->*render)();

}

void CCore::SetRenderMode(bool _bTool)
{
	if (_bTool)
		render = &CRenderMgr::render_tool;
	else
		render = &CRenderMgr::render;
}

void CCore::Play(bool _bTrue)
{
	if (_bTrue)
		scene_progress = &CSceneMgr::progress;
	else
		scene_progress = &CSceneMgr::progress_pause;
}

void CCore::LoadScene()
{
	wstring strInitPath = CPathMgr::GetResPath();
	strInitPath += L"Scene\\Title.scene";

	CSaveLoadMgr::LoadScene(strInitPath.c_str());
}

void CCore::LoadResource(wstring _strFolderPath)
{
	wstring strFolderPath = (wstring)_strFolderPath + L"\\*.*";

	WIN32_FIND_DATA tData = {};
	HANDLE h = (HANDLE)FindFirstFile(strFolderPath.c_str(), &tData);
	UINT err = GetLastError();

	while (true)
	{
		if (FILE_ATTRIBUTE_DIRECTORY & tData.dwFileAttributes)
		{
			if (wcscmp(tData.cFileName, L".") && wcscmp(tData.cFileName, L".."))
			{
				wstring temp = (wstring)_strFolderPath + tData.cFileName;
				temp += L'\\';
				LoadResource(temp.c_str());
			}
		}
		else
		{
			const wchar_t* pExt = CPathMgr::GetExt(tData.cFileName);
			wstring strPath = (wstring)_strFolderPath + tData.cFileName;
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
		if (false == FindNextFile(h, &tData))
		{
			err = GetLastError();
			break;
		}
	}
}
