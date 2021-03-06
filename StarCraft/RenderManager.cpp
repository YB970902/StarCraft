#include "stdafx.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "GameObject.h"
#include "ColorReplaceEffect.h"
#include "ShadowEffect.h"
#include "SpriteComponent.h"
#include "RendererComponent.h"
#include "EffectData.h"

void RenderManager::Init()
{
	InitDirect2D();
	InitBitmap();
}

void RenderManager::Release()
{
	ReleaseDirect2D();
	ReleaseLayer();
}

void RenderManager::Render()
{
	if (mbIsInitLayer == false) { return; }
	mpD2DContext->BeginDraw();
	mpD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	mpD2DContext->SetTransform(D2D1::Matrix3x2F::Translation(CAMERA->GetD2DPosition()));

	POINT leftTop = GetDetailIndexByPoint(POINT{ CAMERA->GetCameraRect().left, CAMERA->GetCameraRect().top });
	leftTop.x -= CULLING_EXTRA_SIZE;
	leftTop.y -= CULLING_EXTRA_SIZE;
	if (leftTop.x < 0) leftTop.x = 0;
	if (leftTop.y < 0) leftTop.y = 0;
	POINT rightBottom = GetDetailIndexByPoint(POINT{ CAMERA->GetCameraRect().right, CAMERA->GetCameraRect().bottom });
	rightBottom.x += CULLING_EXTRA_SIZE;
	rightBottom.y += CULLING_EXTRA_SIZE;
	if (rightBottom.x >= mLayerWidth) rightBottom.x = mLayerWidth - 1;
	if (rightBottom.y >= mLayerHeight) rightBottom.y = mLayerHeight - 1;

	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	RenderLayer(mLayerTerrain, leftTop, width, height);
	RenderLayer(mLayerRemains, leftTop, width, height);
	RenderLayer(mLayerGround, leftTop, width, height);
	RenderLayer(mLayerParticle, leftTop, width, height);
	RenderLayer(mLayerFog, leftTop, width, height);

	UI->Render(mpD2DContext);

	mpD2DContext->EndDraw();

	mpSwapChain->Present(1, 0);
}

void RenderManager::InitDefaultLayer()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	InitLayerSize(rc.right - rc.left, rc.bottom - rc.top);
}

void RenderManager::InitLayerSize(int width, int height)
{
	if (mbIsInitLayer)
	{
		mLayerTerrain.clear();
		mLayerRemains.clear();
		mLayerGround.clear();
		mLayerParticle.clear();
		mLayerFog.clear();
	}

	mbIsInitLayer = true;

	mLayerWidth = width / DETAIL_GRID_SIZE;
	mLayerHeight = height / DETAIL_GRID_SIZE;

	mLayerTerrain.resize(mLayerWidth * mLayerHeight);
	mLayerRemains.resize(mLayerWidth * mLayerHeight);
	mLayerGround.resize(mLayerWidth * mLayerHeight);
	mLayerParticle.resize(mLayerWidth * mLayerHeight);
	mLayerFog.resize(mLayerWidth * mLayerHeight);
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
	case eEffectTag::GAUSSIAN:
		return CreateGaussianEffect();
	}
	return nullptr;
}

void RenderManager::AddRenderer(const Vector2& pos, RendererComponent* pComponent)
{
	POINT index = GetDetailIndex(pos);
	switch (pComponent->GetUnitLayer())
	{
	case eUnitLayer::Terrain:
		mLayerTerrain[index.x + index.y * mLayerWidth].push_back(pComponent->GetGameObject());
		break;
	case eUnitLayer::Remains:
		mLayerRemains[index.x + index.y * mLayerWidth].push_back(pComponent->GetGameObject());
		break;
	case eUnitLayer::Ground:
		mLayerGround[index.x + index.y * mLayerWidth].push_back(pComponent->GetGameObject());
		break;
	case eUnitLayer::Particle:
		mLayerParticle[index.x + index.y * mLayerWidth].push_back(pComponent->GetGameObject());
		break;
	case eUnitLayer::Fog:
		mLayerFog[index.x + index.y * mLayerWidth].push_back(pComponent->GetGameObject());
		break;
	}
}

void RenderManager::EraseRenderer(const Vector2& pos, RendererComponent* pComponent)
{
	POINT index = GetDetailIndex(pos);
	switch (pComponent->GetUnitLayer())
	{
	case eUnitLayer::Terrain:
		mLayerTerrain[index.x + index.y * mLayerWidth].erase(
			find(mLayerTerrain[index.x + index.y * mLayerWidth].begin(),
				mLayerTerrain[index.x + index.y * mLayerWidth].end(),
				pComponent->GetGameObject()
			));
		break;
	case eUnitLayer::Remains:
		mLayerRemains[index.x + index.y * mLayerWidth].erase(
			find(mLayerRemains[index.x + index.y * mLayerWidth].begin(),
				mLayerRemains[index.x + index.y * mLayerWidth].end(),
				pComponent->GetGameObject()
			));
		break;
	case eUnitLayer::Ground:
		mLayerGround[index.x + index.y * mLayerWidth].erase(
			find(mLayerGround[index.x + index.y * mLayerWidth].begin(),
				mLayerGround[index.x + index.y * mLayerWidth].end(),
				pComponent->GetGameObject()
			));
		break;
	case eUnitLayer::Particle:
		mLayerParticle[index.x + index.y * mLayerWidth].erase(
			find(mLayerParticle[index.x + index.y * mLayerWidth].begin(),
				mLayerParticle[index.x + index.y * mLayerWidth].end(),
				pComponent->GetGameObject()
			));
		break;
	case eUnitLayer::Fog:
		mLayerFog[index.x + index.y * mLayerWidth].erase(
			find(mLayerFog[index.x + index.y * mLayerWidth].begin(),
				mLayerFog[index.x + index.y * mLayerWidth].end(),
				pComponent->GetGameObject()
			));
		break;
	}
}

void RenderManager::RendererMoved(RendererComponent* pComponent, const Vector2& prevPos, const Vector2& curPos)
{
	POINT prev = GetDetailIndex(prevPos);
	POINT cur = GetDetailIndex(curPos);

	if (prev.x != cur.x || prev.y != cur.y)
	{
		EraseRenderer(prevPos, pComponent);
		AddRenderer(curPos, pComponent);
	}
}

IDWriteTextFormat* RenderManager::GetTextFormat(int fontSize, D2D1::ColorF color, eTextAlign align)
{
	IDWriteTextFormat* pFormat = nullptr;

	mpDWriteFactory->CreateTextFormat(TEXT("Arial"), nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize, TEXT("ko"), &pFormat);
	
	pFormat->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)align);
	
	return pFormat;
}

ID2D1SolidColorBrush* RenderManager::GetSolidBrush(float weight, D2D1::ColorF color)
{
	ID2D1SolidColorBrush* pBrush = nullptr;

	mpD2DContext->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);

	return pBrush;
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
	mMapBitmap[eBitmapTag::NONE] = CreateBitmap((LPWSTR)TEXT("Images/void.png"));

	mMapBitmap[eBitmapTag::UNIT_MARINE_L] = CreateBitmap((LPWSTR)TEXT("Images/Units/Marine/MarineL.png"));
	mMapBitmap[eBitmapTag::UNIT_MARINE_R] = CreateBitmap((LPWSTR)TEXT("Images/Units/Marine/MarineR.png"));
	mMapBitmap[eBitmapTag::UNIT_GOLIATH_HEAD_L] = CreateBitmap((LPWSTR)TEXT("Images/Units/GoliathHead/GoliathL.png"));
	mMapBitmap[eBitmapTag::UNIT_GOLIATH_HEAD_R] = CreateBitmap((LPWSTR)TEXT("Images/Units/GoliathHead/GoliathR.png"));
	mMapBitmap[eBitmapTag::UNIT_GOLIATH_LEG_L] = CreateBitmap((LPWSTR)TEXT("Images/Units/GoliathBody/GoliathL.png"));
	mMapBitmap[eBitmapTag::UNIT_GOLIATH_LEG_R] = CreateBitmap((LPWSTR)TEXT("Images/Units/GoliathBody/GoliathR.png"));

	mMapBitmap[eBitmapTag::BUILDING_BARRACK] = CreateBitmap((LPWSTR)TEXT("Images/Buildings/Barrack/Barrack.png"));
	mMapBitmap[eBitmapTag::BUILDING_FACTORY] = CreateBitmap((LPWSTR)TEXT("Images/Buildings/Factory/Factory.png"));
	mMapBitmap[eBitmapTag::BUILDING_START] = CreateBitmap((LPWSTR)TEXT("Images/Buildings/StartLocation/StartLocation.png"));

	mMapBitmap[eBitmapTag::TILE_PALETTE] = CreateBitmap((LPWSTR)TEXT("Images/Tile/TilePalette.png"));
	mMapBitmap[eBitmapTag::TILE_FULL_FOG] = CreateBitmap((LPWSTR)TEXT("Images/Tile/FogFull.png"));
	mMapBitmap[eBitmapTag::TILE_HALF_FOG] = CreateBitmap((LPWSTR)TEXT("Images/Tile/FogHalf.png"));

	mMapBitmap[eBitmapTag::ICON_BARRACK] = CreateBitmap((LPWSTR)TEXT("Images/Icon/Building/Barrack.png"));
	mMapBitmap[eBitmapTag::ICON_FACTORY] = CreateBitmap((LPWSTR)TEXT("Images/Icon/Building/Factory.png"));

	mMapBitmap[eBitmapTag::UI_CURSOR] = CreateBitmap((LPWSTR)TEXT("Images/UI/Cursor.png"));
	mMapBitmap[eBitmapTag::UI_CLICK_CIRCLE] = CreateBitmap((LPWSTR)TEXT("Images/UI/ClickCircle.png"));

	mMapBitmap[eBitmapTag::UI_BTN_SINGLE] = CreateBitmap((LPWSTR)TEXT("Images/UI/Single.png"));
	mMapBitmap[eBitmapTag::UI_BTN_ON_SINGLE] = CreateBitmap((LPWSTR)TEXT("Images/UI/OnSingle.png"));
	mMapBitmap[eBitmapTag::UI_BTN_MULTI_TAIL] = CreateBitmap((LPWSTR)TEXT("Images/UI/MultiTail.png"));
	mMapBitmap[eBitmapTag::UI_BTN_MULTI] = CreateBitmap((LPWSTR)TEXT("Images/UI/Multi.png"));
	mMapBitmap[eBitmapTag::UI_BTN_ON_MULTI] = CreateBitmap((LPWSTR)TEXT("Images/UI/OnMulti.png"));
	mMapBitmap[eBitmapTag::UI_BTN_EDITOR] = CreateBitmap((LPWSTR)TEXT("Images/UI/Editor.png"));
	mMapBitmap[eBitmapTag::UI_BTN_ON_EDITOR] = CreateBitmap((LPWSTR)TEXT("Images/UI/OnEditor.png"));
	mMapBitmap[eBitmapTag::UI_BTN_EXIT] = CreateBitmap((LPWSTR)TEXT("Images/UI/Exit.png"));
	mMapBitmap[eBitmapTag::UI_BTN_ON_EXIT] = CreateBitmap((LPWSTR)TEXT("Images/UI/OnExit.png"));

	mMapBitmap[eBitmapTag::BACKGROUND_LOGO] = CreateBitmap((LPWSTR)TEXT("Images/Background/Logo.png"));
	mMapBitmap[eBitmapTag::BACKGROUND_MAIN] = CreateBitmap((LPWSTR)TEXT("Images/Background/Main.png"));
	
	mMapBitmap[eBitmapTag::PARTICLE_ATTACK] = CreateBitmap((LPWSTR)TEXT("Images/Particle/Attack.png"));
	mMapBitmap[eBitmapTag::PARTICLE_EXPLOSION] = CreateBitmap((LPWSTR)TEXT("Images/Particle/Explosion.png"));
	mMapBitmap[eBitmapTag::PARTICLE_MARINE_DEAD] = CreateBitmap((LPWSTR)TEXT("Images/Particle/MarineDead.png"));

	mMapBitmap[eBitmapTag::REMAINS_MARINE] = CreateBitmap((LPWSTR)TEXT("Images/Remains/Marine.png"));

	mMapBitmap[eBitmapTag::SELECTED_CIRCLE_SMALL] = CreateBitmap((LPWSTR)TEXT("Images/Units/Selected/Selected22.png"));
	mMapBitmap[eBitmapTag::SELECTED_CIRCLE_MIDIUM] = CreateBitmap((LPWSTR)TEXT("Images/Units/Selected/Selected48.png"));
	mMapBitmap[eBitmapTag::SELECTED_CIRCLE_BIG] = CreateBitmap((LPWSTR)TEXT("Images/Units/Selected/Selected122.png"));
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

void RenderManager::ReleaseLayer()
{
	mLayerTerrain.clear();
	mLayerRemains.clear();
	mLayerGround.clear();
	mLayerParticle.clear();
}

void RenderManager::RenderLayer(const Layer& layer, const POINT& leftTop, int width, int height)
{
	size_t size;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			size = layer[(leftTop.x + x) + (leftTop.y + y) * mLayerWidth].size();
			for (int i = 0; i < size; ++i)
			{
				layer[(leftTop.x + x) + (leftTop.y + y) * mLayerWidth][i]->render(mpD2DContext);
			}
		}
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

ID2D1Effect* RenderManager::CreateGaussianEffect()
{
	ID2D1Effect* pResult = nullptr;
	mpD2DContext->CreateEffect(GUID{ 0x1feb6d69, 0x2fe6, 0x4ac9, 0x8c, 0x58, 0x1d, 0x7f, 0x93, 0xe7, 0xa6, 0xa5 }, &pResult);
	return pResult;
}

POINT RenderManager::GetDetailIndex(const Vector2& pos)
{
	return POINT{ (long)pos.x / DETAIL_GRID_SIZE, (long)pos.y / DETAIL_GRID_SIZE };
}

POINT RenderManager::GetDetailIndexByPoint(const POINT& pos)
{
	return POINT{ pos.x / DETAIL_GRID_SIZE, pos.y / DETAIL_GRID_SIZE };
}
