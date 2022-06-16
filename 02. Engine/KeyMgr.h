#pragma once

enum class KEY_TYPE
{
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_SPACE,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	MOUSE_LBTN,
	MOUSE_RBTN,
	KEY_1,
	KEY_2,
	KEY_3,

	KEY_END
};

enum class KEY_STATE
{
	STATE_NONE,
	STATE_ENTER,
	STATE_HOLD,
	STATE_AWAY,
};

struct tKeyState
{
	KEY_STATE eState;
	bool bPrevPress;
};


class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyState> m_vecKey;
	POINT m_pMousePos;
	POINT m_pPrevMousePos;

public:
	void Init();
	void Update();

public:
	KEY_STATE GetKeyState(KEY_TYPE _etype) { return m_vecKey[(UINT)_etype].eState; }
	const POINT& GetMousePos() { return m_pMousePos; }
	const POINT& GetPrevMousePos() { return m_pPrevMousePos; }

};

