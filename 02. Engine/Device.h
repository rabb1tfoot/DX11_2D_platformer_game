#pragma once

class CConstBuffer;

class CDevice
{
	SINGLE(CDevice);

private:
	ID3D11Device*			m_pDevice;				// ��ġ �޸� ����
	ID3D11DeviceContext*	m_pContext;				// �׷��Ƚ� ������ ���� �� ������ 
	
	IDXGISwapChain*			m_pSwapChain;			// SwapChain ��� ���
	ID3D11RenderTargetView* m_pRenderTargetView;	// RenderTarget View(Texture �� SwapChain ���� ����)

	ID3D11Texture2D*		m_pDepthStencilTex;		// DepthStencil �뵵 Texture
	ID3D11DepthStencilView* m_pDepthStencilView;	// DepthStencilView
	ID3D11RasterizerState*  m_pRasterizerState;

	map<wstring, CConstBuffer*> m_mapConstBuffer;

	UINT					m_iQuality;

	////////////////////////////////
	//2d�� ���� ����
	///////////////////////////////
	ID3D11Texture2D*		m_pOffscreenTexture;    //2d�ؽ���

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
		// ���� �ʱ�ȭ ���� �ִ밪 1�� �ʱ�ȭ
		// ���̸� ����ϱ� ���ؼ��� ��ü�� ���̰� �� ������� �ϱ� ������,
		// �׷����� ���� �ʱ�ȭ�� �ִ� ������ ���� �� �д�.
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

