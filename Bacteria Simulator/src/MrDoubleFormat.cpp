#include "pch.h"
#include "MrDoubleFormat.h"

namespace MrDoubleFormat {
	double  RoundOff(const double dNum, const double dTo) {
		if (dTo < 0) return dNum;
		return round(dNum * pow(10, dTo)) / pow(10, dTo);
	}

	BOOL CheckAndAtof(CString str, double* dNum, const BOOL bFlag) {
		str.Trim();
		if (str == "") {
			if (bFlag == TRUE) {
				return FALSE;
			}
			else {
				*dNum = 0;
				return TRUE;
			}
		}
		if (str == "0") {
			*dNum = 0;
			return TRUE;
		}
		if (*dNum = _tstof(str))
			return TRUE;
		else {
			if (bFlag == TRUE) {
				return FALSE;
			}
			 else {
				*dNum = 0;
				return TRUE;
			}
		}
	}

	BOOL CheckAndAtoi(CString str, int* iNum, const BOOL bFlag) {
		str.Trim();
		if (str == "") {
			if (bFlag == TRUE) {
				return FALSE;
			}
			else {
				*iNum = 0;
				return TRUE;
			}
		}
		if (str == "0") {
			*iNum = 0;
			return TRUE;
		}
		if (*iNum = _tstoi(str))
			return TRUE;
		else {
			if (bFlag == TRUE) {
				return FALSE;
			}
			else {
				*iNum = 0;
				return TRUE;
			}
		}
	}

	void FormatDouble(CString* str, const double dNum, const double dTo) {
		str->Format(_T("%f"), RoundOff(dNum, dTo));
		str->TrimRight('0');
		str->TrimRight('.');
	}

	double pitaGY(double dZ, double dX) {
		return sqrt(pow(dZ, 2) - pow(dX, 2));
	}
}