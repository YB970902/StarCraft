#include "stdafx.h"
#include "GameRoot.h"
#include "GameObject.h"

void GameRoot::Init()
{
	InitializeContext();

	CreateImage((LPWSTR)TEXT("Images/Marine.png"));

	TIME->Init(60);

	mPlayer1 = new GameObject();
	mPlayer1->Init();
	mPlayer1->SetPosition({ 100, 100 });
	mPlayer2 = new GameObject();
	mPlayer2->Init();
	mPlayer2->SetPosition({ 300, 100 });
}

void GameRoot::Release()
{
	mpD2DFactory->Release();
	mpD2DDevice->Release();
	mpD2DContext->Release();
	mpD2DTargetBitmap->Release();

	mpD3DDevice->Release();
	mpD3DContext->Release();
	mpDxgiDevice->Release();
	mpSwapChain->Release();

	mpDWriteFactory->Release();

	SAFE_RELEASE(mPlayer1);
	SAFE_RELEASE(mPlayer2);
}

void GameRoot::UpdateAndRender()
{
	if (TIME->IsUpdateTime())
	{
		INPUT->Update();

		Update();
		Render();
	}
}

void GameRoot::InitializeContext()
{
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory2),
		&options,
		(void**)&mpD2DFactory
	);

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		(IUnknown**)&mpDWriteFactory
	);

	CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&mpWicFactory)
	);

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	D3D_FEATURE_LEVEL featureLevelsSupported;

	ID3D11DeviceContext* pContext = nullptr;

	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&mpD3DDevice,
		&m_d3dFeatureLevel,
		&pContext
	);

	pContext->QueryInterface(__uuidof(ID3D11DeviceContext3), (void**)&mpD3DContext);

	mpD3DDevice->QueryInterface(__uuidof(IDXGIDevice3), (void**)&mpDxgiDevice);

	mpD2DFactory->CreateDevice(mpDxgiDevice, &mpD2DDevice);
	mpD2DDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&mpD2DContext
	);

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Flags = 0;

	IDXGIAdapter* pDxgiAdapter = nullptr;
	mpDxgiDevice->GetAdapter(&pDxgiAdapter);

	IDXGIFactory2* pDxgiFactory = nullptr;
	pDxgiAdapter->GetParent(IID_PPV_ARGS(&pDxgiFactory));

	pDxgiFactory->CreateSwapChainForHwnd(
		mpDxgiDevice,
		g_hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&mpSwapChain
	);

	mpDxgiDevice->SetMaximumFrameLatency(1);

	ID3D11Texture2D* pBackBuffer = nullptr;
	mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
		);

	IDXGISurface* pDxgiBackBuffer;
	mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));

	mpD2DContext->CreateBitmapFromDxgiSurface(
		pDxgiBackBuffer,
		&bitmapProperties,
		&mpD2DTargetBitmap
	);

	mpD2DContext->SetTarget(mpD2DTargetBitmap);
}

void GameRoot::CreateImage(LPWSTR fileName)
{
	// 이미지 읽기

	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pConverter = nullptr;

	mpWicFactory->CreateDecoderFromFilename(
		fileName,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder
	);

	pDecoder->GetFrame(0, &pFrame);

	mpWicFactory->CreateFormatConverter(&pConverter);
	pConverter->Initialize(
		pFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	mpD2DContext->CreateBitmapFromWicBitmap(pConverter, NULL, &mpBitmap);
}

void GameRoot::Update()
{
	if (INPUT->IsStayKeyDown(VK_LBUTTON))
	{
		mPlayer1->MoveTo(POINTFLOAT{ (float)INPUT->GetMousePosition().x, (float)INPUT->GetMousePosition().y });
	}
	mPlayer1->Update();
	mPlayer2->Update();
}

void GameRoot::Render()
{
	mpD2DContext->BeginDraw();
	mpD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
	
	mpD2DContext->DrawBitmap(mpBitmap);

	mpD2DContext->EndDraw();

	mpSwapChain->Present(1, 0);
}
