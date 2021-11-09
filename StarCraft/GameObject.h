#pragma once
class GameObject
{
protected:
	const float MOVE_SPEED = 100;

	POINTFLOAT mPosition = {};

	bool mbIsMoving = false;
	POINTFLOAT mTargetPosition = {};
	float mAngle = 0.0f;
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void MoveTo(const POINTFLOAT& target);

	inline const POINTFLOAT& GetPosition() { return mPosition; }
	inline void SetPosition(const POINTFLOAT& set) { mPosition.x = set.x; mPosition.y = set.y; }
};