#pragma once

class CConstBuffer;

class CDevice
{
	SINGLE(CDevice);

private:
	ID3D11Device*			m_pDevice;				// 장치 메모리 관리
	ID3D11DeviceContext*	m_pContext;				// 그래픽스 파이프 라인 및 렌더링 
	
	IDXGISwapChain*			m_pSwapChain;			// SwapChain 기능 담당
	ID3D11RenderTargetView* m_pRenderTargetView;	// RenderTarget View(Texture 는 SwapChain 에서 지정)

	ID3D11Texture2D*		m_pDepthStencilTex;		// DepthStencil 용도 Texture
	ID3D11DepthStencilView* m_pDepthStencilView;	// DepthStencilView
	ID3D11RasterizerState*  m_pRasterizerState;

	map<wstring, CConstBuffer*> m_mapConstBuffer;

	UINT					m_iQuality;

	////////////////////////////////
	//2d를 위한 변수
	///////////////////////////////
	ID3D11Texture2D*		m_pOffscreenTexture;    //2d텍스쳐

	IDXGISurface*				m_pBackBuffer;
	ID2D1Factory*				m_pFactory2D;
	ID2D1RenderTarget*			m_pBackBufferRT;
	ID2D1LinearGradientBrush*	m_pBackBufferGradientBrush;
	ID2D1GradientStopCollection*pGradientStops;
	IDWriteFactory*				m_pWriteFactory;
	IDWriteTextFormat*			m_pTextFormat;
	ID2D1SolidColorBrush*		m_pBrush;


	tResolution				m_tRes;

public:
	int Init(HWND _hWnd, const tResolution& _tRes, bool _bWindowed);
	int Init2D(HWND _hWnd, const tResolution& _tRes, bool _bWindowed);

	void Clear( float(&_fColor)[4] )
	{	
		m_pContext->ClearRenderTargetView(m_pRenderTargetView, _fColor);
		m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		// 깊이 초기화 값은 최대값 1로 초기화
		// 깊이를 통과하기 위해서는 물체의 깊이가 더 가까워야 하기 때문에,
		// 그려지기 전에 초기화를 최대 깊으로 설정 해 둔다.
	}

	void Present()
	{
		m_pSwapChain->Present(0, 0);
	}

	const tResolution& GetRes() { return m_tRes; }
	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pContext; }	

	void CreateConstBuffer(const wstring& _strKey, UINT _iSize, UINT _iRegister);
	CConstBuffer* FindConstBuffer(const wstring& _strKey);

	void Render();

};

