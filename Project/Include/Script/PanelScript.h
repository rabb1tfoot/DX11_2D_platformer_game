#pragma once
#include "Script.h"
class CPanelScript :
	public CScript
{
private:

public:
	virtual void Update();

private:
	CLONE(CPanelScript);

public:
	CPanelScript();
	virtual ~CPanelScript();
};

