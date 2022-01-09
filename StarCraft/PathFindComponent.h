#pragma once
#include "Component.h"
#include "JPS.h"

static const int UNIT_MOVE_SPEED = 100;
static const int DISTANCE_EPSILON = 2;

class DetailMap;
class BitArray;
class TransformComponent;
class PathFindComponent : public Component
{
private:
	const int ARRIVED_TILE_COUNT = 4;

	const Fix MAX_WAITING_TIME = (Fix)0.25f;
	TransformComponent* mpTransform = nullptr;

	BitArray* mpSearched = nullptr;
	JumpPointHeap* mpJumpPoint = nullptr;
	TileNode::SharedPtr mpNearTileNode = nullptr;
	TileCoord* mpEndPos = nullptr;

	bool mbIsSearching = false;
	bool mbIsBeginSearching = true;

	bool mbIsFollowPath = false;
	bool mbIsCorrectPath = false;
	bool mbIsHaveTempPath = false;

	int mWidth = 0;
	int mHeight = 0;

	bool mbIsWaiting = false;
	Fix mWaitingTime = 0;

	eUnitTileSize mUnitSize;

	list<TileCoord> mPath;
	list<TileCoord> mTempPath;
	vector<TileCoord> mPassedPath;
	Vector2 mNextPath;
	Vector2 mTargetPosition;

	float mLookAngle = 0.0f;

	bool mbIsObstacle = false;
	bool mbIsOccupied = false;
public:
	PathFindComponent(eUnitTileSize size);
	virtual ~PathFindComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	void FindPath(Vector2 targetPos);

	inline bool IsMoving() { return mbIsFollowPath; }
	inline bool IsArrived() { return mbIsSearching == false && mbIsWaiting == false && mbIsFollowPath == false; }
	inline void Stop() { SetObstacle(); mbIsBeginSearching = true; mbIsWaiting = false;  mbIsSearching = false; mbIsFollowPath = false; }
	inline float GetAngle() { return mLookAngle; }

private:
	void SetObstacle();
	void SetOpen();
	void SetOccupy();

	void KeepFinding();

	void FindPathAgain();
	void WaitingStart();

	bool GetNextPath(Vector2& nextPath);
	bool IsCloserAtNextPath();
};

