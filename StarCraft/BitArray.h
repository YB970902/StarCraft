#pragma once

class BitArray
{
private:
	vector<Bit> mBits;

	int mWidth = 0;
	int mDepth = 0;

	int mWordWidth = 0;
	int mBitWidth = 0;

	int mBitsSize = 0;
public:
	void Init(int width, int depth);
	Bit GetBit(int x, int y);
	Bit GetBitByPosition(int pos);

	inline int GetPosition(int x, int y) { return (y * mBitWidth + x); }
	inline Bit GetBitMaskByDiv(const div_t& dv) { return (Bit)1 << dv.rem; }

	inline int GetWordWidth() { return mWordWidth; }
	inline int GetBitWidth() { return mBitWidth; }

	void SetAt(int x, int y, bool set = true);
	bool IsSet(int x, int y);

	void Clear();
};