#include "stdafx.h"
#include "TileManager.h"
#include "BitArray.h"
#include "DetailMap.h"
#include "RectGizmo.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "FogManager.h"
#include "Scene.h"
#include "Tile.h"
#include <boost/lexical_cast.hpp>

void TileManager::Init()
{
	mbIsReleased = false;
	mpTerrainMap = new DetailMap();
	mpSmallUnitMap = new DetailMap;
	mpBigUnitMap = new DetailMap;
}

void TileManager::Release()
{
	mbIsReleased = true;
	SAFE_RELEASE(mpTerrainMap);
	SAFE_RELEASE(mpSmallUnitMap);
	SAFE_RELEASE(mpBigUnitMap);
}

void TileManager::LoadTileMap(Scene* pLoadedScene, LPCWSTR path)
{
	// 이미 저장된 맵을 받아야 함
	// 
	// 임의값 지정
	ifstream fin(path);
	string str;
	fin >> str;
	int width = boost::lexical_cast<int>(str);
	fin >> str;
	int height = boost::lexical_cast<int>(str);
	mTileWidth = width * 3;
	mTileHeight = height * 3;

	CAMERA->SetMovingSize(POINT{ width * Tile::TILE_SIZE, height * Tile::TILE_SIZE });
	RENDER->InitLayerSize(width * Tile::TILE_SIZE, height * Tile::TILE_SIZE);
	PHYSICS->InitLayerSize(width * Tile::TILE_SIZE, height * Tile::TILE_SIZE);
	//FOG->InitLayerSize(width * Tile::TILE_SIZE, height * Tile::TILE_SIZE);

	mVecPathChecker.resize(mTileWidth * mTileHeight);
	mpTerrainMap->Init(mTileWidth, mTileHeight);
	mpSmallUnitMap->Init(mTileWidth, mTileHeight);
	mpBigUnitMap->Init(mTileWidth, mTileHeight);
	mVecNormalTileState.resize(mTileWidth * mTileHeight);
	mVecSmallTileState.resize(mTileWidth * mTileHeight);
	mVecBigTileState.resize(mTileWidth * mTileHeight);

#ifdef  DEBUG_MODE
	mVecGizmo.resize(mTileWidth * mTileHeight);
	for (int x = 0; x < mTileWidth; ++x)
	{
		for (int y = 0; y < mTileHeight; ++y)
		{
			mVecGizmo[x + y * mTileWidth] = (RectGizmo*)RENDER->RenderRect(Vector2(x * TILE_SIZE, y * TILE_SIZE), Vector2(TILE_SIZE - 1, TILE_SIZE - 1), 1.0f, TILE_DEFAULT);
		}
	}
#endif //  DEBUG_MODE

	Tile* pTile = nullptr;

	int x = 0;
	int y = 0;

	int row = 0;
	int col = 0;

	bool bIsObstacle = false;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			pTile = static_cast<Tile*>(pLoadedScene->AddGameObject(new Tile()));
			fin >> str;
			x = boost::lexical_cast<int>(str);
			fin >> str;
			y = boost::lexical_cast<int>(str);
			fin >> str;
			row = boost::lexical_cast<int>(str);
			fin >> str;
			col = boost::lexical_cast<int>(str);
			pTile->SetPosition(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE);
			pTile->SetTileRowCol(row, col);
			fin >> str;
			bIsObstacle = boost::lexical_cast<bool>(str);
			if (bIsObstacle)
			{
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						mpTerrainMap->SetAt(x * 3 + i, y * 3 + j);
						++mVecNormalTileState[(x * 3 + i) + (y * 3 + j) * mTileWidth].ObstacleCount;
					}
				}
			}
		}
	}

	// 테두리만 충돌처리하기
	for (int y = 0; y < mTileHeight; ++y)
	{
		mpTerrainMap->SetAt(0, y);
		++mVecNormalTileState[y * mTileWidth].ObstacleCount;
		mpTerrainMap->SetAt(mTileWidth - 1, y);
		++mVecNormalTileState[(mTileWidth - 1) + y * mTileWidth].ObstacleCount;
	}

	for (int x = 0; x < mTileWidth; ++x)
	{
		mpTerrainMap->SetAt(x, 0);
		++mVecNormalTileState[x].ObstacleCount;
		mpTerrainMap->SetAt(x, mTileHeight - 1);
		++mVecNormalTileState[x + (mTileHeight - 1) * mTileWidth].ObstacleCount;
	}

	InitTileState();

	for (int x = 0; x < mTileWidth; ++x)
	{
		for (int y = 0; y < mTileHeight; ++y)
		{
			if (mpSmallUnitMap->IsCollision(x, y))
			{
				++mVecSmallTileState[x + y * mTileWidth].ObstacleCount;
			}
			if (mpBigUnitMap->IsCollision(x, y))
			{
				++mVecBigTileState[x + y * mTileWidth].ObstacleCount;
			}
		}
	}
}

DetailMap* TileManager::GetTileMap(eUnitTileSize unitSize)
{
	switch (unitSize)
	{
	case eUnitTileSize::Small:
		return mpSmallUnitMap;
	case eUnitTileSize::Big:
		return mpBigUnitMap;
	}
	return nullptr;
}

bool TileManager::GetEndPosition(const Vector2& startPos, const Vector2& targetPos, const eUnitTileSize& unitSize, TileCoord* pEndPoint)
{
	TileCoord startCoord;
	TileCoord endCoord;

	GetTileCoordByPosition(startPos, startCoord);
	GetTileCoordByPosition(targetPos, endCoord);

	int tileSize = 0;
	switch (unitSize)
	{
	case eUnitTileSize::Small:
		mpCurDetailMap = mpSmallUnitMap;
		tileSize = SMALL_UNIT_TILE_SIZE;
		break;
	case eUnitTileSize::Big:
		mpCurDetailMap = mpBigUnitMap;
		tileSize = BIG_UNIT_TILE_SIZE;
		break;
	default:
		return false;
	}

	if (endCoord.GetX() <= 0) { endCoord.SetX(tileSize); }
	if (endCoord.GetX() >= mTileWidth - 1) { endCoord.SetX(mTileWidth - tileSize - 1); }
	if (endCoord.GetY() <= 0) { endCoord.SetY(tileSize); }
	if (endCoord.GetY() >= mTileHeight - 1) { endCoord.SetY(mTileWidth - tileSize - 1); }


	if (mpCurDetailMap->IsCollision(endCoord.GetX(), endCoord.GetY()))
	{
		int nearDist = INT_MAX;
		TileCoord result;
		IsNearCoord(startCoord, endCoord, result, DIR_DOWN, nearDist);
		IsNearCoord(startCoord, endCoord, result, DIR_RIGHT, nearDist);
		IsNearCoord(startCoord, endCoord, result, DIR_UP, nearDist);
		IsNearCoord(startCoord, endCoord, result, DIR_LEFT, nearDist);

		IsNearCoord(startCoord, endCoord, result, DIR_LEFT_DOWN, nearDist);
		IsNearCoord(startCoord, endCoord, result, DIR_RIGHT_DOWN, nearDist);
		IsNearCoord(startCoord, endCoord, result, DIR_UP_RIGHT, nearDist);
		IsNearCoord(startCoord, endCoord, result, DIR_UP_LEFT, nearDist);

		pEndPoint->SetX(result.GetX());
		pEndPoint->SetY(result.GetY());

		return true;
	}
	else
	{
		pEndPoint->SetX(endCoord.GetX());
		pEndPoint->SetY(endCoord.GetY());

		return true;
	}
}

bool TileManager::BeginSearch(Vector2 startPos, TileCoord* pEndPoint, const eUnitTileSize& unitSize, JumpPointHeap* pJumpPoint, BitArray* pSearched, list<TileCoord>& result, TileNode::SharedPtr* ppNearNode)
{
	DetailMap* pDetailMap = nullptr;
	switch (unitSize)
	{
	case eUnitTileSize::Small:
		pDetailMap = mpSmallUnitMap;
		break;
	case eUnitTileSize::Big:
		pDetailMap = mpBigUnitMap;
		break;
	default:
		return false;
	}
#ifdef  DEBUG_MODE
	InitGizmoColor();
#endif // DEBUG_MODE

	int width = pDetailMap->GetWidth();
	int height = pDetailMap->GetHeight();

	TileCoord startCoord;
	GetTileCoordByPosition(startPos, startCoord);

	if (startCoord.GetX() < 0 || startCoord.GetX() >= width || startCoord.GetY() < 0 || startCoord.GetY() >= height ||
		pEndPoint->GetX() < 0 || pEndPoint->GetX() >= width || pEndPoint->GetY() < 0 || pEndPoint->GetY() >= height)
	{
		return false;
	}

	pJumpPoint->Clear();
	pSearched->Clear();
	result.clear();

	TileNode::SharedPtr startNode = make_shared<TileNode>();
	startNode->Set(nullptr, startCoord, *pEndPoint, DIR_START);

	pJumpPoint->Push(startNode);
	pSearched->SetAt(startNode->GetPositionX(), startNode->GetPositionY(), true);

	(*ppNearNode) = startNode;

	return KeepSearch(pEndPoint, unitSize, pJumpPoint, pSearched, result, ppNearNode);
}

bool TileManager::KeepSearch(TileCoord* pEndPoint, const eUnitTileSize& unitSize, JumpPointHeap* pJumpPoint, BitArray* pSearched, list<TileCoord>& result, TileNode::SharedPtr* ppNearNode)
{
	mpCurEndPoint = pEndPoint;

	switch (unitSize)
	{
	case eUnitTileSize::Small:
		mpCurDetailMap = mpSmallUnitMap;
		break;
	case eUnitTileSize::Big:
		mpCurDetailMap = mpBigUnitMap;
		break;
	default:
		return false;
	}

	int width = mpCurDetailMap->GetWidth();
	int height = mpCurDetailMap->GetHeight();

	int count = 0;

	while (pJumpPoint->IsEmpty() == false)
	{
		TileNode::SharedPtr pCurNode = pJumpPoint->Pop();
		if ((*ppNearNode) == nullptr || (*ppNearNode)->GetScore() < pCurNode->GetScore())
		{
			(*ppNearNode) = pCurNode;
		}
		int direction = ForcedNeighbours(pCurNode->GetPosition(), pCurNode->GetDirection()) | NaturalNeighbours(pCurNode->GetDirection());

		for (int dir = 0; dir < DIR_COUNT; ++dir)
		{
			if ((1 << dir) & direction)
			{
				++count;
				TileCoord newCoord = FindJumpPoint(pCurNode->GetPosition(), dir);
				if (!newCoord.IsEmpty())
				{
					if (newCoord == (*mpCurEndPoint))
					{
						CreateDetailPath(pCurNode, mpCurEndPoint, result);
						return true;
					}

					TileNode::SharedPtr pNewNode = make_shared<TileNode>();
					pNewNode->Set(pCurNode, newCoord, (*mpCurEndPoint), dir);

					if (!pSearched->IsSet(newCoord.GetX(), newCoord.GetY()))
					{
						pJumpPoint->Push(pNewNode);
						pSearched->SetAt(newCoord.GetX(), newCoord.GetY(), true);
					}
					else
					{
						pJumpPoint->PushSmaller(pNewNode);
					}
				}
			}
		}

		if (count >= MAX_FINDING_COUNT && !pJumpPoint->IsEmpty())
		{
			return false;
		}
	}

	return false;
}

void TileManager::CreateDetailTempPath(TileNode::SharedPtr pNearNode, list<TileCoord>& result)
{
	if (pNearNode == nullptr)
	{
		return;
	}
	TileNode::SharedPtr nearNode = make_shared<TileNode>();
	nearNode->SetParent(pNearNode->GetParent());
	nearNode->SetPosition(pNearNode->GetPosition());
	nearNode->SetDirection(pNearNode->GetDirection());
	CreateDetailPath(nearNode, nullptr, result);
}

void TileManager::ConnectPath(vector<TileCoord>& passedPath, list<TileCoord>& correctPath)
{
	fill(mVecPathChecker.begin(), mVecPathChecker.end(), -1);

	TileCoord prevCoord = correctPath.front();

	for (int i = 0; i < passedPath.size(); ++i)
	{
		mVecPathChecker[passedPath[i].GetX() + passedPath[i].GetY() * mTileWidth] = prevCoord.GetX() + prevCoord.GetY() * mTileWidth;
		prevCoord = passedPath[i];
	}

	prevCoord = correctPath.front();
	correctPath.pop_front();
	while (false == correctPath.empty())
	{
		mVecPathChecker[prevCoord.GetX() + prevCoord.GetY() * mTileWidth] = correctPath.front().GetX() + correctPath.front().GetY() * mTileWidth;
		prevCoord = correctPath.front();
		correctPath.pop_front();
	}

	TileCoord curCoord = passedPath[passedPath.size() - 1];
	div_t dv;
	int index = 0;
	while (curCoord != prevCoord)
	{
		correctPath.push_back(curCoord);
		index = mVecPathChecker[curCoord.GetX() + curCoord.GetY() * mTileWidth];
		dv = div(index, mTileWidth);
		curCoord = TileCoord(dv.rem, dv.quot);
	}
	correctPath.push_back(curCoord);
}

void TileManager::SetOccupyTile(const Vector2& pos, const eUnitTileSize& unitSize, bool set)
{
	SetTileState(pos, unitSize, false, set);
}

void TileManager::SetObstacleTile(const Vector2& pos, const eUnitTileSize& unitSize, bool set)
{
	SetTileState(pos, unitSize, true, set);
}

bool TileManager::IsTileOpen(const Vector2& pos, const eUnitTileSize& unitSize, eTileState& state)
{
	TileCoord leftTop;
	TileCoord rightBottom;

	GetTileCoord(pos, unitSize, leftTop, rightBottom);

	int width = rightBottom.GetX() - leftTop.GetX() + 1;
	int height = rightBottom.GetY() - leftTop.GetY() + 1;

	bool result = true;
	state = eTileState::None;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (mVecNormalTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].IsObstacle())
			{
				state = eTileState::Obstacle;
				return false;
			}
			else if (mVecNormalTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].IsOccupied())
			{
				state = eTileState::Occupied;
				result = false;
			}
		}
	}

	return result;
}

bool TileManager::GetSpawnPosition(const Vector2& spawnPos, const eUnitTileSize& unitSize, Vector2& pos)
{
	TileCoord spawnCoord;

	GetTileCoordByPosition(spawnPos, spawnCoord);

	int tileSize = 0;
	switch (unitSize)
	{
	case eUnitTileSize::Small:
		mpCurDetailMap = mpSmallUnitMap;
		mpVecCurTileState = &mVecSmallTileState;
		tileSize = SMALL_UNIT_TILE_SIZE;
		break;
	case eUnitTileSize::Big:
		mpCurDetailMap = mpBigUnitMap;
		mpVecCurTileState = &mVecBigTileState;
		tileSize = BIG_UNIT_TILE_SIZE;
		break;
	default:
		return false;
	}

	if (spawnCoord.GetX() <= 0) { spawnCoord.SetX(tileSize); }
	if (spawnCoord.GetX() >= mTileWidth - 1) { spawnCoord.SetX(mTileWidth - tileSize - 1); }
	if (spawnCoord.GetY() <= 0) { spawnCoord.SetY(tileSize); }
	if (spawnCoord.GetY() >= mTileHeight - 1) { spawnCoord.SetY(mTileWidth - tileSize - 1); }

	if (mpCurDetailMap->IsCollision(spawnCoord.GetX(), spawnCoord.GetY()))
	{
		int nearDist = INT_MAX;
		TileCoord result;
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_DOWN, nearDist);
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_RIGHT, nearDist);
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_UP, nearDist);
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_LEFT, nearDist);

		IsNearCoord(spawnCoord, spawnCoord, result, DIR_LEFT_DOWN, nearDist);
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_RIGHT_DOWN, nearDist);
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_UP_RIGHT, nearDist);
		IsNearCoord(spawnCoord, spawnCoord, result, DIR_UP_LEFT, nearDist);

		GetPositionByTileCoord(result, pos);

		return true;
	}
	else
	{
		GetPositionByTileCoord(spawnCoord, pos);
		return true;
	}
	return false;
}

void TileManager::SetGizmoColor(int x, int y, int color)
{
	if (x < 0 || x >= mTileWidth || y < 0 || y >= mTileHeight) { return; }
	mVecGizmo[x + y * mTileWidth]->SetColor(color);
}

void TileManager::InitGizmoColor()
{
	for (int x = 0; x < mTileWidth; ++x)
	{
		for (int y = 0; y < mTileHeight; ++y)
		{
			if (mVecNormalTileState[x + y * mTileWidth].IsObstacle())
			{
				mVecGizmo[x + y * mTileWidth]->SetColor(TILE_OBSTACLE);
			}
			else if (mVecNormalTileState[x + y * mTileWidth].IsOccupied())
			{
				mVecGizmo[x + y * mTileWidth]->SetColor(TILE_OCCUPIED);
			}
			else
			{
				mVecGizmo[x + y * mTileWidth]->SetColor(TILE_DEFAULT);
			}
		}
	}
}

void TileManager::InitTileState()
{
	for (int y = mTileHeight - 1; y >= 0; --y)
	{
		for (int x = mTileWidth - 1; x >= 0; --x)
		{
			if (mpTerrainMap->IsCollision(x, y))
			{
				for (int i = 0; i < SMALL_UNIT_TILE_SIZE; ++i)
				{
					for (int j = 0; j < SMALL_UNIT_TILE_SIZE; ++j)
					{
						mpSmallUnitMap->SetAt(x - i, y - j);
#ifdef  DEBUG_MODE
						SetGizmoColor(x - i + 1, y - j + 1, TILE_OBSTACLE);
#endif // DEBUG_MODE
					}
				}
				for (int i = 0; i < BIG_UNIT_TILE_SIZE; ++i)
				{
					for (int j = 0; j < BIG_UNIT_TILE_SIZE; ++j)
					{
						mpBigUnitMap->SetAt(x - i + 1, y - j + 1);
					}
				}
			}
		}
	}
}

bool TileManager::IsPassable(const TileCoord& coord)
{
	return !mpCurDetailMap->IsCollision(coord.GetX(), coord.GetY());
}

bool TileManager::IsPassable(const TileCoord& coord, const int dir)
{
	return IsPassable(TileCoord::NextCoordinate(coord, dir));
}

bool TileManager::IsTilePassable(const TileCoord& coord)
{
	if (coord.GetX() < 0 ||
		coord.GetX() >= mTileWidth - 1 ||
		coord.GetY() < 0 ||
		coord.GetY() >= mTileHeight - 1)
	{
		return false;
	}

	return (!(*mpVecCurTileState)[coord.GetX() + coord.GetY() * mTileWidth].IsObstacle() &&
		!(*mpVecCurTileState)[coord.GetX() + coord.GetY() * mTileWidth].IsOccupied());
}

int TileManager::ForcedNeighbours(const TileCoord& coord, const int dir)
{
	if (dir >= DIR_COUNT) return 0;
	int dirs = 0;

	if (IsDiagonal(dir))
	{
		// 다음 위치에서 왼쪽은 이동가능하고, 왼쪽아래가 이동 불가능할경우 강제이웃 추가
		if (!Implies(IsPassable(coord, (dir + DIR_LEFT) % DIR_COUNT), IsPassable(coord, (dir + DIR_LEFT_DOWN) % DIR_COUNT)))
		{
			dirs = AddDirectionToSet(dirs, (dir + DIR_LEFT) % DIR_COUNT);
		}
		if (!Implies(IsPassable(coord, (dir + DIR_RIGHT) % DIR_COUNT), IsPassable(coord, (dir + DIR_RIGHT_DOWN) % DIR_COUNT)))
		{
			dirs = AddDirectionToSet(dirs, (dir + DIR_RIGHT) % DIR_COUNT);
		}
	}
	else
	{
		if (!IsPassable(coord, dir)) { return dirs; }
		// 다음 위치에서 왼쪽위는 이동가능하고, 왼쪽이 이동 불가능할경우 강제이웃 추가
		if (!Implies(IsPassable(coord, (dir + DIR_UP_LEFT) % DIR_COUNT), IsPassable(coord, (dir + DIR_LEFT) % DIR_COUNT)))
		{
			dirs = AddDirectionToSet(dirs, (dir + DIR_UP_LEFT) % DIR_COUNT);
		}
		if (!Implies(IsPassable(coord, (dir + DIR_UP_RIGHT) % DIR_COUNT), IsPassable(coord, (dir + DIR_RIGHT) % DIR_COUNT)))
		{
			dirs = AddDirectionToSet(dirs, (dir + DIR_UP_RIGHT) % DIR_COUNT);
		}
	}

	return dirs;
}

int TileManager::NaturalNeighbours(const int dir)
{
	// 시작지점은 모든 방향이 자연이웃임
	if (dir == DIR_START) return 255;

	int dirs = 0;
	dirs = AddDirectionToSet(dirs, dir);
	// 대각선인경우 대각선 좌 우 검사
	if (IsDiagonal(dir))
	{
		dirs = AddDirectionToSet(dirs, (dir + DIR_UP_RIGHT) % DIR_COUNT);
		dirs = AddDirectionToSet(dirs, (dir + DIR_UP_LEFT) % DIR_COUNT);
	}
	return dirs;
}

pair<int, int> TileManager::BitScanUpDirection(int x, int y)
{
	if (x < 0 || x >= mpCurDetailMap->GetWidth()) { return make_pair(-1, -1); }

	if (mpCurDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpCurDetailMap->GetOpenValue(x, y, false, false);
		return make_pair(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpCurDetailMap->GetCloseValue(x, y, false, false);
		int openPos = mpCurDetailMap->GetOpenValue(x, closePos, false, false);
		return make_pair(closePos + 1, openPos);
	}
}

pair<int, int> TileManager::BitScanRightDirection(int x, int y)
{
	if (y < 0 || y >= mpCurDetailMap->GetHeight()) { return make_pair(mpCurDetailMap->GetWidth(), mpCurDetailMap->GetWidth()); }

	if (mpCurDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpCurDetailMap->GetOpenValue(x, y, true, true);
		return make_pair(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpCurDetailMap->GetCloseValue(x, y, true, true);
		int openPos = mpCurDetailMap->GetOpenValue(closePos, y, true, true);
		return make_pair(closePos - 1, openPos);
	}
}

pair<int, int> TileManager::BitScanDownDirection(int x, int y)
{
	if (x < 0 || x >= mpCurDetailMap->GetWidth()) { return make_pair(mpCurDetailMap->GetHeight(), mpCurDetailMap->GetHeight()); }

	if (mpCurDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpCurDetailMap->GetOpenValue(x, y, false, true);
		return make_pair(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpCurDetailMap->GetCloseValue(x, y, false, true);
		int openPos = mpCurDetailMap->GetOpenValue(x, closePos, false, true);
		return make_pair(closePos - 1, openPos);
	}
}

pair<int, int> TileManager::BitScanLeftDirection(int x, int y)
{
	if (y < 0 || y >= mpCurDetailMap->GetHeight()) { return make_pair(-1, -1); }

	if (mpCurDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpCurDetailMap->GetOpenValue(x, y, true, false);
		return make_pair(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpCurDetailMap->GetCloseValue(x, y, true, false);
		int openPos = mpCurDetailMap->GetOpenValue(closePos, y, true, false);
		return make_pair(closePos + 1, openPos);
	}
}

TileCoord TileManager::FindJumpPoint(const TileCoord& coord, const char dir)
{
	TileCoord nextCoord = TileCoord::NextCoordinate(coord, dir);
	bool isDiag = IsDiagonal(dir);
	if (isDiag && !IsPassable(coord, (dir + DIR_UP_LEFT) % DIR_COUNT) && !IsPassable(coord, (dir + DIR_UP_RIGHT) % DIR_COUNT)) { return TileCoord(-1, -1); }
	TileCoord offset(0, 0);
	offset = TileCoord::NextCoordinate(offset, dir);

	// 점프포인트를 찾을때까지 탐색을 함.
	// 최적화를 해야 한다면 여기서 카운트를 해서 일정량 이상 탐색을 하면 다음 프레임까지 길찾기를 대기해야 함.
	while (true)
	{
		// 이동이 불가능할 경우 (-1, -1) 반환
		if (!IsPassable(nextCoord)) { return TileCoord(-1, -1); }

		// 점프포인트의 조건을 만족할 경우 경로 반환
		if (ForcedNeighbours(nextCoord, dir) || (*mpCurEndPoint) == nextCoord) { return nextCoord; }

		if (isDiag)
		{
			TileCoord newPoint(-1, -1);
			if (ScanJumpPoint(nextCoord, (dir + DIR_UP_LEFT) % DIR_COUNT, newPoint))
			{
				return nextCoord;
			}
			if (ScanJumpPoint(nextCoord, (dir + DIR_UP_RIGHT) % DIR_COUNT, newPoint))
			{
				return nextCoord;
			}
			// 막혀있는 지형은 못지나감
			if (!IsPassable(nextCoord, (dir + DIR_UP_LEFT) % DIR_COUNT) && !IsPassable(nextCoord, (dir + DIR_UP_RIGHT) % DIR_COUNT)) { return TileCoord(-1, -1); }

			// 점프포인트를 못찾으면 대각선 이동
			nextCoord.Add(offset);
		}
		else
		{
			TileCoord newPoint(-1, -1);
			ScanJumpPoint(nextCoord, dir, newPoint);
			return newPoint;
		}
	}

	return TileCoord(-1, -1);
}

bool TileManager::ScanJumpPoint(TileCoord coord, const char dir, TileCoord& jumpPoint)
{
	coord = TileCoord::NextCoordinate(coord, dir);

	if (!IsPassable(coord)) { return false; }

	bool result = false;
	pair<int, int> up;
	pair<int, int> center;
	pair<int, int> down;

	switch (dir)
	{
	case DIR_UP:
		up = BitScanUpDirection(coord.GetX() - 1, coord.GetY());
		center = BitScanUpDirection(coord.GetX(), coord.GetY());
		down = BitScanUpDirection(coord.GetX() + 1, coord.GetY());

		// 목적지에 도착
		if (coord.GetX() == mpCurEndPoint->GetX() && coord.GetY() >= mpCurEndPoint->GetY() && center.first <= mpCurEndPoint->GetY())
		{
			jumpPoint = (*mpCurEndPoint);
			return true;
		}
		if (down.first != -1 && ((down.second > -1 && down.first > center.first && down.second + 2 > center.first) || (down.first == down.second && down.first + 2 > center.first)))
		{
			jumpPoint = TileCoord(coord.GetX(), down.second + 1);
			result = true;
		}
		if (up.first != -1 && ((up.second > -1 && up.first > center.first && up.second + 2 > center.first) || (up.first == up.second && up.first + 2 > center.first)))
		{
			jumpPoint = TileCoord(coord.GetX(), result ? max(jumpPoint.GetY(), up.second + 1) : up.second + 1);
			return true;
		}
		return result;
	case DIR_RIGHT:
		up = BitScanRightDirection(coord.GetX(), coord.GetY() - 1);
		center = BitScanRightDirection(coord.GetX(), coord.GetY());
		down = BitScanRightDirection(coord.GetX(), coord.GetY() + 1);

		// 목적지에 도착
		if (coord.GetY() == mpCurEndPoint->GetY() && coord.GetX() <= mpCurEndPoint->GetX() && center.first >= mpCurEndPoint->GetX())
		{
			jumpPoint = (*mpCurEndPoint);
			return true;
		}
		if (down.first != mpCurDetailMap->GetWidth() && ((down.second < mpCurDetailMap->GetWidth() && down.first < center.first && down.second - 2 < center.first) || (down.first == down.second && down.first - 2 < center.first)))
		{
			jumpPoint = TileCoord(down.second - 1, coord.GetY());
			result = true;
		}
		if (up.first != mpCurDetailMap->GetWidth() && ((up.second < mpCurDetailMap->GetWidth() && up.first < center.first && up.second - 2 < center.first) || (up.first == up.second && up.first - 2 < center.first)))
		{
			jumpPoint = TileCoord(result ? min(jumpPoint.GetX(), up.second - 1) : up.second - 1, coord.GetY());
			return true;
		}
		return result;
	case DIR_DOWN:
		up = BitScanDownDirection(coord.GetX() - 1, coord.GetY());
		center = BitScanDownDirection(coord.GetX(), coord.GetY());
		down = BitScanDownDirection(coord.GetX() + 1, coord.GetY());

		// 목적지에 도착
		if (coord.GetX() == mpCurEndPoint->GetX() && coord.GetY() <= mpCurEndPoint->GetY() && center.first >= mpCurEndPoint->GetY())
		{
			jumpPoint = (*mpCurEndPoint);
			return true;
		}
		if (down.first != mpCurDetailMap->GetHeight() && ((down.second < mpCurDetailMap->GetHeight() && down.first < center.first && down.second - 2 < center.first) || (down.first == down.second && down.first - 2 < center.first)))
		{
			jumpPoint = TileCoord(coord.GetX(), down.second - 1);
			result = true;
		}
		if (up.first != mpCurDetailMap->GetHeight() && ((up.second < mpCurDetailMap->GetHeight() && up.first < center.first && up.second - 2 < center.first) || (up.first == up.second && up.first - 2 < center.first)))
		{
			jumpPoint = TileCoord(coord.GetX(), result ? min(jumpPoint.GetY(), up.second - 1) : up.second - 1);
			return true;
		}
		return result;
	case DIR_LEFT:
		up = BitScanLeftDirection(coord.GetX(), coord.GetY() - 1);
		center = BitScanLeftDirection(coord.GetX(), coord.GetY());
		down = BitScanLeftDirection(coord.GetX(), coord.GetY() + 1);

		// 목적지에 도착
		if (coord.GetY() == mpCurEndPoint->GetY() && coord.GetX() >= mpCurEndPoint->GetX() && center.first <= mpCurEndPoint->GetX())
		{
			jumpPoint = (*mpCurEndPoint);
			return true;
		}
		if (down.first != -1 && ((down.second > -1 && down.first > center.first && down.second + 2 > center.first) || (down.first == down.second && down.first + 2 > center.first)))
		{
			jumpPoint = TileCoord(down.second + 1, coord.GetY());
			result = true;
		}
		if (up.first != -1 && ((up.second > -1 && up.first > center.first && up.second + 2 > center.first) || (up.first == up.second && up.first + 2 > center.first)))
		{
			jumpPoint = TileCoord(result ? max(jumpPoint.GetX(), up.second + 1) : up.second + 1, coord.GetY());
			return true;
		}
		return result;
	}

	return false;
}

bool TileManager::IsNearCoord(const TileCoord& startCoord, const TileCoord& endCoord, TileCoord& outCoord, const char dir, int& nearDist)
{
	bool isDiagonal = IsDiagonal(dir);
	TileCoord curCoord;
	int dist = 0;

	if (isDiagonal)
	{
		curCoord = endCoord;
		TileCoord newCoord;
		bool result = false;
		for (int i = 0; i < nearDist; ++i)
		{
			if (IsTilePassable(curCoord))
			{
				dist = GetCoordinateDistance(startCoord, curCoord);
				if (dist < nearDist)
				{
					nearDist = dist;
					outCoord.SetX(curCoord.GetX());
					outCoord.SetY(curCoord.GetY());
					return true;
				}
				return false;
			}

			if (ScanOpenCoord(curCoord, (dir + DIR_UP_RIGHT) % DIR_COUNT, newCoord))
			{
				dist = GetCoordinateDistance(startCoord, newCoord);
				if (dist < nearDist)
				{
					nearDist = dist;
					outCoord.SetX(newCoord.GetX());
					outCoord.SetY(newCoord.GetY());
					result = true;
				}
			}
			if (ScanOpenCoord(curCoord, (dir + DIR_UP_LEFT) % DIR_COUNT, newCoord))
			{
				dist = GetCoordinateDistance(startCoord, newCoord);
				if (dist < nearDist)
				{
					nearDist = dist;
					outCoord.SetX(newCoord.GetX());
					outCoord.SetY(newCoord.GetY());
					result = true;
				}
			}

			if (result)
			{
				return true;
			}
			else
			{
				curCoord = TileCoord::NextCoordinate(curCoord, dir);
			}
		}
	}
	else
	{
		if (!ScanOpenCoord(endCoord, dir, curCoord))
		{
			return false;
		}
		else
		{
			dist = GetCoordinateDistance(startCoord, curCoord);
			if (dist < nearDist)
			{
				nearDist = dist;
				outCoord.SetX(curCoord.GetX());
				outCoord.SetY(curCoord.GetY());
				return true;
			}
		}
	}

	return true;
}

bool TileManager::ScanOpenCoord(const TileCoord& coord, const char dir, TileCoord& result)
{
	TileCoord nextCoord;
	switch (dir)
	{
	case DIR_UP:
		nextCoord = TileCoord(coord.GetX(), mpCurDetailMap->GetOpenValue(coord.GetX(), coord.GetY(), false, false));
		break;
	case DIR_DOWN:
		nextCoord = TileCoord(coord.GetX(), mpCurDetailMap->GetOpenValue(coord.GetX(), coord.GetY(), false, true));
		break;
	case DIR_LEFT:
		nextCoord = TileCoord(mpCurDetailMap->GetOpenValue(coord.GetX(), coord.GetY(), true, false), coord.GetY());
		break;
	case DIR_RIGHT:
		nextCoord = TileCoord(mpCurDetailMap->GetOpenValue(coord.GetX(), coord.GetY(), true, true), coord.GetY());
		break;
	}

	if (IsCoordinateInTileMap(nextCoord))
	{
		result.SetX(nextCoord.GetX());
		result.SetY(nextCoord.GetY());
		return true;
	}
	return false;
}

void TileManager::CreateDetailPath(const TileNode::SharedPtr& pCurNode, const TileCoord* pEndNode, list<TileCoord>& result)
{
	TileNode::SharedPtr pTraceNode = pCurNode;
	TileCoord prevCoord;
	TileCoord curCoord;
	if (pEndNode)
	{
		prevCoord = (*mpCurEndPoint);
		curCoord = pTraceNode->GetPosition();
	}
	else
	{
		prevCoord = pCurNode->GetPosition();
	}
	TileCoord nextCoord;
	int curDir = GetCoordinateDir(prevCoord, curCoord);

	if (pEndNode)
	{
		TileCoord endCoord;
		endCoord.SetX(pEndNode->GetX());
		endCoord.SetY(pEndNode->GetY());
		result.push_front(endCoord);
	}

	while (pTraceNode)
	{
		if (!pTraceNode->GetParent() || curDir != GetCoordinateDir(pTraceNode->GetPosition(), pTraceNode->GetParent()->GetPosition()))
		{
			int nextDir = curDir;
			if (pTraceNode->GetParent()) { nextDir = GetCoordinateDir(pTraceNode->GetPosition(), pTraceNode->GetParent()->GetPosition()); }
			curCoord = pTraceNode->GetPosition();
			bool isDiagonal = IsDiagonal(curDir);
			while (prevCoord != curCoord)
			{
				if (isDiagonal)
				{
					bool bPassableLeft = IsPassable(prevCoord, (curDir + DIR_UP_LEFT) % DIR_COUNT);
					bool bPassableRight = IsPassable(prevCoord, (curDir + DIR_UP_RIGHT) % DIR_COUNT);
					if (!bPassableLeft)
					{
						nextCoord = TileCoord::NextCoordinate(prevCoord, (curDir + DIR_UP_RIGHT) % DIR_COUNT);
						result.push_front(nextCoord);
					}
					else if (!bPassableRight)
					{
						nextCoord = TileCoord::NextCoordinate(prevCoord, (curDir + DIR_UP_LEFT) % DIR_COUNT);
						result.push_front(nextCoord);
					}
				}
				prevCoord = TileCoord::NextCoordinate(prevCoord, curDir);
				result.push_front(prevCoord);
			}
			curDir = nextDir;
		}
		pTraceNode = pTraceNode->GetParent();
	}
}

void TileManager::GetTileCoord(const Vector2& pos, const eUnitTileSize& unitSize, TileCoord& leftTop, TileCoord& rightBottom)
{
	int halfSize = 0;

	switch (unitSize)
	{
	case eUnitTileSize::Small:
		halfSize = HALF_SMALL_UNIT_TILE_SIZE;
		break;
	case eUnitTileSize::Big:
		halfSize = HALF_BIG_UNIT_TILE_SIZE;
		break;
	}

	GetTileCoordByPosition(pos, leftTop);

	rightBottom.SetX(leftTop.GetX() + 1);
	rightBottom.SetY(leftTop.GetY() + 1);

	--halfSize;
	leftTop.SetX(leftTop.GetX() - halfSize);
	leftTop.SetY(leftTop.GetY() - halfSize);
	rightBottom.SetX(rightBottom.GetX() + halfSize);
	rightBottom.SetY(rightBottom.GetY() + halfSize);


	if (leftTop.GetX() < 0)
	{
		leftTop.SetX(0);
	}
	if (leftTop.GetY() < 0)
	{
		leftTop.SetY(0);
	}

	if (rightBottom.GetX() >= mTileWidth)
	{
		rightBottom.SetX(mTileWidth - 1);
	}
	if (rightBottom.GetY() >= mTileHeight)
	{
		rightBottom.SetY(mTileHeight - 1);
	}
}

void TileManager::GetTileCoordByPosition(const Vector2& pos, TileCoord& coord)
{
	div_t dv;

	dv = div((int)pos.x + HALF_TILE_SIZE, TILE_SIZE);
	if (dv.rem < HALF_TILE_SIZE) { --dv.quot; }

	coord.SetX(dv.quot);

	dv = div((int)pos.y + HALF_TILE_SIZE, TILE_SIZE);
	if (dv.rem < HALF_TILE_SIZE) { --dv.quot; }

	coord.SetY(dv.quot);
}

void TileManager::SetTileState(const Vector2& pos, const eUnitTileSize& unitSize, bool isObstacle, bool isSet)
{
	if (mbIsReleased) { return; }
	TileCoord leftTop;
	TileCoord rightBottom;

	GetTileCoord(pos, unitSize, leftTop, rightBottom);

	int width = rightBottom.GetX() - leftTop.GetX() + 1;
	int height = rightBottom.GetY() - leftTop.GetY() + 1;

	int offset = isSet ? 1 : -1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (isObstacle)
			{
				mVecNormalTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].ObstacleCount += offset;
			}
			else
			{
				mVecNormalTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].OccupiedCount += offset;
			}
		}
	}

	TileData* pTileData = nullptr;
	leftTop.Add(TileCoord(-1, -1));
	width += HALF_SMALL_UNIT_TILE_SIZE;
	height += HALF_SMALL_UNIT_TILE_SIZE;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (leftTop.GetX() + x < 0 || leftTop.GetX() + x >= mTileWidth || leftTop.GetY() + y < 0 || leftTop.GetY() + y >= mTileHeight)
			{
				continue;
			}

			pTileData = &mVecSmallTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth];

			if (isObstacle) { pTileData->ObstacleCount += offset; }
			else { pTileData->OccupiedCount += offset; }

			if (isSet)
			{
				if (pTileData->IsObstacle() || pTileData->IsOccupied())
				{
					mpSmallUnitMap->SetAt(leftTop.GetX() + x, leftTop.GetY() + y);
				}
			}
			else
			{
				if (!pTileData->IsObstacle() || pTileData->IsOccupied())
				{
					mpSmallUnitMap->ClrAt(leftTop.GetX() + x, leftTop.GetY() + y);
				}
			}
		}
	}

	leftTop.Add(TileCoord(-1, -1));
	width += HALF_BIG_UNIT_TILE_SIZE;
	height += HALF_BIG_UNIT_TILE_SIZE;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (leftTop.GetX() + x < 0 || leftTop.GetX() + x >= mTileWidth || leftTop.GetY() + y < 0 || leftTop.GetY() + y >= mTileHeight)
			{
				continue;
			}

			pTileData = &mVecBigTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth];

			if (isObstacle) { pTileData->ObstacleCount += offset; }
			else { pTileData->OccupiedCount += offset; }

			if (isSet)
			{
				if (pTileData->IsObstacle() || pTileData->IsOccupied())
				{
					mpBigUnitMap->SetAt(leftTop.GetX() + x, leftTop.GetY() + y);
				}
			}
			else
			{
				if (!pTileData->IsObstacle() || pTileData->IsOccupied())
				{
					mpBigUnitMap->ClrAt(leftTop.GetX() + x, leftTop.GetY() + y);
				}
			}
		}
	}
#ifdef  DEBUG_MODE
	InitGizmoColor();
#endif // DEBUG_MODE
}
