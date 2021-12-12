#include "stdafx.h"
#include "RenderManager.h"
#include "ColorReplaceEffect.h"
#include "ShadowEffect.h"
#include "SpriteComponent.h"
#include "RendererComponent.h"
#include "EffectData.h"
#include "Gizmo.h"
#include "TextGizmo.h"
#include "RectGizmo.h"
#include "LineGizmo.h"

void RenderManager::Init()
{
	InitDirect2D();
	InitBitmap();
}

void RenderManager::Release()
{
	ReleaseDirect2D();
	ReleaseGizmo();
}

void RenderManager::Render()
{
	mpD2DContext->BeginDraw();
	mpD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// 줌을 적용시킨 코드
	//mpD2DContext->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(mCameraZoom, mCameraZoom)) * D2D1::Matrix3x2F::Translation(mCameraPosition.x, mCameraPosition.y));
	mpD2DContext->SetTransform(D2D1::Matrix3x2F::Translation(CAMERA->GetD2DPosition()));

	while (mQueTerrain.empty() == false)
	{
		mQueTerrain.top().second->Render(mpD2DContext);
		mQueTerrain.pop();
	}

	while (mQueGround.empty() == false)
	{
		mQueGround.top().second->Render(mpD2DContext);
		mQueGround.pop();
	}

	while (mQueSky.empty() == false)
	{
		mQueSky.top().second->Render(mpD2DContext);
		mQueSky.pop();
	}

	for (int i = 0; i < mVecGizmo.size(); ++i)
	{
		mVecGizmo[i]->Render(mpD2DContext);
	}

	mpD2DContext->EndDraw();

	mpSwapChain->Present(1, 0);
}

ID2D1Bitmap* RenderManager::GetBitmap(eBitmapTag tag)
{
	return mMapBitmap[tag];
}

ID2D1Effect* RenderManager::CreateEffect(eEffectTag tag)
{
	switch (tag)
	{
	case eEffectTag::COLOR_REPLACE:
		return CreateColorReplaceEffect();
	case eEffectTag::SHADOW:
		return CreateShadowEffect();
	}
	return nullptr;
}

void RenderManager::AddRenderer(float posY, RendererComponent* pComponent)
{
	switch (pComponent->GetUnitLayer())
	{
	case eUnitLayer::Terrain:
		mQueTerrain.emplace(posY, pComponent);
		break;
	case eUnitLayer::Ground:
		mQueGround.emplace(posY, pComponent);
		break;
	case eUnitLayer::Sky:
		mQueSky.emplace(posY, pComponent);
		break;
	}
}

Gizmo* RenderManager::RenderText(wstring text, Vector2 pos, Vector2 size, int fontSize, D2D1::ColorF color, eTextAlign align)
{
	IDWriteTextFormat* pFormat = nullptr;
	mpDWriteFactory->CreateTextFormat(TEXT("Arial"), nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize, TEXT("ko"), &pFormat);

	pFormat->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)align);

	ID2D1SolidColorBrush* pBrush = nullptr;
	mpD2DContext->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);

	Gizmo* pResult = new TextGizmo(pFormat, text, pos, size, pBrush);
	mVecGizmo.push_back(pResult);
	return pResult;
}

Gizmo* RenderManager::RenderRect(Vector2 pos, Vector2 size, float weight, D2D1::ColorF color, Vector2 anchor)
{
	ID2D1SolidColorBrush* pBrush = nullptr;
	mpD2DContext->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);

	Gizmo* pResult = new RectGizmo(pos, size, anchor, weight, pBrush);
	mVecGizmo.push_back(pResult);
	return pResult;
}

Gizmo* RenderManager::RenderLine(Vector2 startPos, Vector2 endPos, float width, D2D1::ColorF color)
{
	ID2D1SolidColorBrush* pBrush = nullptr;
	mpD2DContext->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);

	Gizmo* pResult = new LineGizmo(startPos, endPos, width, pBrush);
	mVecGizmo.push_back(pResult);
	return pResult;
}

void RenderManager::InitDirect2D()
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

void RenderManager::InitBitmap()
{
	mMapBitmap[eBitmapTag::UNIT_MARINE_L] = CreateBitmap((LPWSTR)TEXT("Images/Units/Marine/MarineL.png"));
	mMapBitmap[eBitmapTag::UNIT_MARINE_R] = CreateBitmap((LPWSTR)TEXT("Images/Units/Marine/MarineR.png"));
	mMapBitmap[eBitmapTag::UNIT_BATTLE_L] = CreateBitmap((LPWSTR)TEXT("Images/Units/Battlecruiser/BattlecruiserL.png"));
	mMapBitmap[eBitmapTag::UNIT_BATTLE_R] = CreateBitmap((LPWSTR)TEXT("Images/Units/Battlecruiser/BattlecruiserR.png"));

	mMapBitmap[eBitmapTag::TILE_PALETTE] = CreateBitmap((LPWSTR)TEXT("Images/Tile/TilePalette.png"));
}

void RenderManager::ReleaseDirect2D()
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
}

void RenderManager::ReleaseGizmo()
{
	Gizmo* pGizmo = nullptr;
	for (auto it = mVecGizmo.begin(); it != mVecGizmo.end(); )
	{
		pGizmo = (*it);
		it = mVecGizmo.erase(it);
		delete pGizmo;
	}
}

ID2D1Bitmap* RenderManager::CreateBitmap(LPWSTR fileName)
{
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

	ID2D1Bitmap* pResult = nullptr;
	mpD2DContext->CreateBitmapFromWicBitmap(pConverter, NULL, &pResult);

	pDecoder->Release();
	pFrame->Release();
	pConverter->Release();

	return pResult;
}

ID2D1Effect* RenderManager::CreateColorReplaceEffect()
{
	ColorReplaceEffect::Register(mpD2DFactory);
	ID2D1Effect* pResult = nullptr;
	mpD2DContext->CreateEffect(GUID_ColorReplacePixelShader, &pResult);
	pResult->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR, EFFECT_COLOR_RED);
	return pResult;
}

ID2D1Effect* RenderManager::CreateShadowEffect()
{
	ShadowEffect::Register(mpD2DFactory);
	ID2D1Effect* pResult = nullptr;
	mpD2DContext->CreateEffect(GUID_ShadowPixelShader, &pResult);
	return pResult;
}