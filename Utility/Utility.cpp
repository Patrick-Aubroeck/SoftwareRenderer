#include "Utility.h"
#include <math.h>

const void Utility::Swap(float value1, float value2)
{
	float temp = value1;
	value1 = value2;
	value2 = temp;
}

const int Utility::IPart(float x)
{
	return int(x);
}

const int Utility::Round(float x)
{
	return IPart(x + 0.5);
}

const float Utility::FPart(float x)
{
	return x < 0 ? 1 - (x - floor(x)) : x - floor(x);
}

const float Utility::RfPart(float x)
{
	return 1 - FPart(x);
}
