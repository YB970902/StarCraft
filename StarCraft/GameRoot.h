#pragma once

class GameRoot
{
public:
	void Init();
	void Release();
	void Run();

protected:

	void Update();
	void Render();
};