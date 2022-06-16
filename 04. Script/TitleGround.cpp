#include "stdafx.h"
#include "TitleGround.h"
#include <SaveLoadMgr.h>


CTitleGround::CTitleGround()
	: CScript((UINT)SCRIPT_TYPE::TITLEGROUND)
	, m_fAccTime(0.f)
{
}


CTitleGround::~CTitleGround()
{
}

void CTitleGround::Update()
{
	m_fAccTime += DT;
	int TitleShader = 2;
	Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
	Meshrender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &TitleShader);

	if (m_fAccTime >= 12.f)
		m_fAccTime = 0.f;

	if (KEYENTER(KEY_TYPE::KEY_F))
	{
		wstring wPath = CPathMgr::GetResPath();
		wPath += L"Scene\\";
		wPath += L"saveScene_1.scene";
		CSaveLoadMgr::LoadScene(wPath.c_str());
	}
}
