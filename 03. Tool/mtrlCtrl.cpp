#include "stdafx.h"
#include "mtrlCtrl.h"

int g_int_id = 0;

map<UINT, SHADER_PARAM> g_mapIDCtrl;

void tIntCtrl::Create(CFont* _pFont, CString _strName, UINT _iHeight, CWnd* _pParent, SHADER_PARAM _eType)
{
	text.Create(_strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10, _iHeight + 10, 10 + STATIC_WIDTH, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	text.SetFont(_pFont);

	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_INT, _iHeight + 10 + CONTROL_HEIGHT)
		, _pParent, g_int_id);

	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));
}

void tIntCtrl::SetHeight(UINT _iHeight)
{
	CRect rt;
	text.GetClientRect(rt);
	text.ClientToScreen(rt);
	text.GetParent()->ScreenToClient(rt);

	text.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);

	edit.GetClientRect(rt);
	edit.ClientToScreen(rt);
	edit.GetParent()->ScreenToClient(rt);

	edit.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);
}


void tFloatCtrl::Create(CFont * _pFont, CString _strName, UINT _iHeight, CWnd * _pParent, SHADER_PARAM _eType)
{
	text.Create(_strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10, _iHeight + 10, 10 + STATIC_WIDTH, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	text.SetFont(_pFont);

	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_INT, _iHeight + 10 + CONTROL_HEIGHT)
		, _pParent, g_int_id);

	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));
}

void tFloatCtrl::SetHeight(UINT _iHeight)
{
	CRect rt;
	text.GetClientRect(rt);
	text.ClientToScreen(rt);
	text.GetParent()->ScreenToClient(rt);

	text.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);

	edit.GetClientRect(rt);
	edit.ClientToScreen(rt);
	edit.GetParent()->ScreenToClient(rt);

	edit.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);
}

void tVec2Ctrl::Create(CFont * _pFont, CString _strName, UINT _iHeight, CWnd * _pParent, SHADER_PARAM _eType)
{
	text.Create(_strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10, _iHeight + 10, 10 + STATIC_WIDTH, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	text.SetFont(_pFont);

	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit[0].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_VEC2, _iHeight + 10 + CONTROL_HEIGHT)
		, _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit[1].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH + EDIT_WIDTH_VEC2 + 10, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_VEC2 * 2 + 10
			, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));
}

void tVec2Ctrl::SetHeight(UINT _iHeight)
{
	CRect rt;
	text.GetClientRect(rt);
	text.ClientToScreen(rt);
	text.GetParent()->ScreenToClient(rt);

	text.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);

	for (UINT i = 0; i < 2; ++i)
	{
		edit[i].GetClientRect(rt);
		edit[i].ClientToScreen(rt);
		edit[i].GetParent()->ScreenToClient(rt);
		edit[i].SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);
	}
}

void tVec4Ctrl::Create(CFont * _pFont, CString _strName, UINT _iHeight, CWnd * _pParent, SHADER_PARAM _eType)
{
	text.Create(_strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10, _iHeight + 10, 10 + STATIC_WIDTH, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id++);
	text.SetFont(_pFont);

	edit[0].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_VEC4, _iHeight + 10 + CONTROL_HEIGHT)
		, _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit[1].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH + EDIT_WIDTH_VEC4 + 10, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_VEC4 * 2 + 10
			, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit[2].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH + EDIT_WIDTH_VEC4 * 2 + 10 * 2, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_VEC4 * 3 + 10 * 2
			, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit[3].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH + EDIT_WIDTH_VEC4 * 3 + 10 * 3, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_VEC4 * 4 + 10 * 3
			, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));
}

void tVec4Ctrl::SetHeight(UINT _iHeight)
{
	CRect rt;
	text.GetClientRect(rt);
	text.ClientToScreen(rt);
	text.GetParent()->ScreenToClient(rt);

	text.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);

	for (UINT i = 0; i < 4; ++i)
	{
		edit[i].GetClientRect(rt);
		edit[i].ClientToScreen(rt);
		edit[i].GetParent()->ScreenToClient(rt);
		edit[i].SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);
	}
}

SHADER_PARAM tCtrl::GetParamType(UINT _id)
{
	map<UINT, SHADER_PARAM>::iterator iter = g_mapIDCtrl.find(_id);
	if (iter == g_mapIDCtrl.end())
		assert(nullptr);

	return iter->second;
}

void tTexCtrl::Create(CFont * _pFont, CString _strName, UINT _iHeight, CWnd * _pParent, SHADER_PARAM _eType)
{
	text.Create(_strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10, _iHeight + 10, 10 + STATIC_WIDTH, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	text.SetFont(_pFont);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	edit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_INT, _iHeight + 10 + CONTROL_HEIGHT)
		, _pParent, g_int_id);
	edit.SetReadOnly(true);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));

	btn.Create(L"¤ý", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL
		, CRect(10 + STATIC_WIDTH + EDIT_WIDTH_INT + 5, _iHeight + 10, 10 + STATIC_WIDTH + EDIT_WIDTH_INT + BTN_WIDTH + 5
			, _iHeight + 10 + CONTROL_HEIGHT), _pParent, g_int_id);
	g_mapIDCtrl.insert(make_pair(g_int_id++, _eType));
}

void tTexCtrl::SetHeight(UINT _iHeight)
{
	CRect rt;
	text.GetClientRect(rt);
	text.ClientToScreen(rt);
	text.GetParent()->ScreenToClient(rt);
	text.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);
	
	edit.GetClientRect(rt);
	edit.ClientToScreen(rt);
	edit.GetParent()->ScreenToClient(rt);
	edit.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);

	btn.GetClientRect(rt);
	btn.ClientToScreen(rt);
	btn.GetParent()->ScreenToClient(rt);
	btn.SetWindowPos(nullptr, rt.left, _iHeight + 10, rt.Width(), rt.Height(), 0);
}
