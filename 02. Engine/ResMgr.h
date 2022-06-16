#pragma once

#include "Resource.h"

#include "Mesh.h"
#include "Sound.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "ResPtr.h"
#include "Prefab.h"
#include "PathMgr.h"

class CResMgr
{
	SINGLE(CResMgr);

private:
	map<wstring, CReSource*> m_mapRes[(UINT)RES_TYPE::END];

	vector<CReSource*> m_vecCloneRes[(UINT)RES_TYPE::END];

public:
	void Init();

private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void InitSound();

public:
	const map<wstring, CReSource*>& GetResources(RES_TYPE _etype) { return m_mapRes[(UINT)_etype]; }

	template<typename T>
	CResPtr<T> FindRes(const wstring& _strKey);

	template<typename T>
	void AddRes(const wstring& _strKey, CResPtr<T> _pResource);

	void AddRes(RES_TYPE _eType, CReSource* _pResource);

	template<typename T>
	void AddCloneRes(CResPtr<T> _pResource);

	template<typename T>
	CResPtr<T> Load(const wstring& _strKey, const wstring& _strPath);
	
};

template<typename T>
inline CResPtr<T> CResMgr::FindRes(const wstring & _strKey)
{
	map<wstring, CReSource*>::iterator iter;

	RES_TYPE eType = RES_TYPE::END;

	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		eType = RES_TYPE::MESH;
	else if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		eType = RES_TYPE::TEXTURE;
	else if (typeid(T).hash_code() == typeid(CShader).hash_code())
		eType = RES_TYPE::SHADER;
	else if (typeid(T).hash_code() == typeid(CSound).hash_code())
		eType = RES_TYPE::SOUND;
	else if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		eType = RES_TYPE::MATERIAL;
	else if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		eType = RES_TYPE::PREFAB;

	iter = m_mapRes[(UINT)eType].find(_strKey);
	if (iter == m_mapRes[(UINT)eType].end())
		return nullptr;
	else
		return CResPtr<T>((T*)(iter->second));
}

template<typename T>
inline void CResMgr::AddRes(const wstring & _strKey, CResPtr<T> _pResource)
{
	map<wstring, CReSource*>::iterator iter;

	RES_TYPE eType = RES_TYPE::END;

	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		eType = RES_TYPE::MESH;
	else if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		eType = RES_TYPE::TEXTURE;
	else if (typeid(T).hash_code() == typeid(CShader).hash_code())
		eType = RES_TYPE::SHADER;
	else if (typeid(T).hash_code() == typeid(CSound).hash_code())
		eType = RES_TYPE::SOUND;
	else if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		eType = RES_TYPE::MATERIAL;
	else if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		eType = RES_TYPE::PREFAB;

	iter = m_mapRes[(UINT)eType].find(_strKey);
	if (iter != m_mapRes[(UINT)eType].end())
	{
		return;
	}

	m_mapRes[(UINT)eType].insert(make_pair(_strKey, _pResource.GetPointer()));
	_pResource->SetName(_strKey);
}

template<typename T>
inline void CResMgr::AddCloneRes(CResPtr<T> _pResource)
{
	RES_TYPE eType = RES_TYPE::END;

	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		eType = RES_TYPE::MESH;
	else if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		eType = RES_TYPE::TEXTURE;
	else if (typeid(T).hash_code() == typeid(CShader).hash_code())
		eType = RES_TYPE::SHADER;
	else if (typeid(T).hash_code() == typeid(CSound).hash_code())
		eType = RES_TYPE::SOUND;
	else if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		eType = RES_TYPE::MATERIAL;
	else if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		eType = RES_TYPE::PREFAB;

	m_vecCloneRes[(UINT)eType].push_back(_pResource.GetPointer());
}

template<typename T>
inline CResPtr<T> CResMgr::Load(const wstring & _strKey, const wstring & _strPath)
{
	wstring strFilePath = CPathMgr::GetResPath();
	strFilePath += _strPath;

	RES_TYPE eType = RES_TYPE::END;

	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		eType = RES_TYPE::MESH;
	else if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		eType = RES_TYPE::TEXTURE;
	else if (typeid(T).hash_code() == typeid(CShader).hash_code())
		eType = RES_TYPE::SHADER;
	else if (typeid(T).hash_code() == typeid(CSound).hash_code())
		eType = RES_TYPE::SOUND;
	else if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		eType = RES_TYPE::MATERIAL;
	else if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		eType = RES_TYPE::PREFAB;


	// 중복키 검사
	CResPtr<T> pRes = FindRes<T>(_strKey);
	if (nullptr != pRes)
	{
		// 중복키 문제
		return pRes;
	}

	T* pResource = new T(eType);
	pResource->Load(strFilePath);
	m_mapRes[(UINT)eType].insert(make_pair(_strKey, pResource));

	pResource->SetName(_strKey);
	pResource->SetPath(_strPath);

	return CResPtr<T>(pResource);
}