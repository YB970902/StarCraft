#pragma once
class MainGame
{
	bool mbIsInGame = false;

	eSceneTag mSceneTag = eSceneTag::None;
public:
	void Init();
	void Release();
	void Update();
	void Render();
};