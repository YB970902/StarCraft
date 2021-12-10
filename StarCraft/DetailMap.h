#pragma once

class TextGizmo;
class BitArray;
class DetailMap
{
private:
	BitArray* mBitX = nullptr;
	BitArray* mBitY = nullptr;

	int mWidth = 0;
	int mHeight = 0;
public:
	void Init(int width, int height);
	void Release();

	bool IsCollision(int x, int y);

	// x, y ��ǥ�� �浹�������� ����
	void SetAt(int x, int y);
	// x, y ��ǥ�� �浹�������� ����
	void ClrAt(int x, int y);

	int GetCloseValue(int x, int y, bool xAxis, bool forward);
	int GetOpenValue(int x, int y, bool xAxis, bool forward);

	inline int GetWidth() { return mWidth; }
	inline int GetHeight() { return mHeight; }

	int GetPosX(int x, int y);
	int GetPosY(int x, int y);
};