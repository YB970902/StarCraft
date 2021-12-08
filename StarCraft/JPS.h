#pragma once

class TileCoord
{
private:
	int mX = -1;
	int mY = -1;
public:
	TileCoord() = default;
	TileCoord(int x, int y) : mX{ x }, mY{ y } { }
	virtual ~TileCoord() = default;
	void Add(const TileCoord& rhs) { mX += rhs.mX; mY += rhs.mY; }
	void Clear() { mX = -1; mY = -1; }
	void Blocked() { mX = -2; mY = -2; }
	bool IsEmpty() { return (mX == -1 && mY == -1) || IsBlocked(); }
	bool IsBlocked() { return (mX == -2 && mY == -2); }

	const bool operator==(const TileCoord& other) const { return ((mX == other.mX) && (mY == other.mY)); }
	const bool operator!=(const TileCoord& other) const { return ((mX != other.mX) || (mY != other.mY)); }

	Fix DistanceSqrt(const TileCoord& rhs)
	{
		int absX = abs(mX - rhs.mX);
		int absY = abs(mY - rhs.mY);

		int diagDist = min(absX, absY);
		int straightDist = max(absX, absY) - diagDist;

		return ((Fix)diagDist * (Fix)1.141f) + (Fix)straightDist;
	}

	inline int GetX() const { return mX; }
	inline int GetY() const { return mY; }
	inline void SetX(int x) { mX = x; }
	inline void SetY(int y) { mY = y; }

	static TileCoord NextCoordinate(const TileCoord& coord, const int dir)
	{
		static int dtMove[] = { 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0, -1, -1, 0, 0 };
		return TileCoord(coord.mX + dtMove[dir * 2], coord.mY + dtMove[dir * 2 + 1]);
	}
};

class TileNode
{
public:
	typedef shared_ptr<TileNode> SharedPtr;
	typedef weak_ptr<TileNode> WeakPtr;

private:
	Fix mScore = 0;
	Fix mHeuri = 0;
	Fix mTotal = 0;
	TileCoord mPos;
	SharedPtr mpParent = nullptr;
	char mDir = 0;
public:
	void Set(SharedPtr pParent, const TileCoord& pos, const TileCoord& end, const char dir)
	{
		mpParent = pParent;
		mPos = pos;
		mDir = dir;
		if (mpParent)
		{
			mScore = mPos.DistanceSqrt(mpParent->mPos) + mpParent->GetScore();
		}
		else
		{
			mScore = 0;
		}
		mHeuri = mPos.DistanceSqrt(end);
		mTotal = mScore + mHeuri;
	}
	inline const Fix GetScore() const { return mScore; }
	inline const Fix GetHeuristic() const { return mHeuri; }
	inline const Fix GetTotal() const { return mTotal; }

	inline const TileCoord& GetPosition() const { return mPos; }
	inline const int GetPositionX() const { return mPos.GetX(); }
	inline const int GetPositionY() const { return mPos.GetY(); }

	inline const int GetDirection() const { return mDir; }
	inline SharedPtr GetParent() const { return mpParent; }
};

class JumpPointHeap
{
private:
	vector<TileNode::SharedPtr> mHeap;
public:
	void Push(TileNode::SharedPtr pNode)
	{
		mHeap.push_back(pNode);
		ShiftUp(mHeap.size() - 1);
	}

	bool PushSmaller(TileNode::SharedPtr pNode)
	{
		auto pSeg = mHeap.data();
		int size = mHeap.size();
		for (size_t i = 0; i < size; ++i, ++pSeg)
		{
			if (pNode->GetPosition() != (*pSeg)->GetPosition()) { continue; }

			if (pNode->GetTotal() >= (*pSeg)->GetTotal()) { return false; }
			else
			{
				(*pSeg) = pNode;
				ShiftUp(i);
				return true;
			}
		}

		return true;
	}

	TileNode::SharedPtr Pop()
	{
		TileNode::SharedPtr pResult = mHeap.front();
		mHeap.front() = mHeap.back();
		mHeap.pop_back();
		ShiftDown(0);
		return pResult;
	}

	bool IsEmpty() { return mHeap.empty(); }

	void Clear() { mHeap.clear(); }

private:
	void ShiftDown(size_t parent)
	{
		auto pSeg = mHeap.data();
		size_t size = mHeap.size();
		while (true)
		{
			size_t left = parent * 2 + 1;
			if (left >= size) { break; }

			size_t right = left + 1;

			if (right < size && pSeg[right]->GetTotal() < pSeg[left]->GetTotal()) { left = right; }

			if (pSeg[parent]->GetTotal() < pSeg[left]->GetTotal()) { break; }

			swap(pSeg[parent], pSeg[left]);
			parent = left;
		}
	}

	void ShiftUp(size_t node)
	{
		auto pSeg = mHeap.data();
		while (node > 0)
		{
			size_t parent = (node - 1) / 2;

			if (pSeg[node]->GetTotal() > pSeg[parent]->GetTotal()) { return; }

			swap(pSeg[node], pSeg[parent]);
			node = parent;
		}
	}
};

struct PathFinderData
{
	DetailMap* pTileMap = nullptr;
	BitArray* pSearched = nullptr;
	JumpPointHeap JumpPoint;
	TileCoord EndPos;
};

class JPS
{
};