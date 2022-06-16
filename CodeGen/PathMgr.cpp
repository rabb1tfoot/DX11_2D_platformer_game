#include "pch.h"
#include "PathMgr.h"

wchar_t CPathMgr::g_szPath[255] = {};
wchar_t CPathMgr::g_szExt[20] = {};
wchar_t CPathMgr::g_szRelativePath[255] = {};
wchar_t CPathMgr::g_szSolutionPath[255] = {};

void CPathMgr::init()
{
	// 절대경로 찾아두기
	GetCurrentDirectory(255, g_szPath);
	
	int iLen = wcslen(g_szPath);

	int i = iLen - 1;
	for (; 0 < i; --i)
	{
		if ('\\' == g_szPath[i])
		{
			wmemset(g_szPath + (i + 1), 0, iLen - (i + 1));
			break;
		}
	}

	// SolutionPath 구해놓기
	wcscpy_s(g_szSolutionPath, 255, g_szPath);

	// Resources 폴더 경로
	wcscat_s(g_szPath, L"bin\\content\\");
}

const wchar_t * CPathMgr::GetExt(const wchar_t * _pPath)
{
	_wsplitpath_s(_pPath, nullptr, 0, nullptr, 0, nullptr, 0, g_szExt, 20);
	return g_szExt;
}

const wchar_t * CPathMgr::GetRelativePath(const wchar_t * _pPath)
{
	int iLen = wcslen(g_szPath);
	int iMaxLen = wcslen(_pPath);

	wstring str = _pPath;

	wcscpy_s(g_szRelativePath, str.substr(iLen, iMaxLen - iLen).c_str());
	wcscat_s(g_szRelativePath, L"\\");

	return g_szRelativePath;
}

const wchar_t * CPathMgr::GetSolutionPath()
{
	return g_szSolutionPath;
}
