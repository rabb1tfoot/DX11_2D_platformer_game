#include "stdafx.h"
#include "TimeMgr.h"

CTimeMgr::CTimeMgr()
	: m_LICurCount{}
	, m_LIOldCount{}
	, m_LIFrequency{}
	, m_fDeltaTime(0.f)
	, m_dAccTime(0.f)
	, m_bFreeze(false)
{
}


CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_LIFrequency);
	QueryPerformanceCounter(&m_LIOldCount);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_LICurCount);
	m_fDeltaTime = (float)(m_LICurCount.QuadPart - m_LIOldCount.QuadPart) / (float)m_LIFrequency.QuadPart;
	m_LIOldCount = m_LICurCount;

	m_dAccTime += m_fDeltaTime;

	if (m_bFreeze)
	{
		m_bFreeze = false;
		m_fDeltaTime = 1.f / 60.f;
	}

	g_global.fDeltaTime = DT;
	g_global.dAccTime = m_dAccTime;
}

