#pragma once

class CSamplerState;
class CBlendState;

class CRenderMgr
{
	SINGLE(CRenderMgr);

private:
	CSamplerState * m_arrSampler[(UINT)SAMPLER_TYPE::END];
	CBlendState * m_arrBlendState[(UINT)BLEND_TYPE::END];

public:
	void Init();
	void render();
	void render_tool();


public:
	CBlendState* GetBlendState(BLEND_TYPE _eType) { return m_arrBlendState[(UINT)_eType]; }

private:
	void CreateSamplerState();
	void CreateBlendState();

};

