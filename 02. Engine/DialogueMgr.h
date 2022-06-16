#pragma once
class CDialogueMgr
{
	SINGLE(CDialogueMgr);

private:
	LPCWSTR  m_str;
	Vec2	 m_vLoc;

public:
	void Init();
	void SetString(const wchar_t* _wstring) { m_str = (LPCWSTR)_wstring; }
	void SetLoc(Vec2 _vec2) { m_vLoc = _vec2; }

	Vec2 GetLoc() { return m_vLoc; }
	LPCWSTR GetString() { return m_str; }
};

