#pragma once

class GameRoot
{
public:
	void Init();
	void Release();
	void UpdateAndRender();

protected:

	void Update();
	void Render();
};