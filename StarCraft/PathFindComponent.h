#pragma once
#include "Component.h"

class DetailMap;
class BitArray;
class JumpPointHeap;
class TileCoord;
class TileNode;
class TransformComponent;
class PathFindComponent : public Component
{
private:
	TransformComponent* mpTransform = nullptr;

	BitArray* mpSearched = nullptr;
	JumpPointHeap* mpJumpPoint = nullptr;
	TileNode* mpNearTileNode = nullptr;
	TileCoord* mpEndPos = nullptr;

	bool mbIsSearching = false;
	bool mbIsBeginSearching = true;

	bool mbIsFollowPath = false;
	bool mbIsCorrectPath = false;

	int mWidth = 0;
	int mHeight = 0;

	eUnitTileSize mUnitSize;

	list<TileCoord> mPath;
	list<TileCoord> mTempPath;
	vector<TileCoord> mPassedPath;
	Vector2 mNextPath;
	Vector2 mTargetPosition;

	bool mbIsObstacle = false;
	bool mbIsOccupied = false;
public:
	PathFindComponent(eUnitTileSize size);
	virtual ~PathFindComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;
	virtual eComponentTag GetTag() override { return eComponentTag::PathFind; }

	void SetStop();
	void SetMove();
	void SetPause();

	void FindPath(Vector2 targetPos);
private:
	void KeepFinding();
};

