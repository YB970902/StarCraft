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
class Gizmo;
typedef priority_queue<pair<float, RendererComponent*>, vector<pair<float, RendererComponent*>>, std::greater<pair<float, RendererComponent*>>> QueueZOrder;
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

	vector<Gizmo*> mVecGizmo;

	Vector2 mCameraPosition;
	float mCameraZoom = 0.0f;

public:
	void Init();
	void Release();
	void Render();

	ID2D1Bitmap* GetBitmap(eBitmapTag tag);
	ID2D1Effect* CreateEffect(eEffectTag tag);

	void AddRenderer(float posY, RendererComponent* pComponent);
	Gizmo* RenderText(wstring text, Vector2 pos, Vector2 size, int fontSize = 14, D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black), eTextAlign align = eTextAlign::Center);
	Gizmo* RenderRect(Vector2 pos, Vector2 size, float weight = 1.0f, D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black), Vector2 anchor = Vector2(0.5f, 0.5f));

	Vector2 GetCameraPosition() { return mCameraPosition; }
	void SetCameraPosition(Vector2 pos) { mCameraPosition = pos; }
	void AddCameraPosition(Vector2 add) { mCameraPosition += add; }

	void SetCameraZoom(float zoom) { mCameraZoom = zoom; }
	void AddCameraZoom(float zoom) { mCameraZoom += zoom; }
	float GetCameraZoom() { return mCameraZoom; }

private:
	void InitDirect2D();
	void InitBitmap();

	void ReleaseDirect2D();
	void ReleaseGizmo();

	ID2D1Bitmap* CreateBitmap(LPWSTR fileName);
	ID2D1Effect* CreateColorReplaceEffect();
	ID2D1Effect* CreateShadowEffect();
};