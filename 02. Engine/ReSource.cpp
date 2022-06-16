#include "stdafx.h"
#include "ReSource.h"

CReSource::CReSource(RES_TYPE _etype)
	:m_eType(_etype)
{
}

CReSource::~CReSource()
{
}

void CReSource::SaveToScene(FILE * _pFile)
{
	SaveWString(GetName().c_str(), _pFile);

	SaveWString(m_strPath.c_str(), _pFile);
}

bool CReSource::LoadFromScene(FILE * _pFile)
{
	SetName(LoadWString(_pFile));

	m_strPath = LoadWString(_pFile);

	return true;
}
