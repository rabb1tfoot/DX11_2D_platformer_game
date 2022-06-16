#pragma once
class CEntity
{
private:
	static UINT g_iID;

private:
	wstring m_strName;
	UINT m_iID;

public:
	const wstring& GetName() { return m_strName; }
	void SetName(const wstring& _name) { m_strName = _name; }

	UINT GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity & operator = (const CEntity& _other)
	{
		m_strName = _other.m_strName;
		return *this;

	}

public:
	CEntity();
	CEntity(const CEntity& _other);
	~CEntity();
};

