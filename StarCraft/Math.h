#pragma once

#define PI (3.14159265359f)
#define DEG2RAD(angle) (angle * 0.01745329251f)
#define RAD2DEG(angle) (angle * 57.29577951307f)

#include "Fixed.h"
#include "fixed_point.h"

typedef fpml::fixed_point<int, 16> Fix;

//typedef numeric::fixed<16, 16> fix;

//static fix fix_sqrt(fix n)
//{
//	fix x = 2;
//
//	for (int i = 0; i < 100; i++)
//	{
//		x = (x + (n / x)) / 2;
//	}
//
//	return x;
//}
//
//static fix fix_abs(fix n)
//{
//	if (n < 0)
//		return n * (-1);
//	else
//		return n;
//}

#include "Vector2.h"