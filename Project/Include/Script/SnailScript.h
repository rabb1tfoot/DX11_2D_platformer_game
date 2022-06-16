#pragma once
#include <Script.h>
class CSnailScript :
	public CScript
{
	enum class RotType
	{
		TOP,
		RIGHT,
		DOWN,
		LEFT,
		END,
	};

private:
	RotType m_eRotType;

public:
	virtual void Update();
	virtual void OnCollisionExit(CCollider2D* _pOther);

public:
	CLONE(CSnailScript);
	CSnailScript();
	virtual ~CSnailScript();
};

