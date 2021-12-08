#include "stdafx.h"
#include "BitArray.h"

void BitArray::Init(int width, int depth)
{
	div_t dv = div(width, BIT_COUNT);
	if (dv.rem) { ++dv.quot; }

	mWordWidth = dv.quot;
	mBitWidth = mWordWidth * BIT_COUNT;
	mBits.resize(dv.quot * depth, 0);

	mWidth = width;
	mDepth = depth;

	mBitsSize = mBits.size() * BIT_COUNT;
}

Bit BitArray::GetBit(int x, int y)
{
	int pos = GetPosition(x, y);
	if (pos < 0 || pos >= mBitsSize) { return ~(0); }
	return mBits[pos / BIT_COUNT];
}

Bit BitArray::GetBitByPosition(int pos)
{
	return mBits[pos / BIT_COUNT];
}

void BitArray::SetAt(int x, int y, bool set)
{
	int pos = GetPosition(x, y);
	if (pos < 0 || pos >= mBitsSize) return;

	div_t dv = div(pos, BIT_COUNT);
	if (set) { mBits[dv.quot] |= GetBitMaskByDiv(dv); }
	else { mBits[dv.quot] &= ~GetBitMaskByDiv(dv); }
}

bool BitArray::IsSet(int x, int y)
{
	int pos = GetPosition(x, y);
	if (pos < 0 || pos >= mBitsSize) { return true; }

	div_t dv = div(pos, BIT_COUNT);
	return (mBits[dv.quot] & GetBitMaskByDiv(dv)) ? true : false;
}

void BitArray::Clear()
{
	fill(mBits.begin(), mBits.end(), 0);
}
