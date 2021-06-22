#include "pch.h"
#include "CClipCalculator.h"

CClipCalculator::CClipCalculator() {

}

BOOL CClipCalculator::Calculate(const MrDArray& InputArr, const MrpDArray& OutputArr) {
	ASSERT(InputArr.arrlen() == 6);
	ASSERT(OutputArr.arrlen() == 3);

	return CalcClip(InputArr[0], InputArr[1], InputArr[2], InputArr[3], InputArr[4], InputArr[5], OutputArr);

}


BOOL CClipCalculator::CalcClip(double dR, double dW, double dPt, double dt, double dCP, double dB, const MrpDArray& resultArr) {
	*resultArr[0] = abs(dCP - pitaGY(dR +dPt, dW)) + dB;
	*resultArr[1] = abs(dCP - pitaGY(dR + dPt, dW + dt)) + dB;
	*resultArr[2] = abs(dCP - pitaGY(dR + dPt, dW - dt)) + dB;
	return (dCP-pitaGY(dR+dPt, dW)) > 0;
}

double CClipCalculator::CalcClipLength(double dR, double dW, double dPt, double dCP) {
	return abs(dCP - pitaGY(dR, dW));
}