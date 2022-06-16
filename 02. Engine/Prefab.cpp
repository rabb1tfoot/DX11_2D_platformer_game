#include "stdafx.h"
#include "Prefab.h"

#include "GameObj.h"


CPrefab::CPrefab(RES_TYPE _etype, CGameObj * _pProtoObj)
	: CReSource(_etype)
	, m_pProto(_pProtoObj)
{
	assert(m_pProto);
}

CPrefab::CPrefab(RES_TYPE _etype)
	:CReSource(_etype)
{
}

CPrefab::~CPrefab()
{
	SAFE_DELETE(m_pProto);
}


CGameObj * CPrefab::Instantiate()
{
	return m_pProto->Clone();
}