#pragma once
#include <afxdialogex.h>
#include <cmath>

#define PI 3.1415926535897932
#define CA(R) (pow((R),2)*PI)

namespace MrDoubleFormat {
	double  RoundOff(const double dNum, const double dTo);
	BOOL CheckAndAtof(CString str, double* dNum, const BOOL bFlag = FALSE);
	BOOL CheckAndAtoi(CString str, int* dNum, const BOOL bFlag = FALSE);
	void FormatDouble(CString* str, const double dNum, const double dTo);
	double pitaGY(double dZ, double dX);
}


template <typename T>
class MrArray {
private:
	T* arr;
	int len;
public:
	MrArray(int len) : len(len) {
		ASSERT(len >= 0);
		arr = new T[len];
	}

	~MrArray() {
		delete[] arr;
	}

	T operator[] (int idx) const {
		ASSERT(idx >= 0);
		ASSERT(idx < len);
		return arr[idx];
	}

	T& operator[] (int idx) {
		ASSERT(idx >= 0);
		ASSERT(idx < len);
		return arr[idx];
	}

	MrArray<T>& operator=(const MrArray<T>& copy) {
		int length = copy.arrlen();
		
		ClearAndSetLen(length);

		for (int i = 0; i < len; i++) {
			arr[i] = copy[i];
		}
		return *this;
	}

	void ClearAndSetLen(int length) {
		delete[] arr;
		arr = new T[length];
		len = length;
	}

	int arrlen() const {
		return len;
	}
};

typedef MrArray<double> MrDArray;
typedef MrArray<double*> MrpDArray;
typedef MrArray<CString> MrszArray;
typedef MrArray<CString*> MrpszArray;
typedef MrArray<int> MrIArray;
