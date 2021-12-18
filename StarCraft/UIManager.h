#pragma once
#include "Singleton.h"

class UICursor;
class UIManager : public Singleton<UIManager>
{
private:
	UICursor* mpCursor;

public:
	void Init();
	void Release();
	void Update();

	void ChangeCursorState(eCursorState state);
};

