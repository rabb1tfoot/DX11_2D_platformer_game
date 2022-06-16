#pragma once

class CStage;

class CStageMgr
{
	SINGLE(CStageMgr);

private:
	CStage * m_stage[1];
	CStage* m_currentStage;

public:
	void Init();
	void UpdateRender();
	//void Render();

public:
	CStage * GetCurrentStage() { return m_currentStage; }
};

