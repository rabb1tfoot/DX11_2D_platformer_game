#include "stdafx.h"
#include "HilightScript.h"
#include "TimeMgr.h"

CHilightScript::CHilightScript()
	: CScript((UINT)SCRIPT_TYPE::HILIGHTSCRIPT)
	, m_iClearEffOn(0)
	, m_fClearColor(0.f)
	, m_fAccTime(0.f)
	, m_fdurTime(0.f)
	, m_bHittedOn(false)
{
}


CHilightScript::~CHilightScript()
{
}


void CHilightScript::Update()
{
	if (m_bHittedOn)
	{
		m_fdurTime += DT;
		//1초가 지나면 종료
		if (m_fdurTime > 1.f)
		{
			EffectHittedOff();
			m_fdurTime = 0.f;
			m_iClearEffOn = 0;
			m_fClearColor = 1.f;
		}
		else
		{
			m_iClearEffOn = 1;

			//0.3초마다 깜빡거린다.
			if ((UINT)(m_fAccTime * 10) % 3 == 0)
			{
				m_fAccTime += DT;
				m_fClearColor = 0.8f;
			}
			else
			{
				m_fAccTime += DT;
				m_fClearColor = 0.5f;
			}
		}
		Meshrender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &m_iClearEffOn);
		Meshrender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fClearColor);
	}
}
