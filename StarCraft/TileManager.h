#pragma once
#include "Singleton.h"
#include "JPS.h"

#define TILE TileManager::GetInstance()

//#define DEBUG_MODE

class DetailMap;
class BitArray;
class RectGizmo;
class Scene;
class TileManager : public Singleton<TileManager>
{
private:
	struct TileData
	{
		int ObstacleCount = 0;
		int OccupiedCount = 0;

		bool IsObstacle() { return (ObstacleCount > 0); }
		bool IsOccupied() { return (OccupiedCount > 0); }
	};

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

	static const int TILE_SIZE = 10;
	static const int HALF_TILE_SIZE = 5;
	static const int SMALL_UNIT_SIZE = TILE_SIZE * 2;
	static const int HALF_SMALL_UNIT_SIZE = TILE_SIZE;
	static const int BIG_UNIT_SIZE = SMALL_UNIT_SIZE * 2;
	static const int HALF_BIG_UNIT_SIZE = SMALL_UNIT_SIZE;

	static const int SMALL_UNIT_TILE_SIZE = 2;
	static const int BIG_UNIT_TILE_SIZE = 4;
	static const int HALF_SMALL_UNIT_TILE_SIZE = 1;
	static const int HALF_BIG_UNIT_TILE_SIZE = 2;

	DetailMap* mpTerrainMap = nullptr;
	DetailMap* mpSmallUnitMap = nullptr;
	DetailMap* mpBigUnitMap = nullptr;

	DetailMap* mpCurDetailMap = nullptr;
	TileCoord* mpCurEndPoint = nullptr;

	vector<TileData> mVecNormalTileState;
	vector<TileData> mVecSmallTileState;
	vector<TileData> mVecBigTileState;

	int mTileWidth = 0;
	int mTileHeight = 0;

	vector<RectGizmo*> mVecGizmo;
	vector<int> mVecPathChecker;

	static const int TILE_DEFAULT = D2D1::ColorF::LightGreen;
	static const int TILE_OBSTACLE = D2D1::ColorF::OrangeRed;
	static const int TILE_OCCUPIED = D2D1::ColorF::Black;
	static const int TILE_PATH = D2D1::ColorF::LightBlue;

public:
	enum class eTileState
	{
		None,
		Obstacle,
		Occupied,
	};

	void Init();
	void Release();
	void LoadTileMap(Scene* pLoadedScene, LPCWSTR path);

	DetailMap* GetTileMap(eUnitTileSize unitSize);
	bool GetEndPosition(const Vector2& startPos, const Vector2& targetPos, const eUnitTileSize& unitSize, TileCoord* pEndPoint);
	bool BeginSearch(Vector2 startPos, TileCoord* pEndPoint, const eUnitTileSize& unitSize, JumpPointHeap* pJumpPoint, BitArray* pSearched, list<TileCoord>& result, TileNode** ppNearNode);
	bool KeepSearch(TileCoord* pEndPoint, const eUnitTileSize& unitSize, JumpPointHeap* pJumpPoint, BitArray* pSearched, list<TileCoord>& result, TileNode** ppNearNode);
	void CreateDetailTempPath(TileNode* pNearNode, list<TileCoord>& result);
	void ConnectPath(vector<TileCoord>& passedPath, list<TileCoord>& correctPath);

	void SetOccupyTile(const Vector2& pos, const eUnitTileSize& unitSize, bool set);
	void SetObstacleTile(const Vector2& pos, const eUnitTileSize& unitSize, bool set);
	bool IsTileOpen(const Vector2& pos, const eUnitTileSize& unitSize, eTileState& state);

	void SetGizmoColor(int x, int y, int color);
	void InitGizmoColor();

	inline void GetPositionByTileCoord(const TileCoord& coord, Vector2& position)
	{
		position.x = coord.GetX() * TILE_SIZE + HALF_TILE_SIZE;
		position.y = coord.GetY() * TILE_SIZE + HALF_TILE_SIZE;
	}

	inline int GetTileWidth() { return mTileWidth; }
	inline int GetTileHeight() { return mTileHeight; }
private:
	void InitTileState();

	bool IsPassable(const TileCoord& coord);
	bool IsPassable(const TileCoord& coord, const int dir);

	bool IsCoordinateInTileMap(const TileCoord& coord) { return (coord.GetX() >= 0 && coord.GetX() < mTileWidth && coord.GetY() >= 0 && coord.GetY() < mTileHeight); }

	int ForcedNeighbours(const TileCoord& coord, const int dir);
	int NaturalNeighbours(const int dir);

	pair<int, int> BitScanUpDirection(int x, int y);
	pair<int, int> BitScanRightDirection(int x, int y);
	pair<int, int> BitScanDownDirection(int x, int y);
	pair<int, int> BitScanLeftDirection(int x, int y);

	TileCoord FindJumpPoint(const TileCoord& coord, const char dir);
	bool ScanJumpPoint(TileCoord coord, const char dir, TileCoord& jumpPoint);

	bool IsNearCoord(const TileCoord& startCoord, const TileCoord& endCoord, TileCoord& outCoord, const char dir, int& nearDist);
	bool ScanOpenCoord(const TileCoord& coord, const char dir, TileCoord& result);

	void CreateDetailPath(const TileNode::SharedPtr& pCurNode, const TileCoord* pEndNode, list<TileCoord>& result);

	void GetTileCoord(const Vector2& pos, const eUnitTileSize& unitSize, TileCoord& leftTop, TileCoord& rightBottom);
	void GetTileCoordByPosition(const Vector2& pos, TileCoord& coord);

	void SetTileState(const Vector2& pos, const eUnitTileSize& unitSize, bool isObstacle, bool isSet);

	inline bool IsDiagonal(const int dir) { return (dir % 2) != 0; }
	inline int Implies(const int a, const int b) { return a ? b : 1; }
	inline int AddDirectionToSet(const int dirs, const int dir) { return dirs | 1 << dir; }

	inline int GetCoordinateDir(const TileCoord& start, const TileCoord& dest)
	{
		static const int dir[] = { 0, 0, 2, 1, 4, 0, 3, 0, 6, 7, 0, 0, 5 };
		int dirs = 0;
		if (start.GetX() > dest.GetX()) { dirs |= 1 << 3; }
		else if (start.GetX() < dest.GetX()) { dirs |= 1 << 1; }
		if (start.GetY() > dest.GetY()) { dirs |= 1; }
		else if (start.GetY() < dest.GetY()) { dirs |= 1 << 2; }
		return dir[dirs];
	}

	inline int GetCoordinateDistance(const TileCoord& start, const TileCoord& dest)
	{
		int width = abs(start.GetX() - dest.GetX());
		int height = abs(start.GetY() - dest.GetY());
		return max(width, height);
	}
};