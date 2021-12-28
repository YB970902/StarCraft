#pragma once
#include "JPS.h"

typedef pair<int, int> ScanResult;

class DetailMap;
class BitArray;
class PathFinder
{
private:
	DetailMap* mpDetailMap = nullptr;
	BitArray* mpSearched = nullptr;

	JumpPointHeap mJumpPoint;
	TileCoord mEndPos;

	static const int MAX_FINDING_COUNT = 100;

	static const int DIR_UP = 0;
	static const int DIR_UP_RIGHT = 1;
	static const int DIR_RIGHT = 2;
	static const int DIR_RIGHT_DOWN = 3;
	static const int DIR_DOWN = 4;
	static const int DIR_LEFT_DOWN = 5;
	static const int DIR_LEFT = 6;
	static const int DIR_UP_LEFT = 7;
	static const int DIR_START = 8;
	static const int DIR_COUNT = 8;

	int mCount = 0;

public:
	PathFinder();
	virtual ~PathFinder();
	void Init(DetailMap* pDetailMap);
	void Release();
	bool FindPath(int sX, int sY, int eX, int eY, list<TileCoord>& result);

private:
	bool IsPassable(const TileCoord& coord);
	bool IsPassable(const TileCoord& coord, const int dir);

	int ForcedNeighbours(const TileCoord& coord, const int dir);
	int NaturalNeighbours(const int dir);

	ScanResult BitScanUpDirection(int x, int y);
	ScanResult BitScanRightDirection(int x, int y);
	ScanResult BitScanDownDirection(int x, int y);
	ScanResult BitScanLeftDirection(int x, int y);

	TileCoord FindJumpPoint(const TileCoord& coord, const char dir);
	bool ScanJumpPoint(TileCoord coord, const char dir, TileCoord& jumpPoint);

	inline bool IsDiagonal(const int dir) { return (dir % 2) != 0; }
	inline int Implies(const int a, const int b) { return a ? b : 1; }
	inline int AddDirectionToSet(const int dirs, const int dir) { return dirs | 1 << dir; }

	inline int GetCoordinateDir(const TileCoord& start, const TileCoord& dest)
	{
		int dirs = 0;
		if (start.GetX() > dest.GetX()) { dirs += 1 << 4; }
		else if (start.GetX() < dest.GetX()) { dirs += 1 << 2; }
		if (start.GetY() > dest.GetY()) { dirs += 1 << 3; }
		else if (start.GetY() < dest.GetY()) { dirs += 1 << 1; }
		return dirs;
	}
};