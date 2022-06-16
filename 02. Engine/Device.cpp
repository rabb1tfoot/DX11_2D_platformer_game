#include "stdafx.h"
#include "Device.h"

#include "ConstBuffer.h"
#include "DialogueMgr.h"

CDevice::CDevice()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_pDepthStencilTex(nullptr)
	, m_pDepthStencilView(nullptr)
	, m_iQuality(0)
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);

	SAFE_RELEASE(m_pDepthStencilTex);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pBackBufferRT);
	SAFE_RELEASE(m_pRasterizerState);

	SAFE_RELEASE(m_pWriteFactory);
	SAFE_RELEASE(m_pTextFormat);
	SAFE_RELEASE(m_pBrush);

	Safe_Delete_Map(m_mapConstBuffer);
}

int CDevice::Init(HWND _hWnd, const tResolution& _tRes, bool _bWindowed)
{
	m_tRes = _tRes;

	int iFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT ;
#ifdef _DEBUG
	iFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL m_featureLevel;

	// Create 3D device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		iFlag,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&m_pDevice,
		&m_featureLevel,
		&m_pContext);


	// Multi Sample Level Check
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iQuality);

	// Create SwapChain
	DXGI_SWAP_CHAIN_DESC tSwapDesc = {};

	tSwapDesc.BufferCount = 1;

	tSwapDesc.BufferDesc.Width = (UINT)_tRes.fWidth;
	tSwapDesc.BufferDesc.Height = (UINT)_tRes.fHeight;
	tSwapDesc.BufferDesc.RefreshRate.Numerator = 60;
	tSwapDesc.BufferDesc.RefreshRate.Denominator = 1;
	tSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	tSwapDesc.SampleDesc.Count = 4;
	tSwapDesc.SampleDesc.Quality = 0;

	tSwapDesc.OutputWindow = _hWnd;	// 출력 윈도우
	tSwapDesc.Windowed = _bWindowed;

	tSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	tSwapDesc.Flags = 0;

	IDXGIDevice* pIDXDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIDXDevice);
	pIDXDevice->GetAdapter(&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	HRESULT hr = pFactory->CreateSwapChain(m_pDevice, &tSwapDesc, &m_pSwapChain);

	// 참조 Count 줄여주기
	SAFE_RELEASE(pIDXDevice);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	// RenderTarget View
	// SwapChain 이 가지고 있는 RenderTarget 용 Texture 를 얻어온다.
	ID3D11Texture2D* pRenderTargetTex = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pRenderTargetTex);
	SAFE_RELEASE(pRenderTargetTex); // 얻어온 ref count 하나 줄여주기

									// SwapChain 의 RenderTarget 용 Texture 를 이용해서 RenderTargetView 를 만듬
	m_pDevice->CreateRenderTargetView(pRenderTargetTex, nullptr, &m_pRenderTargetView);


	// DepthStencil Texture
	D3D11_TEXTURE2D_DESC tTexDesc = {};

	// RenderTarget 과 같은 해상도로 설정
	tTexDesc.Width = (UINT)_tRes.fWidth;
	tTexDesc.Height = (UINT)_tRes.fHeight;
	tTexDesc.MipLevels = 1;
	tTexDesc.ArraySize = 1;
	tTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tTexDesc.SampleDesc.Count = 4;
	tTexDesc.SampleDesc.Quality = 0;
	tTexDesc.Usage = D3D11_USAGE_DEFAULT;			// 메모리 사용 용도(읽기, 쓰기 관련)
	tTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;  // Texture  가 DepthStencil 용도로 사용될 것을 알림

	m_pDevice->CreateTexture2D(&tTexDesc, nullptr, &m_pDepthStencilTex);

	tTexDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	tTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;


	// DepthStencil View
	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, nullptr, &m_pDepthStencilView);

	//Resterizer state object
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;
	RSDesc.CullMode = D3D11_CULL_NONE;
	RSDesc.FrontCounterClockwise = FALSE;
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = FALSE;
	RSDesc.AntialiasedLineEnable = FALSE;

	hr = m_pDevice->CreateRasterizerState(&RSDesc, &m_pRasterizerState);



	// Output Merget State 에 RenderTagetView, DepthStencilView 전달
	// OM 단계에 전달할 수 있는 최대 RenderTargetView 개수는 dx11 기준 8개 이다.
	// 따라서 OMSetRenderTargets 함수는 RenderTargetView* 배열을 요구한다
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	//레스터라이저 스테이지에 스테이트 오브젝트 설정
	m_pContext->RSSetState(m_pRasterizerState);


	// ViewPort
	D3D11_VIEWPORT tVP = {};

	tVP.Width = (UINT)_tRes.fWidth;
	tVP.Height = (UINT)_tRes.fHeight;

	tVP.TopLeftX = 0;
	tVP.TopLeftY = 0;

	tVP.MinDepth = 0.f;
	tVP.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tVP);

	return S_OK;
}

int CDevice::Init2D(HWND _hWnd, const tResolution & _tRes, bool _bWindowed)
{
	//2d 백버퍼 만들기
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pBackBuffer));
	
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory2D);
	
	FLOAT dpiX;
	FLOAT dpiY;
	m_pFactory2D->GetDesktopDpi(&dpiX, &dpiY);
	
	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);
	
	
	hr = m_pFactory2D->CreateDxgiSurfaceRenderTarget(
		m_pBackBuffer,
		&props,
		&m_pBackBufferRT
	);
	
	SAFE_RELEASE(m_pFactory2D);
	SAFE_RELEASE(m_pBackBuffer);
	
	//write출력용 팩토리 만들기
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pWriteFactory)
	);
	m_pWriteFactory->CreateTextFormat(
		L"궁서",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"ko",
		&m_pTextFormat
	);
	//브러쉬 만들기
	m_pBackBufferRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &m_pBrush);
	
	return 0;
}

void CDevice::CreateConstBuffer(const wstring & _strKey, UINT _iSize, UINT _iRegister)
{
	map<wstring, CConstBuffer*>::iterator iter = m_mapConstBuffer.find(_strKey);
	if (iter != m_mapConstBuffer.end())
	{
		return;
	}

	CConstBuffer* pConstBuffer = new CConstBuffer;

	pConstBuffer->Create(_iSize, _iRegister);
	pConstBuffer->SetName(_strKey);

	m_mapConstBuffer.insert(make_pair(_strKey, pConstBuffer));

}

CConstBuffer * CDevice::FindConstBuffer(const wstring & _strKey)
{
	return m_mapConstBuffer.find(_strKey)->second;
}

void CDevice::Render()
{	
	LPCWSTR str = CDialogueMgr::GetInst()->GetString();
	Vec2 vLoc = CDialogueMgr::GetInst()->GetLoc();
	
	m_pBackBufferRT->BeginDraw();
	
	m_pBackBufferRT->DrawTextW(
	
		str,
	
		wcslen(str),
	
		m_pTextFormat,
	
		D2D1::RectF(0, 0, vLoc.x, vLoc.y),
	
		m_pBrush
	
	);
	
	m_pBackBufferRT->EndDraw();
}
