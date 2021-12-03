#pragma once
#include "Singleton.h"
#include "RenderConfig.h"

#define RENDER RenderManager::GetInstance()


class SpriteComponent;
class ColorReplaceEffect;
class ShadowEffect;
class RendererComponent;
typedef priority_queue<pair<float, RendererComponent*>, vector<pair<float, RendererComponent*>>, std::less<pair<float, RendererComponent*>>> QueueZOrder;
class RenderManager : public Singleton<RenderManager>
{
private:
	D3D_FEATURE_LEVEL m_d3dFeatureLevel = D3D_FEATURE_LEVEL_9_1;

	ID2D1Factory3* mpD2DFactory = nullptr;
	ID2D1Device2* mpD2DDevice = nullptr;
	ID2D1DeviceContext2* mpD2DContext = nullptr;
	ID2D1Bitmap1* mpD2DTargetBitmap = nullptr;

	ID3D11Device* mpD3DDevice = nullptr;
	ID3D11DeviceContext3* mpD3DContext = nullptr;
	IDXGIDevice3* mpDxgiDevice = nullptr;
	IDXGISwapChain1* mpSwapChain = nullptr;

	IDWriteFactory2* mpDWriteFactory = nullptr;
	IWICImagingFactory2* mpWicFactory = nullptr;

	unordered_map<eBitmapTag, ID2D1Bitmap*> mMapBitmap;

	QueueZOrder mQueTerrain;
	QueueZOrder mQueGround;
	QueueZOrder mQueSky;

public:
	void Init();
	void Release();
	void Render();

	ID2D1Bitmap* GetBitmap(eBitmapTag tag);
	ID2D1Effect* CreateEffect(eEffectTag tag);

	void PushRenderer(float posY, RendererComponent* pComponent);

private:
	void InitDirect2D();
	void InitBitmap();

	void ReleaseDirect2D();

	ID2D1Bitmap* CreateBitmap(LPWSTR fileName);
	ID2D1Effect* CreateColorReplaceEffect();
	ID2D1Effect* CreateShadowEffect();
};