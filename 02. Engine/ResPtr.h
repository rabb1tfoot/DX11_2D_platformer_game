#pragma once

#include "ReSource.h"

template<typename T>
class CResPtr
{
private:
	T * m_pRes;

public:
	T* operator ->()
	{
		return m_pRes;
	}

	void operator = (T* _pRes)
	{
		if (nullptr != m_pRes)
			m_pRes->SubRef();

		m_pRes = _pRes;

		if (nullptr != m_pRes)
			m_pRes->AddRef();
	}

	void operator = (CResPtr<T> _pResPtr)
	{
		if (nullptr != m_pRes)
			m_pRes->SubRef();

		m_pRes = _pResPtr.m_pRes;

		if (nullptr != m_pRes)
			m_pRes->AddRef();
	}

	bool operator == (void* _pVoid)
	{
		return _pVoid == m_pRes;
	}

	bool operator != (void* _pVoid)
	{
		return _pVoid != m_pRes;
	}

	bool operator == (const CResPtr<T>& _other)
	{
		return m_pRes == _other.m_pRes;
	}

	bool operator != (const CResPtr<T>& _other)
	{
		return m_pRes != _other.m_pRes;
	}

private:
	CReSource * GetPointer() { return (CReSource*)m_pRes; };

public:
	CResPtr() : m_pRes(nullptr)
	{}

	CResPtr(T* _pRes)
		: m_pRes(_pRes)
	{
		if (nullptr != m_pRes)
			m_pRes->AddRef();
	}

	CResPtr(const CResPtr<T>& _resptr)
		: m_pRes(_resptr.m_pRes)
	{
		if (nullptr != m_pRes)
			m_pRes->AddRef();
	}

	~CResPtr()
	{
		if (nullptr != m_pRes)
			m_pRes->SubRef();
	}

	friend class CResMgr;

	template<typename T>
	friend bool operator == (void*, const CResPtr<T>&);

	template<typename T>
	friend bool operator != (void*, const CResPtr<T>&);
};

template<typename T>
bool operator == (void* _pVoid, const CResPtr<T>& _pPtr)
{
	return _pVoid == _pPtr.m_pRes;
}

template<typename T>
bool operator != (void* _pVoid, const CResPtr<T>& _pPtr)
{
	return _pVoid != _pPtr.m_pRes;
}