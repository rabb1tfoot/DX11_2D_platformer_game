#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "SceneMgr.h"
#include "ConstBuffer.h"
#include "samplerState.h"
#include "BlendState.h"

tGlobalValue g_global = {};

CRenderMgr::CRenderMgr()
	: m_arrSampler{}
	, m_arrBlendState{}
{
}


CRenderMgr::~CRenderMgr()
{
	Safe_Delete_Array(m_arrSampler);
	Safe_Delete_Array(m_arrBlendState);
}

void CRenderMgr::Init()
{
	CreateSamplerState();
	CreateBlendState();
}

void CRenderMgr::render()
{
	// 장치 색상 초기화
	float arrCol[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->Clear(arrCol);

	{
		static CConstBuffer* pGlobal = CDevice::GetInst()->FindConstBuffer(L"Global");
		pGlobal->AddData(&g_global, sizeof(tGlobalValue));
		pGlobal->UpdateData();
		pGlobal->SetRegisterAll();

		CSceneMgr::GetInst()->Render();
	}

	//2D렌더링
	CDevice::GetInst()->Render();

	// 윈도우에 출력
	CDevice::GetInst()->Present();
}

void CRenderMgr::render_tool()
{
	// 장치 색상 초기화
	float arrCol[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->Clear(arrCol);
}

void CRenderMgr::CreateSamplerState()
{
	D3D11_SAMPLER_DESC tDesc = {};

	m_arrSampler[(UINT)SAMPLER_TYPE::DEFAULT] = new CSamplerState;

	tDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	m_arrSampler[(UINT)SAMPLER_TYPE::DEFAULT]->Create(&tDesc, 0);
	m_arrSampler[(UINT)SAMPLER_TYPE::DEFAULT]->UpdateData();
}

void CRenderMgr::CreateBlendState()
{
	D3D11_BLEND_DESC tDesc = {};

	// ====================
	// Default Blend State
	// ====================
	m_arrBlendState[(UINT)BLEND_TYPE::DEFAULT] = new CBlendState;

	// =================
	// Alpha Blend State
	// =================
	m_arrBlendState[(UINT)BLEND_TYPE::ALPHABLEND] = new CBlendState;

	tDesc.AlphaToCoverageEnable = true;   // 투명물체 깊의에 의해서 가려지는 현상 제거(x4 멀티샘플이 지원될 경우)
	tDesc.IndependentBlendEnable = false; // 렌더타켓 별로 독립적인 블렌드 공식 사용

	tDesc.RenderTarget[0].BlendEnable = true;

	// 색상 혼합 공식
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	// Alpha 값 끼리 혼합공식
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_arrBlendState[(UINT)BLEND_TYPE::ALPHABLEND]->Create(&tDesc);
}
