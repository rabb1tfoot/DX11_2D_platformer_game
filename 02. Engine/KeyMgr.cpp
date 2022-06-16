#include "stdafx.h"
#include "KeyMgr.h"

#include "Core.h"

int arrVK[(UINT)KEY_TYPE::KEY_END] =
{
	VK_UP,
	VK_DOWN,
	VK_RIGHT,
	VK_LEFT,
	VK_SPACE,
	'W',
	'A',
	'S',
	'D',
	'F',
	'G',
	VK_LBUTTON,
	VK_RBUTTON,
	'1',
	'2',
	'3',
};

CKeyMgr::CKeyMgr()
{

}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Init()
{
	for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
	{
		m_vecKey.push_back(tKeyState{ KEY_STATE::STATE_NONE, false });
	}
}

void CKeyMgr::Update()
{
	HWND hActive = GetActiveWindow();

	//윈도우가 비활성화 된 경우
	if (nullptr == hActive)
	{
		for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
		{
			m_vecKey[i].eState = KEY_STATE::STATE_NONE;
			m_vecKey[i].bPrevPress = false;
		}
		return;
	}

	//윈도우가 활성화된경우
	for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
	{
		if (GetAsyncKeyState(arrVK[i]) & 0x8000)
		{
			if (false == m_vecKey[i].bPrevPress)
			{
				m_vecKey[i].bPrevPress = true;
				m_vecKey[i].eState = KEY_STATE::STATE_ENTER;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_HOLD;
			}
		}
		else
		{
			if (true == m_vecKey[i].bPrevPress)
			{
				m_vecKey[i].bPrevPress = false;
				m_vecKey[i].eState = KEY_STATE::STATE_AWAY;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_NONE;
			}
		}
	}

	m_pPrevMousePos = m_pMousePos;
	GetCursorPos(&m_pMousePos);
	ScreenToClient(CCore::GetInst()->GetWindowHWND(), &m_pMousePos);
}