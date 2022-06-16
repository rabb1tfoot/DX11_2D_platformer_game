#pragma once

class CLayer;
class CGameObj;

class CSaveLoadMgr
{
	CSaveLoadMgr();
	~CSaveLoadMgr();
public:
	static void SaveScene(const wstring& _strPath);
	static void SaveResource(FILE* _pFile);
	static void SaveLayer(CLayer* _pLayer, FILE* _pFile);
	static void SaveGameObject(CGameObj* _pObject, FILE* _pFile);


	static CGameObj* LoadGameObject(FILE* _pFile);
	static void LoadScene(const wstring& _strPath);
	static void LoadResource(FILE* _pFile);
	static CLayer* LoadLayer(FILE* _pFile);
};

