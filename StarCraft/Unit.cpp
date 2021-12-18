#include "stdafx.h"
#include "Unit.h"
#include "UnitModel.h"
#include "MultipleAnimation.h"

Unit::Unit(eTeamTag teamTag)
	: GameObject::GameObject(), mTeamTag{ teamTag }
{
	Init();
}

Unit::~Unit()
{
}

void Unit::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(eBitmapTag::SELECTED_CIRCLE_SMALL, SpriteData::SINGLE_FRAME_X, SpriteData::SINGLE_FRAME_Y)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpPathFind = static_cast<PathFindComponent*>(AddComponent(new PathFindComponent(eUnitTileSize::Small)));
	AddComponent(new ColliderComponent(Vector2(0, -20), Vector2(32, 32), mTeamTag));
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	ChangeCircleColor(EFFECT_COLOR_GREEN);
	AddChild(new UnitModel(), 0);
}

void Unit::Release()
{
}

void Unit::Update()
{
	if (mbIsMove)
	{
		Vector2 dir = Vector2(mTargetPos - GetPosition()).Normalize();
		Vector2 moved = dir * (Fix)100 * DELTA_TIME;

		cout << "Moved X : " << moved.x << endl;
		cout << "Moved Y : " << moved.y << endl;

		mpTransform->SetPosition(GetPosition() + dir * (Fix)100 * DELTA_TIME);
		if (Vector2::GetDistance(GetPosition(), mTargetPos) < (Fix)3)
		{
			mpTransform->SetPosition(mTargetPos);
			cout << "My X : " << GetPosition().x << ", My Y : " << GetPosition().y << endl;
			cout << "Target X : " << mTargetPos.x << ", Target Y : " << mTargetPos.y << endl;
			mbIsMove = false;
		}
	}

	if (INPUT->IsOnceKeyDown('Q'))
	{
		mpSprite->ChangeBitmap(eBitmapTag::NONE);
	}
}

void Unit::FindPath(POINT pos)
{
	mpPathFind->FindPath(Vector2(pos.x, pos.y));
}

void Unit::SetTargetPosition(POINT pos)
{
	mTargetPos.x = pos.x;
	mTargetPos.y = pos.y;
	mbIsMove = true;
}