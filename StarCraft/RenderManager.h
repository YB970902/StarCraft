#pragma once
#include "Singleton.h"

#define RENDER RenderManager::GetInstance()

class SpriteComponent;
class ColorReplaceEffect;
class ShadowEffect;
class RendererComponent;
class GameObject;
class Gizmo;

typedef vector<vector<GameObject*>> Layer;
class RenderManager : public Singleton<RenderManager>
{
private:
	const int DETAIL_GRID_SIZE = 10;

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

	Layer mLayerTerrain;
	Layer mLayerRemains;
	Layer mLayerGround;
	Layer mLayerParticle;
	Layer mLayerFog;

	bool mbIsInitLayer = false;

	int mLayerWidth = 0;
	int mLayerHeight = 0;

	const int CULLING_EXTRA_SIZE = 3;

public:
	void Init();
	void Release();
	void Render();

	void InitDefaultLayer();
	void InitLayerSize(int width, int height);

	ID2D1Bitmap* GetBitmap(eBitmapTag tag);
	ID2D1Effect* CreateEffect(eEffectTag tag);

	void AddRenderer(const Vector2& pos, RendererComponent* pComponent);
	void EraseRenderer(const Vector2& pos, RendererComponent* pComponent);
	void RendererMoved(RendererComponent* pComponent, const Vector2& prevPos, const Vector2& curPos);

	IDWriteTextFormat* GetTextFormat(int fontSize = 14, D2D1::ColorF color = D2D1::ColorF::Black, eTextAlign align = eTextAlign::Center);
	ID2D1SolidColorBrush* GetSolidBrush(float weight = 1.0f, D2D1::ColorF color = D2D1::ColorF::Black);

private:
	void InitDirect2D();
	void InitBitmap();

	void ReleaseDirect2D();
	void ReleaseLayer();

	void RenderLayer(const Layer& layer, const POINT& leftTop, int width, int height);

	ID2D1Bitmap* CreateBitmap(LPWSTR fileName);
	ID2D1Effect* CreateColorReplaceEffect();
	ID2D1Effect* CreateShadowEffect();
	ID2D1Effect* CreateGaussianEffect();

	POINT GetDetailIndex(const Vector2& pos);
	POINT GetDetailIndexByPoint(const POINT& pos);
};