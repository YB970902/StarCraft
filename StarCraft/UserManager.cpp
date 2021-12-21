#include "stdafx.h"
#include "UserManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "RenderManager.h"
#include "UnitManager.h"
#include "SoundManager.h"
#include "LineGizmo.h"
#include "Unit.h"

void UserManager::Init(eTeamTag teamTag)
{
	mTeamTag = teamTag;
	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		mEnemyTeamTag = eTeamTag::BLUE_TEAM;
		break;
	case eTeamTag::BLUE_TEAM:
		mEnemyTeamTag = eTeamTag::RED_TEAM;
		break;
	}
	mVecSelectedUnit.reserve(MAX_SELECT_UNIT);
	for (int i = 0; i < LINE_SIZE; ++i)
	{
		mpArrLineGizmo[i] = static_cast<LineGizmo*>(RENDER->RenderLine(Vector2::Left(), Vector2::Left(), 3, D2D1::ColorF::LimeGreen));
	}
}

void UserManager::Release()
{
}

void UserManager::Update()
{
	UnitID ID = 0;
	if (mbIsDrag)
	{
		UpdateDragLine();
		if (INPUT->IsOnceKeyUp(VK_LBUTTON))
		{
			vector<UnitID> vecUnit;
			if (PHYSICS->GetDragUnit(mTeamTag, mDragStartPos, INPUT->GetMousePosition(), vecUnit, MAX_SELECT_UNIT))
			{
				SOUND->Play(eSoundTag::MarineClick);
				ChangeSelectDragUnit(vecUnit);
			}
			else if (PHYSICS->GetDragUnit(mEnemyTeamTag, mDragStartPos, INPUT->GetMousePosition(), vecUnit, 1))
			{
				ChangeSelectUnit(vecUnit[0]);
			}
			SetIsDrag(false);
		}
	}
	else
	{
		if (INPUT->IsOnceKeyDown('A'))
		{
			SetAttackMode(true);
		}

		if (mbIsAttackMode && INPUT->IsOnceKeyDown(VK_ESCAPE))
		{
			SetAttackMode(false);
		}
		else if (mbIsAttackMode && INPUT->IsOnceKeyDown(VK_RBUTTON))
		{
			SetAttackMode(false);
			UnitMove(INPUT->GetMousePosition());
		}

		if (PHYSICS->GetUnit(mTeamTag, INPUT->GetMousePosition(), &ID))
		{
			if (mbIsAttackMode)
			{
				UI->ChangeCursorState(eCursorState::PrepairGreen);
				if (INPUT->IsOnceKeyUp(VK_LBUTTON))
				{
					ChaseTarget(ID);
					SetAttackMode(false);
				}
			}
			else
			{
				UI->ChangeCursorState(eCursorState::OnGreen);
				if (INPUT->IsOnceKeyUp(VK_LBUTTON))
				{
					SOUND->Play(eSoundTag::MarineClick);
					ChangeSelectUnit(ID);
				}
			}
		}
		else if (PHYSICS->GetUnit(mEnemyTeamTag, INPUT->GetMousePosition(), &ID))
		{
			if (mbIsAttackMode)
			{
				UI->ChangeCursorState(eCursorState::PrepairRed);
			}
			else
			{
				UI->ChangeCursorState(eCursorState::OnRed);
				if (INPUT->IsOnceKeyUp(VK_LBUTTON))
				{
					ChangeSelectUnit(ID);
				}
			}
		}
		else
		{
			if (mbIsAttackMode)
			{
				if (INPUT->IsOnceKeyDown(VK_LBUTTON))
				{
					AttackGround(INPUT->GetMousePosition());
					SetAttackMode(false);
				}
				else
				{
					UI->ChangeCursorState(eCursorState::PrepairYellow);
				}
			}
			else
			{
				if (INPUT->IsOnceKeyDown(VK_LBUTTON))
				{
					SetIsDrag(true);
				}
				else
				{
					UI->ChangeCursorState(eCursorState::Idle);
				}
			}
		}

		if (INPUT->IsOnceKeyDown(VK_RBUTTON))
		{
			UnitMove(INPUT->GetMousePosition());
		}
		else if (INPUT->IsOnceKeyDown('S'))
		{
			if (!mVecSelectedUnit.empty())
			{
				for (int i = 0; i < mVecSelectedUnit.size(); ++i)
				{
					UNIT->CommandStopUnit(mVecSelectedUnit[i]);
				}
			}
		}
	}
}

void UserManager::OnUnitRemoved(UnitID ID)
{
	auto it = find(mVecSelectedUnit.begin(), mVecSelectedUnit.end(), ID);

	if (it != mVecSelectedUnit.end())
	{
		mVecSelectedUnit.erase(it);
	}
}

void UserManager::UpdateDragLine()
{
	if (mbIsDrag == false) { return; }

	POINT dragEndPos = INPUT->GetMousePosition();

	mpArrLineGizmo[0]->SetStartPosition(Vector2(mDragStartPos.x, mDragStartPos.y));
	mpArrLineGizmo[1]->SetStartPosition(Vector2(mDragStartPos.x, mDragStartPos.y));
	mpArrLineGizmo[0]->SetEndPosition(Vector2(mDragStartPos.x, dragEndPos.y));
	mpArrLineGizmo[1]->SetEndPosition(Vector2(dragEndPos.x, mDragStartPos.y));

	mpArrLineGizmo[2]->SetEndPosition(Vector2(dragEndPos.x, dragEndPos.y));
	mpArrLineGizmo[3]->SetEndPosition(Vector2(dragEndPos.x, dragEndPos.y));
	mpArrLineGizmo[2]->SetStartPosition(Vector2(mDragStartPos.x, dragEndPos.y));
	mpArrLineGizmo[3]->SetStartPosition(Vector2(dragEndPos.x, mDragStartPos.y));
}

void UserManager::SetIsDrag(bool set)
{
	mbIsDrag = set;
	if (mbIsDrag)
	{
		UI->ChangeCursorState(eCursorState::Drag);
		mDragStartPos = INPUT->GetMousePosition();
	}
	else
	{
		UI->ChangeCursorState(eCursorState::Idle);
		for (int i = 0; i < LINE_SIZE; ++i)
		{
			mpArrLineGizmo[i]->SetStartPosition(Vector2::Left());
			mpArrLineGizmo[i]->SetEndPosition(Vector2::Left());
		}
	}
}

void UserManager::ChangeSelectUnit(UnitID ID)
{
	for (int i = 0; i < mVecSelectedUnit.size(); ++i)
	{
		UNIT->SetSelectUnit(mVecSelectedUnit[i], false);
	}
	mVecSelectedUnit.clear();

	UNIT->SetSelectUnit(ID, true);
	mVecSelectedUnit.push_back(ID);
}

void UserManager::ChangeSelectDragUnit(const vector<UnitID>& vecUnit)
{
	for (int i = 0; i < mVecSelectedUnit.size(); ++i)
	{
		UNIT->SetSelectUnit(mVecSelectedUnit[i], false);
	}
	mVecSelectedUnit.clear();

	for (int i = 0; i < vecUnit.size(); ++i)
	{
		UNIT->SetSelectUnit(vecUnit[i], true);
		mVecSelectedUnit.push_back(vecUnit[i]);
	}
}

void UserManager::UnitMove(const POINT& pos)
{
	if (mVecSelectedUnit.empty()) { return; }

	SOUND->Play(eSoundTag::MarineMove);

	if (mVecSelectedUnit.size() == 1)
	{
		UNIT->CommandMoveUnit(mVecSelectedUnit[0], INPUT->GetMousePosition());
	}
	else
	{
		POINT leftTop;
		POINT rightBottom;
		vector<POINT> vecTargetPos;
		vecTargetPos.resize(mVecSelectedUnit.size());
		UNIT->GetUnitPosition(mVecSelectedUnit[0], vecTargetPos[0]);
		leftTop = vecTargetPos[0];
		rightBottom = vecTargetPos[0];

		for (int i = 1; i < mVecSelectedUnit.size(); ++i)
		{
			UNIT->GetUnitPosition(mVecSelectedUnit[i], vecTargetPos[i]);
			leftTop.x = min(leftTop.x, vecTargetPos[i].x);
			leftTop.y = min(leftTop.y, vecTargetPos[i].y);
			rightBottom.x = max(rightBottom.x, vecTargetPos[i].x);
			rightBottom.y = max(rightBottom.y, vecTargetPos[i].y);
		}

		POINT center = { leftTop.x + (rightBottom.x - leftTop.x) / 2, leftTop.y + (rightBottom.y - leftTop.y) / 2 };

		for (int i = 0; i < mVecSelectedUnit.size(); ++i)
		{
			UNIT->CommandMoveUnit(mVecSelectedUnit[i], POINT{
				INPUT->GetMousePosition().x + (vecTargetPos[i].x - center.x),
				INPUT->GetMousePosition().y + (vecTargetPos[i].y - center.y)
				}
			);
		}
	}
}

void UserManager::ChaseTarget(UnitID ID)
{
	for (int i = 0; i < mVecSelectedUnit.size(); ++i)
	{
		UNIT->CommandAttackUnit(mVecSelectedUnit[i], ID);
	}
}

void UserManager::AttackGround(const POINT& pos)
{
	for (int i = 0; i < mVecSelectedUnit.size(); ++i)
	{
		UNIT->CommandAttackGround(mVecSelectedUnit[i], pos);
	}
}

void UserManager::SetAttackMode(bool set)
{
	if (set)
	{
		mbIsAttackMode = true;
		UI->ChangeCursorState(eCursorState::PrepairYellow);
	}
	else
	{
		mbIsAttackMode = false;
		UI->ChangeCursorState(eCursorState::Idle);
	}
}
