#include "stdafx.h"
#include "PathFinder.h"
#include "TileManager.h"
#include "DetailMap.h"
#include "BitArray.h"

PathFinder::PathFinder()
{
	mpSearched = new BitArray();
}

PathFinder::~PathFinder()
{

}

void PathFinder::Init(DetailMap* pDetailMap)
{
	mpDetailMap = pDetailMap;
	mpSearched->Init(mpDetailMap->GetWidth(), mpDetailMap->GetHeight());
}

void PathFinder::Release()
{
	delete mpSearched;
}

bool PathFinder::FindPath(int sX, int sY, int eX, int eY, list<TileCoord>& result)
{
	if (sX < 0 || sX >= mpDetailMap->GetWidth()
		|| eY < 0 || eY >= mpDetailMap->GetHeight())
	{
		return false;
	}

	mEndPos.SetX(eX);
	mEndPos.SetY(eY);

	result.clear();
	mJumpPoint.Clear();
	mpSearched->Clear();

	TileNode::SharedPtr pStartNode = make_shared<TileNode>();
	pStartNode->Set(nullptr, TileCoord(sX, sY), mEndPos, DIR_START);

	mJumpPoint.Push(pStartNode);
	mpSearched->SetAt(pStartNode->GetPositionX(), pStartNode->GetPositionY(), true);

	while (mJumpPoint.IsEmpty() == false)
	{
		TileNode::SharedPtr pCurNode = mJumpPoint.Pop();
		int direction = ForcedNeighbours(pCurNode->GetPosition(), pCurNode->GetDirection()) | NaturalNeighbours(pCurNode->GetDirection());

		for (int dir = 0; dir < DIR_COUNT; ++dir)
		{
			//if (mCount >= 100) { return false; }
			// 검사가 가능한 방향인경우
			if ((1 << dir) & direction)
			{
				TileCoord newCoord = FindJumpPoint(pCurNode->GetPosition(), dir);
				if (!newCoord.IsEmpty())
				{
					if (newCoord == mEndPos)
					{
						result.push_front(mEndPos);

						TileNode::SharedPtr pTraceNode = pCurNode;
						int curDir = GetCoordinateDir(mEndPos, pCurNode->GetPosition());
						while (pTraceNode)
						{
							int nextDir = 0;
							if (pTraceNode->GetParent()) { nextDir = GetCoordinateDir(pTraceNode->GetPosition(), pTraceNode->GetParent()->GetPosition()); }
							if (curDir != nextDir) { result.push_front(pTraceNode->GetPosition()); }
							pTraceNode = pTraceNode->GetParent();
							curDir = nextDir;
						}
						return true;
					}

					TileNode::SharedPtr pNewNode = make_shared<TileNode>();
					pNewNode->Set(pCurNode, newCoord, mEndPos, dir);

					if (!mpSearched->IsSet(newCoord.GetX(), newCoord.GetY()))
					{
						mJumpPoint.Push(pNewNode);
						mpSearched->SetAt(newCoord.GetX(), newCoord.GetY(), true);
					}
					else
					{
						mJumpPoint.PushSmaller(pNewNode);
					}
				}
			}
		}
	}

	return false;
}

bool PathFinder::IsPassable(const TileCoord& coord)
{
	return !mpDetailMap->IsCollision(coord.GetX(), coord.GetY());
}

bool PathFinder::IsPassable(const TileCoord& coord, const int dir)
{
	return IsPassable(TileCoord::NextCoordinate(coord, dir));
}

int PathFinder::ForcedNeighbours(const TileCoord& coord, const int dir)
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
		if (!IsPassable(coord, 0)) { return dirs; }
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

int PathFinder::NaturalNeighbours(const int dir)
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

TileCoord PathFinder::FindJumpPoint(const TileCoord& coord, const char dir)
{
	TileCoord nextCoord = TileCoord::NextCoordinate(coord, dir);
	bool isDiag = IsDiagonal(dir);
	if(isDiag && !IsPassable(coord, (dir + DIR_UP_LEFT) % DIR_COUNT) && !IsPassable(coord, (dir + DIR_UP_RIGHT) % DIR_COUNT)) { return TileCoord(-1, -1); }
	TileCoord offset(0, 0);
	offset = TileCoord::NextCoordinate(offset, dir);

	// 점프포인트를 찾을때까지 탐색을 함.
	// 최적화를 해야 한다면 여기서 카운트를 해서 일정량 이상 탐색을 하면 다음 프레임까지 길찾기를 대기해야 함.
	while (true)
	{
		// 이동이 불가능할 경우 (-1, -1) 반환
		if (!IsPassable(nextCoord)) { return TileCoord(-1, -1); }

		// 점프포인트의 조건을 만족할 경우 경로 반환
		if (ForcedNeighbours(nextCoord, dir) || mEndPos == nextCoord) { return nextCoord; }

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

bool PathFinder::ScanJumpPoint(TileCoord coord, const char dir, TileCoord& jumpPoint)
{
	coord = TileCoord::NextCoordinate(coord, dir);

	if (!IsPassable(coord)) { return false; }

	bool result = false;
	ScanResult up;
	ScanResult center;
	ScanResult down;

	switch (dir)
	{
	case DIR_UP:
		up = BitScanUpDirection(coord.GetX() - 1, coord.GetY());
		center = BitScanUpDirection(coord.GetX(), coord.GetY());
		down = BitScanUpDirection(coord.GetX() + 1, coord.GetY());

		// 목적지에 도착
		if (coord.GetX() == mEndPos.GetX() && coord.GetY() >= mEndPos.GetY() && center.first <= mEndPos.GetY())
		{
			jumpPoint = mEndPos;
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
		if (coord.GetY() == mEndPos.GetY() && coord.GetX() <= mEndPos.GetX() && center.first >= mEndPos.GetX())
		{
			jumpPoint = mEndPos;
			return true;
		}
		if (down.first != mpDetailMap->GetWidth() && ((down.second < mpDetailMap->GetWidth() && down.first < center.first && down.second - 2 < center.first) || (down.first == down.second && down.first - 2 < center.first)))
		{
			jumpPoint = TileCoord(down.second - 1, coord.GetY());
			result = true;
		}
		if (up.first != mpDetailMap->GetWidth() && ((up.second < mpDetailMap->GetWidth() && up.first < center.first && up.second - 2 < center.first) || (up.first == up.second && up.first - 2 < center.first)))
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
		if (coord.GetX() == mEndPos.GetX() && coord.GetY() <= mEndPos.GetY() && center.first >= mEndPos.GetY())
		{
			jumpPoint = mEndPos;
			return true;
		}
		if (down.first != mpDetailMap->GetHeight() && ((down.second < mpDetailMap->GetHeight() && down.first < center.first && down.second - 2 < center.first) || (down.first == down.second && down.first - 2 < center.first)))
		{
			jumpPoint = TileCoord(coord.GetX(), down.second - 1);
			result = true;
		}
		if (up.first != mpDetailMap->GetHeight() && ((up.second < mpDetailMap->GetHeight() && up.first < center.first && up.second - 2 < center.first) || (up.first == up.second && up.first - 2 < center.first)))
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
		if (coord.GetY() == mEndPos.GetY() && coord.GetX() >= mEndPos.GetX() && center.first <= mEndPos.GetX())
		{
			jumpPoint = mEndPos;
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

ScanResult PathFinder::BitScanUpDirection(int x, int y)
{
	if (x < 0 || x >= mpDetailMap->GetWidth()) { return ScanResult(-1, -1); }

	if (mpDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpDetailMap->GetOpenValue(x, y, false, false);
		return ScanResult(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpDetailMap->GetCloseValue(x, y, false, false);
		int openPos = mpDetailMap->GetOpenValue(x, closePos, false, false);
		return ScanResult(closePos + 1, openPos);
	}
}

ScanResult PathFinder::BitScanRightDirection(int x, int y)
{
	if (y < 0 || y >= mpDetailMap->GetHeight()) { return ScanResult(mpDetailMap->GetWidth(), mpDetailMap->GetWidth()); }

	if (mpDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpDetailMap->GetOpenValue(x, y, true, true);
		return ScanResult(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpDetailMap->GetCloseValue(x, y, true, true);
		int openPos = mpDetailMap->GetOpenValue(closePos, y, true, true);
		return ScanResult(closePos - 1, openPos);
	}
}

ScanResult PathFinder::BitScanDownDirection(int x, int y)
{
	if (x < 0 || x >= mpDetailMap->GetWidth()) { return ScanResult(mpDetailMap->GetHeight(), mpDetailMap->GetHeight()); }

	if (mpDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpDetailMap->GetOpenValue(x, y, false, true);
		return ScanResult(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpDetailMap->GetCloseValue(x, y, false, true);
		int openPos = mpDetailMap->GetOpenValue(x, closePos, false, true);
		return ScanResult(closePos - 1, openPos);
	}
}

ScanResult PathFinder::BitScanLeftDirection(int x, int y)
{
	if (y < 0 || y >= mpDetailMap->GetHeight()) { return ScanResult(-1, -1); }

	if (mpDetailMap->IsCollision(x, y))
	{
		// 현재 위치가 장애물이면 장애물이 없는지점을 반환합니다.
		int openPos = mpDetailMap->GetOpenValue(x, y, true, false);
		return ScanResult(openPos, openPos);
	}
	else
	{
		// 현재 위치가 장애물이 아니면 장애물이 있는지점과 없는 지점을 반환합니다.
		int closePos = mpDetailMap->GetCloseValue(x, y, true, false);
		int openPos = mpDetailMap->GetOpenValue(closePos, y, true, false);
		return ScanResult(closePos + 1, openPos);
	}
}