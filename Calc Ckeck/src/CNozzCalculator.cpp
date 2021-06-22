#include "pch.h"
#include "CNozzCalculator.h"

CNozzCalculator::CNozzCalculator(int mode) 
	: m_iCalcMode(0) 
{
	memset(m_baResultRr, TRUE, sizeof(m_baResultRr) / sizeof(m_baResultRr[0]));
	memset(m_daResultRr, 0, sizeof(m_daResultRr) / sizeof(m_daResultRr[0]));
}

void CNozzCalculator::SetCalcMode(int iNum) {
	m_iCalcMode = iNum;
}

int CNozzCalculator::GetCalcMode() {
	return m_iCalcMode;
}

void CNozzCalculator::SetResultRr(BOOL bRr, double dID, int iIndex) {
	m_baResultRr[iIndex] = bRr;
	if (bRr) m_daResultRr[iIndex] = 0.9045 * dID;
	else m_daResultRr[iIndex] = 0.1727 * dID;
}

BOOL CNozzCalculator::GetbResultRr(int iIndex) {
	return m_baResultRr[iIndex];
}

double CNozzCalculator::GetdResultRr(int iIndex) {
	return m_daResultRr[iIndex];
}

BOOL CNozzCalculator::Calculate(const MrDArray& InputArr, const MrpDArray& OutputArr) {
	ASSERT(InputArr[1] != NULL);
	ASSERT(InputArr.arrlen() == 5);
	ASSERT(OutputArr.arrlen() == 4);

	switch (m_iCalcMode) {
	case 0:
		CalcShell(InputArr[0], InputArr[1], InputArr[2], InputArr[3], InputArr[4], OutputArr);
		break;
	case 1:
		CalcAppOval(InputArr[0], InputArr[1], InputArr[2], InputArr[3], InputArr[4], OutputArr);
		break;
	case 2:
		CalcRghtOval(InputArr[0], InputArr[1], InputArr[2], InputArr[3], InputArr[4], OutputArr);
		break;
	}

	return TRUE;
}

void CNozzCalculator::CalcShell(double dD, double dTk, double dNS, double dNP, double dVT, const MrpDArray& ResultArr) {
	double dRad = dD / 2;

	if (!dVT) {
		*ResultArr[0] = dNP - dRad - dTk;
		*ResultArr[1] = dNP - pitaGY(dRad, dVT + dNS / 2);
		*ResultArr[2] = dNP - pitaGY(dRad + dTk, dVT + dNS / 2);
		*ResultArr[3] = *ResultArr[2];
	}
	else {
		*ResultArr[0] = dNP - pitaGY(dRad + dTk, dVT);
		*ResultArr[1] = dNP - pitaGY(dRad, dVT + dNS / 2);
		*ResultArr[2] = dNP - pitaGY(dRad + dTk, abs(dVT - dNS / 2));
		*ResultArr[3] = dNP - pitaGY(dRad + dTk, dVT + dNS / 2);
	}
}

void CNozzCalculator::CalcAppOval(double dD, double dTk, double dNS, double dNP, double dVT, const MrpDArray& ResultArr) {
	double dRad = dD / 4;
	double R = 0.9045 * dD;
	double r = 0.1727 * dD;
	double d_ex = R - dRad;
	double vt_ex = dVT - (dD / 2 - r);

	BOOL bFlags[2] = { TRUE, TRUE };

	if (!dVT) {
		*ResultArr[0] = dNP - dRad - dTk;
		*ResultArr[1] = dNP - pitaGY(dRad, dVT + dNS / 2);
		*ResultArr[2] = dNP - pitaGY(dRad + dTk, dVT + dNS / 2);
		*ResultArr[3] = *ResultArr[2];

		SetResultRr(bFlags[0], dD, 0);
		SetResultRr(bFlags[1], dD, 1);
	}
	else {

		*ResultArr[0] = (bFlags[0] = DistingishRr(dD, dTk, 0, dVT)) ? (dNP + d_ex) - pitaGY(R + dTk, dVT) : (dNP)-pitaGY(r + dTk, vt_ex);
		*ResultArr[1] = (bFlags[1] = DistingishRr(dD, 0, dNS, dVT)) ? (dNP + d_ex) - pitaGY(R, dVT + dNS / 2) : (dNP)-pitaGY(r, vt_ex + dNS / 2);
		*ResultArr[2] = DistingishRr(dD, dTk, -dNS, dVT) ? (dNP + d_ex) - pitaGY(R + dTk, abs(dVT - dNS / 2)) : (dNP)-pitaGY(r + dTk, abs(vt_ex - dNS / 2));
		*ResultArr[3] = DistingishRr(dD, dTk, dNS, dVT) ? (dNP + d_ex) - pitaGY(R + dTk, dVT + dNS / 2) : (dNP)-pitaGY(r + dTk, vt_ex + dNS / 2);

		SetResultRr(bFlags[0], dD, 0);
		SetResultRr(bFlags[1], dD, 1);
	}
}

// R 계산 : (NP+d_ex) - pitaGY(R+htk,vt+ns/2)
// r 계산 : (NP) - pitaGY(r+htk, vt+ns/2)

BOOL CNozzCalculator::DistingishRr(double dD, double dTk, double dNS, double dVT) {

	return (dVT + dNS / 2) / (dD * 0.9045 + dTk) < sin(0.0174533 * 26.5675);
}

void CNozzCalculator::CalcRghtOval(double dD, double dTk, double dNS, double dNP, double dVT, const MrpDArray& ResultArr) {
	double tt = (dD / 4 + dTk) / (dD / 2 + dTk);
	double dRad = dD / 2;

	if (!dVT) {
		*ResultArr[0] = dNP - (dRad + dTk) * tt;
		*ResultArr[1] = dNP - pitaGY(dRad, dVT + dNS / 2) * 0.5;
		*ResultArr[2] = dNP - pitaGY(dRad + dTk, dVT + dNS / 2) * tt;
		*ResultArr[3] = *ResultArr[2];
	}
	else {
		*ResultArr[0] = dNP - pitaGY(dRad + dTk, dVT) * tt;
		*ResultArr[1] = dNP - pitaGY(dRad, dVT + dNS / 2) * 0.5;
		*ResultArr[2] = dNP - pitaGY(dRad + dTk, abs(dVT - dNS / 2)) * tt;
		*ResultArr[3] = dNP - pitaGY(dRad + dTk, dVT + dNS / 2) * tt;
	}
}

