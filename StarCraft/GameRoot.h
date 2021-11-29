#pragma once

class GameObject;
class GameRoot
{
protected:
	GameObject* mPlayer1 = nullptr;
	GameObject* mPlayer2 = nullptr;

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
	
	ID2D1Bitmap* mpBitmap = nullptr;

public:
	void Init();
	void Release();
	void UpdateAndRender();

protected:
	void InitializeContext();
	void CreateImage(LPWSTR fileName);

	void Update();
	void Render();
};