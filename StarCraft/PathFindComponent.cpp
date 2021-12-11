#include "stdafx.h"
#include "PathFindComponent.h"
#include "TileManager.h"
#include "DetailMap.h"
#include "BitArray.h"
#include "JPS.h"
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
		KeepFinding();
	}
	if (mbIsWaiting)
	{
		Vector2 dir = (mNextPath - mpTransform->GetPosition()).Normalize();
		Vector2 nextPosition = mpTransform->GetPosition() + dir * 100 * DELTA_TIME;
		TileManager::eTileState tileState;
		if (TILE->IsTileOpen(nextPosition, mUnitSize, tileState))
		{
			mbIsWaiting = false;
		}
		else
		{
			switch (tileState)
			{
			case TileManager::eTileState::Obstacle:
				FindPathAgain();
				break;
			case TileManager::eTileState::Occupied:
				mWaitingTime += DELTA_TIME;
				if (mWaitingTime >= MAX_WAITING_TIME)
				{
					FindPathAgain();
				}
				break;
			}
		}
	}
	else if (mbIsFollowPath)
	{
		Vector2 dir = (mNextPath - mpTransform->GetPosition()).Normalize();
		Vector2 nextPosition = mpTransform->GetPosition() + dir * 100 * DELTA_TIME;

		TileManager::eTileState tileState;
		SetMove();
		if (TILE->IsTileOpen(nextPosition, mUnitSize, tileState))
		{
			mpTransform->SetPosition(nextPosition);
		}
		else
		{
			switch (tileState)
			{
			case TileManager::eTileState::Obstacle:
				FindPathAgain();
				return;
			case TileManager::eTileState::Occupied:
				WaitingStart();
				return;
			}
		}
		SetPause();

		if (Vector2::GetDistance(mpTransform->GetPosition(), mNextPath) < (Fix)1)
		{
			SetMove();
			if (TILE->IsTileOpen(mNextPath, mUnitSize, tileState))
			{
				mpTransform->SetPosition(mNextPath);
			}
			else
			{
				switch (tileState)
				{
				case TileManager::eTileState::Obstacle:
					FindPathAgain();
					return;
				case TileManager::eTileState::Occupied:
					WaitingStart();
					return;
				}
			}
			SetPause();
			if (mbIsCorrectPath)
			{
				if (mPath.empty())
				{
					SetStop();
					mbIsFollowPath = false;
				}
				else
				{
					TILE->GetPositionByTileCoord(mPath.front(), mNextPath);
					mPath.pop_front();
				}
			}
			else
			{
				if (mTempPath.empty())
				{
					SetStop();
					mbIsFollowPath = false;
				}
				else
				{
					TILE->GetPositionByTileCoord(mTempPath.front(), mNextPath);
					mPassedPath.push_back(mTempPath.front());
					mTempPath.pop_front();
				}
			}
		}
	}
}

void PathFindComponent::SetStop()
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

void PathFindComponent::SetMove()
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

void PathFindComponent::SetPause()
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
	TileCoord prevEndPos;
	prevEndPos.SetX(mpEndPos->GetX());
	prevEndPos.SetY(mpEndPos->GetY());

	if (!TILE->GetEndPosition(mpTransform->GetPosition(), targetPos, mUnitSize, mpEndPos)) { return; }
	if (prevEndPos == *mpEndPos) return;

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

	// ��ã�⿡ ������ ���
	if (result)
	{
		mbIsFollowPath = true;
		mbIsSearching = false;
		mbIsCorrectPath = true;

		// ��� ������ Ž�������� �������� ���� ��θ� ��ã�� ��쿣
		// Ž���ߴ� ��ε��� ���� �������� ����� ����� �θ� ���� �ӽ� ��θ� �����.
		if (mPath.empty())
		{
			TILE->CreateDetailTempPath(mpNearTileNode, mPath);
		}

		// �ӽ� ��θ� ���󰡰� �ִ���쿣
		// ���ݱ��� ���󰬴� ��ο� Ž���� ��θ� �̾��ش�.
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
		if (mPassedPath.empty())
		{
			mbIsFollowPath = true;
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

	SetMove();

	mbIsWaiting = false;
	mbIsCorrectPath = false;
	mbIsFollowPath = false;
	mbIsSearching = true;
	mbIsBeginSearching = true;
}

void PathFindComponent::WaitingStart()
{
	SetPause();
	mbIsWaiting = true;
	mWaitingTime = 0;
}
