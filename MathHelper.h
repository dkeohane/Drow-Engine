#ifndef MATHHELPER_H_
#define MATHHELPER_H_

#include <math.h>
#include <algorithm>
class MathHelper
{
public:
	static inline float clamp(float x, float a, float b)
	{
		return x < a ? a : (x > b ? b : x);
	}

	static inline float max(float a, float b)
	{
		return (a < b) ? b : a;
	}

	static inline float min(float a, float b)
	{
		return (a < b) ? a : b;
	}
};
#endif