#pragma once
#include "ReSource.h"

class CGameObj;

class CPrefab :
	public CReSource
{
private:
	CGameObj * m_pProto;


public:
	CGameObj * Instantiate();

public:
	CPrefab(RES_TYPE _etype, CGameObj* _pProtoObj);
	CPrefab(RES_TYPE _etype);
	virtual ~CPrefab();
};

