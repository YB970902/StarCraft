#pragma once

#define PI (3.14159265359f)
#define DEG2RAD(angle) (angle * 0.01745329251f)
#define RAD2DEG(angle) (angle * 57.29577951307f)

#include "Fixed.h"
#include "fixed_point.h"

typedef fpml::fixed_point<int, 16> Fix;

#include "Vector2.h"