#pragma once

struct Vector2
{
	Fix x = 0;
	Fix y = 0;

	Vector2() = default;
	Vector2(Fix _x, Fix _y)
	{
		x = _x;
		y = _y;
	}
	Vector2(const Vector2& other) { x = other.x; y = other.y; }

	Fix Distance() { return sqrt(SqrtDistance()); }
	Fix SqrtDistance() { return (x * x) + (y * y); }
	Vector2 Normalize()
	{
		Fix mag = Distance();
		if (mag == (Fix)0) return Zero();
		return Vector2(this->x / mag, this->y / mag);
	}

	Vector2 operator * (const Fix& num) const { return Vector2(x * num, y * num); }
	Vector2 operator / (const Fix& num) const { return Vector2(x / num, y / num); }
	Vector2& operator *= (const Fix& num) { x *= num; y *= num; return *this; }
	Vector2& operator /= (const Fix& num) { x /= num; y /= num; return *this; }
	
	Vector2 operator + (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	Vector2 operator - (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2 operator * (const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	Vector2 operator / (const Vector2& v) const { return Vector2(x / v.x, y / v.y); }
	Vector2& operator += (const Vector2& v) { x += v.x; y += v.y; return *this; }
	Vector2& operator -= (const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	Vector2& operator *= (const Vector2& v) { x *= v.x; y *= v.y; return *this; }
	Vector2& operator /= (const Vector2& v) { x /= v.x; y /= v.y; return *this; }
	
	bool operator == (const Vector2& v) { return (x == v.x) && (y == v.y); }
	bool operator != (const Vector2& v) { return (x != v.x) || (y != v.y); }
	
	static Vector2 Zero() { return Vector2((Fix)0, (Fix)0); }
	static Vector2 One() { return Vector2((Fix)1, (Fix)1); }
	static Vector2 Left() { return Vector2((Fix)-1, (Fix)0); }
	static Vector2 Right() { return Vector2((Fix)1, (Fix)0); }
	static Vector2 Up() { return Vector2((Fix)0, (Fix)-1); }
	static Vector2 Down() { return Vector2((Fix)0, (Fix)1); }
	
	static Fix GetDistance(const Vector2& a, const Vector2& b) { return Vector2(a.x - b.x, a.y - b.y).Distance(); }
	static Fix GetSqrtDistance(const Vector2& a, const Vector2& b) { return Vector2(a.x - b.x, a.y - b.y).SqrtDistance(); }
};