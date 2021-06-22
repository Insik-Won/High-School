#include "pch.h"
#include "CCalcDialog.h"

CCalcDialog::CCalcDialog(UINT nIDTemplate, CWnd* pParent)
	: CBaseCalcDialog(nIDTemplate, pParent)
{
}

BOOL CCalcDialog::Create(UINT nIDTemplate, CWnd* pParent, int Digit) {
	return CBaseCalcDialog::Create(nIDTemplate, pParent, Digit);
}