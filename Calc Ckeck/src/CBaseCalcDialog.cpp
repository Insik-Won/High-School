#include "pch.h"
#include "CBaseCalcDialog.h"

CBaseCalcDialog::CBaseCalcDialog(UINT nIDTemplate, CWnd* pParent) 
	: CDialogEx(nIDTemplate, pParent) 
	, m_iDigit(0)
{	 
	m_pEditBrush = new CBrush(RGB(255, 255, 255));
}

BOOL CBaseCalcDialog::Create(UINT nIDTemplate, CWnd* pParent, int Digit) {	
	m_iDigit = Digit;
	return CDialogEx::Create(nIDTemplate, pParent);
}

CBaseCalcDialog::~CBaseCalcDialog() {
	delete m_pEditBrush;
}

int CBaseCalcDialog::GetDigit() { 
	return m_iDigit; 
}
void CBaseCalcDialog::SetDigit(int iDigit) { 
	m_iDigit = iDigit; 
}

CBrush* CBaseCalcDialog::GetEditBrush() {
	return m_pEditBrush;
}

void CBaseCalcDialog::SetBrushColor(COLORREF color) {
	m_pEditBrush->CreateSolidBrush(color);
}