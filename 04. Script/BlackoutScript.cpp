#include "stdafx.h"
#include "BlackoutScript.h"
#include <MeshRender.h>
#include <Material.h>
#include <KeyMgr.h>


CBlackoutScript::CBlackoutScript()
	: CScript((UINT)SCRIPT_TYPE::BLACKOUTSCRIPT)
	, m_bActiveFadeOff(false)
	, m_bActiveFadeIn(true)
	, m_fAccTime(0.f)
	, m_iActiveNum(0)
{
}



void CBlackoutScript::Update()
{
	if (m_bActiveFadeOff)
	{
		if (m_fAccTime < 0.5f)
		{
			m_iActiveNum = 5;
			m_fAccTime += DT;
			Object()->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &m_iActiveNum);
			Meshrender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
		}
		if (m_fAccTime > 0.5f)
		{
			m_bActiveFadeOff = false;
			m_fAccTime = 0.f;
		}
	}
	if (m_bActiveFadeIn)
	{
		if (m_fAccTime < 0.5f)
		{
			m_iActiveNum = 6;
			m_fAccTime += DT;
			Object()->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &m_iActiveNum);
			Meshrender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
		}
		if (m_fAccTime > 0.5f)
		{
			m_bActiveFadeIn = false;
			m_fAccTime = 0.f;
			m_iActiveNum = 4;
			Meshrender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_2, &m_iActiveNum);
			Meshrender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fAccTime);
		}
	}
}

CBlackoutScript::~CBlackoutScript()
{
}
