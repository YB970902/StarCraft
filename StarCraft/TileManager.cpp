#include "stdafx.h"
#include "TileManager.h"
#include "BitArray.h"
#include "DetailMap.h"
#include "RectGizmo.h"
#include "RenderManager.h"

void TileManager::Init()
{
	mpTerrainMap = new DetailMap();
	mpSmallUnitMap = new DetailMap;
	mpBigUnitMap = new DetailMap;
}

void TileManager::Release()
{
	SAFE_RELEASE(mpTerrainMap);
	SAFE_RELEASE(mpSmallUnitMap);
	SAFE_RELEASE(mpBigUnitMap);
}

void TileManager::LoadTileMap()
{
	// 이미 저장된 맵을 받아야 함
	// 
	// 임의값 지정
	mTileWidth = 100;
	mTileHeight = 100;
	mVecPathChecker.resize(mTileWidth * mTileHeight);
	mpTerrainMap->Init(mTileWidth, mTileHeight);
	mpSmallUnitMap->Init(mTileWidth, mTileHeight);
	mpBigUnitMap->Init(mTileWidth, mTileHeight);
	mVecTileState.resize(mTileWidth * mTileHeight);
	mVecGizmo.resize(mTileWidth * mTileHeight);

	// 임의로 장애물 생성
	srand(0);

	mpCurDetailMap = mpBigUnitMap;

	for (int x = 0; x < mTileWidth; ++x)
	{
		for (int y = 0; y < mTileHeight; ++y)
		{
			mVecGizmo[x + y * mTileWidth] = (RectGizmo*)RENDER->RenderRect(Vector2(x * TILE_SIZE, y * TILE_SIZE), Vector2(TILE_SIZE - 1, TILE_SIZE - 1), 1.0f, TILE_DEFAULT);
		}
	}

	for (int x = 0; x < mTileWidth; ++x)
	{
		for (int y = 0; y < mTileHeight; ++y)
		{
			// 시작위치는 장애물이 생성되지 않도록 설정
			if (x == 0 && y == 0) { continue; }
			if (rand() % 50 == 0)
			{
				mpTerrainMap->SetAt(x, y);
				for (int i = 0; i < SMALL_UNIT_TILE_SIZE; ++i)
				{
					for (int j = 0; j < SMALL_UNIT_TILE_SIZE; ++j)
					{
						mpSmallUnitMap->SetAt(x - i, y - j);
					}
				}
				for (int i = 0; i < BIG_UNIT_TILE_SIZE; ++i)
				{
					for (int j = 0; j < BIG_UNIT_TILE_SIZE; ++j)
					{
						mpBigUnitMap->SetAt(x - i + 1, y - j + 1);
						SetGizmoColor(x - i + 1, y - j + 1, TILE_OBSTACLE);
					}
				}
				++mVecTileState[x + y * mTileWidth].ObstacleCount;
				//SetGizmoColor(x, y, TILE_OBSTACLE);
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

bool TileManager::BeginSearch(Vector2 startPos, TileCoord* pEndPoint, const eUnitTileSize& unitSize, JumpPointHeap* pJumpPoint, BitArray* pSearched, list<TileCoord>& result, TileNode** ppNearNode)
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
	InitGizmoColor();

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

	(*ppNearNode) = startNode.get();

	return KeepSearch(pEndPoint, unitSize, pJumpPoint, pSearched, result, ppNearNode);
}

bool TileManager::KeepSearch(TileCoord* pEndPoint, const eUnitTileSize& unitSize, JumpPointHeap* pJumpPoint, BitArray* pSearched, list<TileCoord>& result, TileNode** ppNearNode)
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
			(*ppNearNode) = pCurNode.get();
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

void TileManager::CreateDetailTempPath(TileNode* pNearNode, list<TileCoord>& result)
{
	TileNode::SharedPtr nearNode = make_shared<TileNode>();
	nearNode->SetParent(pNearNode->GetParent());
	nearNode->SetPosition(pNearNode->GetPosition());
	nearNode->SetDirection(pNearNode->GetDirection());
	CreateDetailPath(nearNode, nullptr, result);
}

void TileManager::ConnectPath(vector<TileCoord>& passedPath, list<TileCoord>& correctPath)
{
	fill(mVecPathChecker.begin(), mVecPathChecker.end(), -1);

	TileCoord prevCoord = passedPath.front();

	for(int i = 0; i < passedPath.size(); ++i)
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
		index = mVecPathChecker[curCoord.GetX() + curCoord.GetY() * mTileWidth];
		dv = div(index, mTileWidth);
		correctPath.push_back(curCoord);
		curCoord = TileCoord(dv.rem, dv.quot);
	}
	correctPath.push_back(curCoord);
}

void TileManager::SetOccupyTile(const Vector2& pos, const eUnitTileSize& unitSize, bool set)
{
	TileCoord leftTop;
	TileCoord rightBottom;

	GetTileCoord(pos, unitSize, leftTop, rightBottom);

	int width = rightBottom.GetX() - leftTop.GetX() + 1;
	int height = rightBottom.GetY() - leftTop.GetY() + 1;

	int offset = set ? 1 : -1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			mVecTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].OccupiedCount += offset;
		}
	}

	InitGizmoColor();
}

void TileManager::SetObstacleTile(const Vector2& pos, const eUnitTileSize& unitSize, bool set)
{
	TileCoord leftTop;
	TileCoord rightBottom;

	GetTileCoord(pos, unitSize, leftTop, rightBottom);

	int width = rightBottom.GetX() - leftTop.GetX() + 1;
	int height = rightBottom.GetY() - leftTop.GetY() + 1;

	int offset = set ? 1 : -1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			mVecTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].ObstacleCount += offset;
		}
	}

	InitGizmoColor();
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
			if (mVecTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].IsObstacle())
			{
				state = eTileState::Obstacle;
				return false;
			}
			else if (mVecTileState[(leftTop.GetX() + x) + (leftTop.GetY() + y) * mTileWidth].IsOccupied())
			{
				state = eTileState::Occupied;
				result = false;
			}
		}
	}

	return result;
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
			//if (mpCurDetailMap->IsCollision(x, y))
			if (mVecTileState[x + y * mTileWidth].IsObstacle())
			{
				mVecGizmo[x + y * mTileWidth]->SetColor(TILE_OBSTACLE);
			}
			else if (mVecTileState[x + y * mTileWidth].IsOccupied())
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

bool TileManager::IsPassable(const TileCoord& coord)
{
	return !mpCurDetailMap->IsCollision(coord.GetX(), coord.GetY());
}

bool TileManager::IsPassable(const TileCoord& coord, const int dir)
{
	return IsPassable(TileCoord::NextCoordinate(coord, dir));
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
			if (IsPassable(curCoord))
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
