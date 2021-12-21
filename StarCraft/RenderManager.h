#pragma once
#include "Singleton.h"

#define RENDER RenderManager::GetInstance()

enum class eTextAlign
{
	Left = 0,
	Right = 1,
	Center = 2,
};

class SpriteComponent;
class ColorReplaceEffect;
class ShadowEffect;
class RendererComponent;
class GameObject;
class Gizmo;

typedef vector<vector<GameObject*>> QueueZOrder;
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

	QueueZOrder mLayerTerrain;
	QueueZOrder mLayerRemains;
	QueueZOrder mLayerGround;
	QueueZOrder mLayerParticle;
	vector<GameObject*> mLayerUI;
	bool mbIsInitLayer = false;
	int mLayerWidth = 0;
	int mLayerHeight = 0;

	const int CULLING_EXTRA_SIZE = 3;

	vector<Gizmo*> mVecGizmo;

public:
	void Init();
	void Release();
	void Render();

	void InitLayerSize(int width, int height);

	ID2D1Bitmap* GetBitmap(eBitmapTag tag);
	ID2D1Effect* CreateEffect(eEffectTag tag);

	void AddRenderer(const Vector2& pos, RendererComponent* pComponent);
	void AddUIRenderer(RendererComponent* pComponent);
	void EraseRenderer(const Vector2& pos, RendererComponent* pComponent);
	void EraseUIRenderer(RendererComponent* pComponent);
	void RendererMoved(RendererComponent* pComponent, const Vector2& prevPos, const Vector2& curPos);

	Gizmo* RenderText(wstring text, Vector2 pos, Vector2 size, int fontSize = 14, D2D1::ColorF color = D2D1::ColorF::Black, eTextAlign align = eTextAlign::Center);
	Gizmo* RenderRect(Vector2 pos, Vector2 size, float weight = 1.0f, D2D1::ColorF color = D2D1::ColorF::Black, Vector2 anchor = Vector2(0.5f, 0.5f));
	Gizmo* RenderLine(Vector2 startPos, Vector2 endPos, float width = 1.0f, D2D1::ColorF color = D2D1::ColorF::Black);
	void RemoveGizmo(Gizmo* pGizmo);

private:
	void InitDirect2D();
	void InitBitmap();

	void ReleaseDirect2D();
	void ReleaseGizmo();
	void ReleaseLayer();

	ID2D1Bitmap* CreateBitmap(LPWSTR fileName);
	ID2D1Effect* CreateColorReplaceEffect();
	ID2D1Effect* CreateShadowEffect();

	POINT GetDetailIndex(const Vector2& pos);
	POINT GetDetailIndexByPoint(const POINT& pos);
};