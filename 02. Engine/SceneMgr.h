#pragma once

class CCamera;
class CGameObj;
#include "Scene.h"
#include "ResPtr.h"
#include "Sound.h"

class CSceneMgr
{
	SINGLE(CSceneMgr);

private:
	CScene* m_pCurScene;
	CScene*  m_pInfoScene;
	CScene*  m_pMapScene;
	CScene*  m_pToolScene;

	CResPtr<CSound> m_pSound;

public:
	void RegisterCamera(CCamera* _pCam);
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetMapScene() { return m_pMapScene; }
	CScene* GetToolScene() { return m_pToolScene; }
	CScene* GetInfoScene() { return m_pInfoScene; }
	CResPtr<CSound>& GetSound() { return m_pSound; }

	void SetMapScene(CScene* p_MapScene){ m_pMapScene = p_MapScene; }
	void SetToolScene(CScene* p_ToolScene) { m_pToolScene = p_ToolScene; }
	void ChangeCurScene(CScene* _pNextScene) { m_pCurScene = _pNextScene; m_pCurScene->Awake(); }

	//게임에서는 이 함수를 써야함
	void ChangeScene(CScene* _pNextScene) { SAFE_DELETE(m_pCurScene); m_pCurScene = _pNextScene; m_pCurScene->Awake(); }

	void FindGameObject(const wstring& _strTagName, vector<CGameObj*>& _vecOut);

public:
	void Init();
	void progress();
	void progress_pause();
	void Render();

private:
	void CreatePrefab();
	void LoadTexture();
};

