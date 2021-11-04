#pragma once

class GameObject;
class GameRoot
{
protected:
	GameObject* mPlayer1 = nullptr;
	GameObject* mPlayer2 = nullptr;
public:
	void Init();
	void Release();
	void UpdateAndRender();

protected:
	void Update();
	void Render();
};