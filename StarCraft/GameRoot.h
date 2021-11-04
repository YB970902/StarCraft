#pragma once

class Image;
class GameRoot
{
protected:
	int mMousePosX = 0;
	int mMousePosY = 0;
	int mClickedMousePosX = 0;
	int mClickedMousePosY = 0;

	Image* mBackBuffer = nullptr;

public:
	void Init();
	void Release();
	void UpdateAndRender();

protected:
	void Update();
	void Render();
};