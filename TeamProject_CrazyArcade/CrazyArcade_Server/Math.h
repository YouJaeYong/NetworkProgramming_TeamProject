#pragma once
typedef struct tagVector2
{
	float		x;
	float		y;

	tagVector2() { x = 0.f, y = 0.f; }
	tagVector2(float fX, float fY) { x = fX, y = fY; }

	tagVector2 operator * (float fNum)
	{
		tagVector2 vec;
		vec.x = x * fNum;
		vec.y = y * fNum;
		return vec;
	}
	tagVector2 operator + (float fNum)
	{
		tagVector2 vec;
		vec.x = x + fNum;
		vec.y = y + fNum;
		return vec;
	}
	tagVector2 operator + (tagVector2 _vec)
	{
		tagVector2 vec;
		vec.x = x + _vec.x;
		vec.y = y + _vec.y;
		return vec;
	}
	tagVector2 operator - (tagVector2 _vec)
	{
		tagVector2 vec;
		vec.x = x - _vec.x;
		vec.y = y - _vec.y;
		return vec;
	}
	tagVector2 operator - (float fNum)
	{
		tagVector2 vec;
		vec.x = x - fNum;
		vec.y = y - fNum;
		return vec;
	}
	tagVector2 operator * (tagVector2 _vec)
	{
		tagVector2 vec;
		vec.x = x * _vec.x;
		vec.y = y * _vec.y;
		return vec;
	}

	tagVector2 operator += (tagVector2 vec2)
	{
		x += vec2.x;
		y += vec2.y;
		return *this;
	}
	tagVector2 operator -= (tagVector2 vec2)
	{
		x -= vec2.x;
		y -= vec2.y;
		return *this;
	}

	tagVector2 operator *= (tagVector2 vec2)
	{
		x *= vec2.x;
		y *= vec2.y;
		return *this;
	}

	bool operator == (tagVector2 vec2)
	{
		if (x == vec2.x && y == vec2.y)return true;
		return false;
	}

	float Length(void)
	{
		float fLen = sqrtf(x * x + y * y);
		return fLen;
	}
	tagVector2 Nomalize(void)
	{
		x = x / Length();
		y = y / Length();
		return *this;
	}
	float Dot(tagVector2 vec2)
	{
		float fx = x * vec2.x;
		float fy = y * vec2.y;
		return fx + fy;
	}
}VECTOR2;