#pragma once
#include "CBaseCalcDialog.h"

class CCalcDialog :
	public CBaseCalcDialog
{
public:
	CCalcDialog(UINT nIDTemplate, CWnd* pParent);
	BOOL Create(UINT nIDTemplate, CWnd* pParent, int Digit = 1);

public:
	virtual BOOL MakeFile(CString FileDir, const CString& FileName) = 0;
	virtual BOOL LoadFile(const CString& FileDir, const CString& FileName) = 0;
	virtual void UpdateDigit() = 0;
	virtual void DlgCalculate() = 0;
};

