#pragma once

class CObj;
class CRenderMgr;
class CSceneMgr;

enum class SCENE_STATE
{
	PLAY,
	PAUSE,
	STOP,
	SAVE,
	LOAD,
};

class CCore
{
	SINGLE(CCore);
private:
	HWND			m_hWnd;	// 출력 윈도우 핸들
	tResolution		m_tRes; // 윈도우 해상도 정보

	SCENE_STATE		m_eState;

	void(CRenderMgr::*render)(void);
	void(CSceneMgr::*scene_progress)(void);
	
	
public:
	int Init(HWND _hWnd, bool _bWindowed);
	void Progress();

	void SetRenderMode(bool _bTool);
	void Play(bool _bTrue);
	void LoadScene();

	SCENE_STATE GetState() { return m_eState; }
	void SetState(SCENE_STATE _eState) { m_eState = _eState; }

public:
	HWND GetWindowHWND() { return m_hWnd; }
	tResolution GetRes() { return m_tRes; }

private:
	void LoadResource(wstring _strFolderPath);

};

