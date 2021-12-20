#include "stdafx.h"
#include "PathFindComponent.h"
#include "TileManager.h"
#include "DetailMap.h"
#include "BitArray.h"
#include "GameObject.h"
#include "TransformComponent.h"

PathFindComponent::PathFindComponent(eUnitTileSize size)
{
	mUnitSize = size;
	mpSearched = new BitArray;
	mpSearched->Init(TILE->GetTileWidth(), TILE->GetTileHeight());
	mpJumpPoint = new JumpPointHeap();
	mpEndPos = new TileCoord();
}

PathFindComponent::~PathFindComponent()
{
	SAFE_DELETE(mpSearched);
	SAFE_DELETE(mpJumpPoint);
	SAFE_DELETE(mpEndPos);
}

void PathFindComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpTransform = static_cast<TransformComponent*>(mpGameObject->GetComponent(eComponentTag::Transform));
}

void PathFindComponent::Release()
{
}

void PathFindComponent::Update()
{
	if (mbIsSearching)
	{
		SetOpen();
		KeepFinding();
		SetObstacle();
	}

	if (mbIsWaiting)
	{
		mWaitingTime += DELTA_TIME;
		if (mWaitingTime >= MAX_WAITING_TIME)
		{
			FindPath(mTargetPosition);
			return;
		}

		Vector2 dir = (mNextPath - mpTransform->GetPosition()).Normalize();
		Vector2 nextPosition = mpTransform->GetPosition() + dir * 100 * DELTA_TIME;
		TileManager::eTileState tileState;

		SetOpen();
		if (TILE->IsTileOpen(nextPosition, mUnitSize, tileState))
		{
			mbIsWaiting = false;
			mbIsFollowPath = true;
		}
		SetOccupy();
	}
	else if (mbIsFollowPath)
	{
		Vector2 toNextPos = (mNextPath - mpTransform->GetPosition());
		Vector2 dir = toNextPos.Normalize();
		Vector2 nextPosition = mpTransform->GetPosition() + dir * 100 * DELTA_TIME;

		mLookAngle = RAD2DEG(atan2f(-toNextPos.y, toNextPos.x));

		TileManager::eTileState tileState;
		SetOpen();
		if (TILE->IsTileOpen(nextPosition, mUnitSize, tileState))
		{
			mpTransform->SetPosition(nextPosition);
		}
		else
		{
			if ((mbIsCorrectPath && mPath.size() < ARRIVED_TILE_COUNT) ||
				(mbIsCorrectPath == false && mTempPath.size() < ARRIVED_TILE_COUNT))
			{
				Stop();
			}
			else
			{
				switch (tileState)
				{
				case TileManager::eTileState::Obstacle:
					FindPath(mTargetPosition);
					return;
				case TileManager::eTileState::Occupied:
					WaitingStart();
					return;
				}
			}
		}
		SetOccupy();

		if (IsCloserAtNextPath())
		{
			SetOpen();
			if (TILE->IsTileOpen(mNextPath, mUnitSize, tileState))
			{
				mpTransform->SetPosition(mNextPath);
				SetOccupy();
			}
			else
			{
				// 이동할 수 없는 경로인데 목적지와 가까우면 그냥 정지
				if ((mbIsCorrectPath && mPath.size() < ARRIVED_TILE_COUNT) ||
					(mbIsCorrectPath == false && mTempPath.size() < ARRIVED_TILE_COUNT))
				{
					Stop();
				}
				else
				{
					switch (tileState)
					{
					case TileManager::eTileState::Obstacle:
						FindPath(mTargetPosition);
						return;
					case TileManager::eTileState::Occupied:
						WaitingStart();
						return;
					}
				}
			}
			if (GetNextPath(mNextPath))
			{
				SetOccupy();
			}
			else
			{
				SetObstacle();
			}
		}
	}
}

void PathFindComponent::SetObstacle()
{
	if (!mbIsObstacle)
	{
		TILE->SetObstacleTile(mpTransform->GetPosition(), mUnitSize, true);
		mbIsObstacle = true;
	}
	if (mbIsOccupied)
	{
		TILE->SetOccupyTile(mpTransform->GetPosition(), mUnitSize, false);
		mbIsOccupied = false;
	}
}

void PathFindComponent::SetOpen()
{
	if (mbIsObstacle)
	{
		TILE->SetObstacleTile(mpTransform->GetPosition(), mUnitSize, false);
		mbIsObstacle = false;
	}
	if (mbIsOccupied)
	{
		TILE->SetOccupyTile(mpTransform->GetPosition(), mUnitSize, false);
		mbIsOccupied = false;
	}
}

void PathFindComponent::SetOccupy()
{
	if (mbIsObstacle)
	{
		TILE->SetObstacleTile(mpTransform->GetPosition(), mUnitSize, false);
		mbIsObstacle = false;
	}
	if (!mbIsOccupied)
	{
		TILE->SetOccupyTile(mpTransform->GetPosition(), mUnitSize, true);
		mbIsOccupied = true;
	}
}

void PathFindComponent::FindPath(Vector2 targetPos)
{
	if (!TILE->GetEndPosition(mpTransform->GetPosition(), targetPos, mUnitSize, mpEndPos)) { return; }

	mTargetPosition = targetPos;

	FindPathAgain();
}

void PathFindComponent::KeepFinding()
{
	bool result;
	if (mbIsBeginSearching)
	{
		result = TILE->BeginSearch(mpTransform->GetPosition(), mpEndPos, mUnitSize, mpJumpPoint, mpSearched, mPath, &mpNearTileNode);
		mbIsBeginSearching = false;
	}
	else
	{
		result = TILE->KeepSearch(mpEndPos, mUnitSize, mpJumpPoint, mpSearched, mPath, &mpNearTileNode);
	}

	// 길찾기에 성공한 경우
	if (result)
	{
		mbIsFollowPath = true;
		mbIsSearching = false;
		mbIsCorrectPath = true;

		// 임시 경로를 따라가고 있던경우엔
		// 지금까지 따라갔던 경로와 탐색한 경로를 이어준다.
		if (!mPassedPath.empty())
		{
			TILE->ConnectPath(mPassedPath, mPath);
		}

		TILE->GetPositionByTileCoord(mPath.front(), mNextPath);
		mPath.pop_front();
		if (!mPath.empty())
		{
			TILE->GetPositionByTileCoord(mPath.front(), mNextPath);
			mPath.pop_front();
		}
	}
	else
	{
		// 도착지로 가는 경로가 없음
		if (mpJumpPoint->IsEmpty())
		{
			if (mTempPath.empty() == false)
			{
				mbIsCorrectPath = true;
				mPath = mTempPath;
				mTempPath.clear();
				mPassedPath.clear();
			}
		}
		// 도착지로 가는 경로는 있지만, 임시 경로가 없는경우
		else if (mbIsHaveTempPath == false)
		{
			mbIsHaveTempPath = true;
			mbIsFollowPath = true;
			mbIsSearching = true;
			TILE->CreateDetailTempPath(mpNearTileNode, mTempPath);
			mPassedPath.reserve(mTempPath.size());
			mTempPath.pop_front();
			TILE->GetPositionByTileCoord(mTempPath.front(), mNextPath);
			mPassedPath.push_back(mTempPath.front());
			mTempPath.pop_front();
		}
	}

	mpNearTileNode = nullptr;
}

void PathFindComponent::FindPathAgain()
{
	mTempPath.clear();
	mPassedPath.clear();
	mPath.clear();

	SetObstacle();

	mbIsWaiting = false;
	mbIsCorrectPath = false;
	mbIsFollowPath = false;
	mbIsSearching = true;
	mbIsBeginSearching = true;
	mbIsHaveTempPath = false;
}

void PathFindComponent::WaitingStart()
{
	SetOccupy();
	mbIsWaiting = true;
	mbIsFollowPath = false;
	mWaitingTime = 0;
}

bool PathFindComponent::GetNextPath(Vector2& nextPath)
{
	if (mbIsCorrectPath)
	{
		if (mPath.empty())
		{
			mbIsFollowPath = false;
			return false;
		}
		else
		{
			TILE->GetPositionByTileCoord(mPath.front(), nextPath);
			mPath.pop_front();
			return true;
		}
	}
	else
	{
		if (mTempPath.empty())
		{
			mbIsFollowPath = false;
			return false;
		}
		else
		{
			TILE->GetPositionByTileCoord(mTempPath.front(), nextPath);
			mPassedPath.push_back(mTempPath.front());
			mTempPath.pop_front();
			return true;
		}
	}
}

bool PathFindComponent::IsCloserAtNextPath()
{
	return (Vector2::GetDistance(mpTransform->GetPosition(), mNextPath) < (Fix)1);
}
