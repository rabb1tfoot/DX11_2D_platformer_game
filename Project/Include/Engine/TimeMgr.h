#pragma once
class CTimeMgr
{
	SINGLE(CTimeMgr);

private:
	LARGE_INTEGER	m_LIOldCount;
	LARGE_INTEGER	m_LICurCount;
	LARGE_INTEGER	m_LIFrequency;

	float			m_fDeltaTime;
	double			m_dAccTime;
	float			m_fFPS;

	bool			m_bFreeze;

public:
	void Init();
	void Update();

public:
	float GetDeltaTime() { return m_fDeltaTime; }
	void DTFreeze() { m_bFreeze = true; }
	float GetFPS() { return m_fFPS; }
};

