#pragma once
#include "MrDoubleFormat.h"

using namespace MrDoubleFormat;

class CBaseCalculator
{
public:
	virtual BOOL Calculate(const MrDArray& InputArr, const MrpDArray& ResultArr) = 0;
};
