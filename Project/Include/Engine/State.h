#pragma once
#include "Entity.h"
class CState :
	public CEntity
{
public:
	virtual void UpdateData() = 0;

public:
	CState();
	virtual ~CState();
};

