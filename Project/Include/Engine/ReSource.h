#pragma once
#include "Entity.h"
class CReSource :
	public CEntity
{
private:
	wstring		m_strPath;	// 상대경로
	int			m_iRefCount;// 참조 횟수
	RES_TYPE	m_eType;

public:
	void SetPath(const wstring& _strPath) { m_strPath = _strPath; }
	const wstring& GetPath() { return m_strPath; }
	RES_TYPE GetType() { return m_eType; }

public:
	virtual void Load(const wstring& _strFilePath) {};
	virtual void Save() {};

	virtual void SaveToScene(FILE* _pFile);
	virtual bool LoadFromScene(FILE* _pFile);

private:
	void AddRef() { ++m_iRefCount; }
	void SubRef() { --m_iRefCount; }

public:
	CLONE_DISABLE(CReSource);
public:
	CReSource(RES_TYPE _etype);
	virtual ~CReSource();

	template<typename T> friend class CResPtr;
};

