#pragma once
#include "CBaseCalculator.h"

class CNozzCalculator :
	public CBaseCalculator
{
public:
	CNozzCalculator(int mode = 0);

private:
	int m_iCalcMode;
	BOOL m_baResultRr[2];
	double m_daResultRr[2];

public:
	int GetCalcMode();
	void SetCalcMode(int iNum);
	void SetResultRr(BOOL bRr, double dNum, int iIndex);
	BOOL GetbResultRr(int iIndex);
	double GetdResultRr(int iIndex);
	virtual BOOL Calculate(const MrDArray& InputArr, const MrpDArray& ResultArr);

private:
	void CalcShell(double dD, double dTk, double dNS, double dNP, double dVT, const MrpDArray& ResultArr);
	void CalcAppOval(double dD, double dTk, double dNS, double dNP, double dVT, const MrpDArray& ResultArr);
	void CalcRghtOval(double dD, double dTk, double dNS, double dNP, double dVT, const MrpDArray& ResultArr);
	static BOOL DistingishRr(double dD, double dTk, double dNS, double dVT);
};

