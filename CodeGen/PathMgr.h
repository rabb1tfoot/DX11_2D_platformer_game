#pragma once

class CPathMgr
{
private:
	static wchar_t g_szPath[255];
	static wchar_t g_szExt[20];
	static wchar_t g_szRelativePath[255];
	static wchar_t g_szSolutionPath[255];

public:
	static void init();

	static const wchar_t* GetResPath() {
		return g_szPath;
	}
	static const wchar_t* GetExt(const wchar_t* _pPath);
	static const wchar_t* GetRelativePath(const wchar_t* _pPath);
	static const wchar_t* GetSolutionPath();
};

