#pragma once

#include <PathMgr.h>

#define STATIC_WIDTH 100
#define CONTROL_HEIGHT 25

#define EDIT_WIDTH_INT 150
#define EDIT_WIDTH_VEC2 80
#define EDIT_WIDTH_VEC4 40
#define BTN_WIDTH 28

extern int g_int_id;

class CMaterialDlg;

struct tCtrl
{
	CStatic text;

public:
	void SetText(const CString& _strName)
	{
		text.SetWindowTextW(_strName);
	}
	virtual void ShowCtrl(bool _bTrue)
	{
		text.ShowWindow(_bTrue);
	}

	static SHADER_PARAM GetParamType(UINT _id);
};

struct tIntCtrl : public tCtrl
{
	CEdit   edit;

public:
	void Create(CFont* _pFont, CString _strName, UINT _iHeight, CWnd* _pParent, SHADER_PARAM _etype);
	void SetHeight(UINT _iHeight);

	virtual void ShowCtrl(bool _bTrue)
	{
		tCtrl::ShowCtrl(_bTrue);
		edit.ShowWindow(_bTrue);
	}
	int GetData()
	{
		CString str;
		edit.GetWindowTextW(str);
		return _wtoi(str);
	}
};

struct tFloatCtrl : public tCtrl
{
	CEdit   edit;

	void Create(CFont* _pFont, CString _strName, UINT _iHeight, CWnd* _pParent, SHADER_PARAM _eType);
	void SetHeight(UINT _iHeight);
	virtual void ShowCtrl(bool _bTrue)
	{
		tCtrl::ShowCtrl(_bTrue);
		edit.ShowWindow(_bTrue);
	}

	float GetData()
	{
		CString str;
		edit.GetWindowTextW(str);
		return _wtof(str);
	}
};

struct tVec2Ctrl : public tCtrl
{
	CEdit   edit[2];

	void Create(CFont* _pFont, CString _strName, UINT _iHeight, CWnd* _pParent, SHADER_PARAM _eType);
	void SetHeight(UINT _iHeight);
	virtual void ShowCtrl(bool _bTrue)
	{
		tCtrl::ShowCtrl(_bTrue);
		edit[0].ShowWindow(_bTrue);
		edit[1].ShowWindow(_bTrue);
	}
	Vec2 GetData()
	{
		Vec2 vRet;
		CString str[2];
		edit[0].GetWindowTextW(str[0]);
		edit[1].GetWindowTextW(str[1]);

		vRet.x = _wtof(str[0]);
		vRet.y = _wtof(str[1]);

		return vRet;
	}


};

struct tVec4Ctrl : public tCtrl
{
	CEdit   edit[4];

	void Create(CFont* _pFont, CString _strName, UINT _iHeight, CWnd* _pParent, SHADER_PARAM _eType);
	void SetHeight(UINT _iHeight);
	virtual void ShowCtrl(bool _bTrue)
	{
		tCtrl::ShowCtrl(_bTrue);
		edit[0].ShowWindow(_bTrue);
		edit[1].ShowWindow(_bTrue);
		edit[2].ShowWindow(_bTrue);
		edit[3].ShowWindow(_bTrue);
	}
	Vec4 GetData()
	{
		Vec4 vRet;
		CString str[4];
		edit[0].GetWindowTextW(str[0]);
		edit[1].GetWindowTextW(str[1]);
		edit[2].GetWindowTextW(str[2]);
		edit[3].GetWindowTextW(str[3]);

		vRet.x = _wtof(str[0]);
		vRet.y = _wtof(str[1]);
		vRet.z = _wtof(str[2]);
		vRet.w = _wtof(str[3]);

		return vRet;
	}
};

struct tTexCtrl : public tCtrl
{
	CEdit edit; // 택스처 이름
	CButton btn;

	void Create(CFont* _pFont, CString _strName, UINT _iHeight, CWnd* _pParent, SHADER_PARAM _eType);
	void SetHeight(UINT _iHeight);
	virtual void ShowCtrl(bool _bTrue)
	{
		tCtrl::ShowCtrl(_bTrue);
		edit.ShowWindow(_bTrue);
		btn.ShowWindow(_bTrue);
	}
	void SetData(const CString& _strTextName)
	{
		CString strTexName = CPathMgr::GetFileName(_strTextName);
		edit.SetWindowTextW(strTexName);
	}
	CString GetData()
	{
		CString str;
		edit.GetWindowTextW(str);
		return str;
	}

};

struct tMatCtrl : public tCtrl
{
	//CEdit   edit[16];

	//void Create(CFont* _pFont, CString _strName, UINT _iHeight/*부모 윈도우에서 출발할 기준높이*/, CWnd* _pParent, SHADER_PARAM _eType);
	//void SetHeight(UINT _iHeight);
	//virtual void ShowCtrl(bool _bTrue)
	//{
	//	tCtrl::ShowCtrl(_bTrue);
	//	edit[0].ShowWindow(_bTrue);
	//	edit[1].ShowWindow(_bTrue);
	//	edit[2].ShowWindow(_bTrue);
	//	edit[3].ShowWindow(_bTrue);
	//	edit[4].ShowWindow(_bTrue);
	//	edit[5].ShowWindow(_bTrue);
	//	edit[6].ShowWindow(_bTrue);
	//	edit[7].ShowWindow(_bTrue);
	//	edit[8].ShowWindow(_bTrue);
	//	edit[9].ShowWindow(_bTrue);
	//	edit[10].ShowWindow(_bTrue);
	//	edit[11].ShowWindow(_bTrue);
	//	edit[12].ShowWindow(_bTrue);
	//	edit[13].ShowWindow(_bTrue);
	//	edit[14].ShowWindow(_bTrue);
	//	edit[15].ShowWindow(_bTrue);
	//}
	//Vec4 GetData()
	//{
	//	 vRet;
	//	CString str[4];
	//	edit[0].GetWindowTextW(str[0]);
	//	edit[1].GetWindowTextW(str[1]);
	//	edit[2].GetWindowTextW(str[2]);
	//	edit[3].GetWindowTextW(str[3]);
	//	edit[4].GetWindowTextW(str[4]);
	//	edit[5].GetWindowTextW(str[5]);
	//	edit[6].GetWindowTextW(str[6]);
	//	edit[7].GetWindowTextW(str[7]);
	//	edit[8].GetWindowTextW(str[8]);
	//	edit[9].GetWindowTextW(str[9]);
	//	edit[10].GetWindowTextW(str[10]);
	//	edit[11].GetWindowTextW(str[11]);
	//	edit[12].GetWindowTextW(str[12]);
	//	edit[13].GetWindowTextW(str[13]);
	//	edit[14].GetWindowTextW(str[14]);
	//	edit[15].GetWindowTextW(str[15]);
	//
	//
	//	vRet.x = _wtof(str[0]);
	//	vRet.y = _wtof(str[1]);
	//	vRet.z = _wtof(str[2]);
	//	vRet.w = _wtof(str[3]);
	//
	//	return vRet;
	//}
};