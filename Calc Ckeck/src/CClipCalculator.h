#pragma once
#include "CBaseCalculator.h"

class CClipCalculator :	public CBaseCalculator
{
public :
	CClipCalculator();

public:
	virtual BOOL Calculate(const MrDArray& InputArr, const MrpDArray& OutputArr);
	double CalcClipLength(double dR, double dW, double dPt, double dCP);
private:
	BOOL CalcClip(double dR, double dW, double dPt, double dt, double dCP, double dB, const MrpDArray& ResultArr);
	
};

