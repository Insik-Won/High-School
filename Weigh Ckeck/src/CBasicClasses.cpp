#include "pch.h"
#include "CBasicClasses.h"
//---------------------------CBasicSegment------------------------//

CBasicCalcDlg::CBasicCalcDlg(UINT nIDTemplate, CWnd* pParent, int iDigit, double dSpGravity) 
	:CDialogEx(nIDTemplate, pParent)
	,m_iDigit(iDigit)
	,m_dSpGravity(dSpGravity)
{
	m_pEditBrush = new CBrush(RGB(255, 255, 255));
}

CBasicCalcDlg::~CBasicCalcDlg() {
	delete m_pEditBrush;
}

BOOL CBasicCalcDlg::Create(UINT nIDTemplate, CWnd* pParent, int iDigit, double dSpGravity) {
	m_iDigit = iDigit;
	m_dSpGravity = dSpGravity;
	return CDialogEx::Create(nIDTemplate, pParent);
}

void CBasicCalcDlg::SetDigit(int iDigit) {
	m_iDigit = iDigit;
}
int CBasicCalcDlg::GetDigit() {
	return m_iDigit;
}

void CBasicCalcDlg::SetSpGravity(double dSpGravity) {
	m_dSpGravity = dSpGravity;
}
double CBasicCalcDlg::GetSpGravity() {
	return m_dSpGravity;
}

CBrush* CBasicCalcDlg::GetEditBrush() {
	return m_pEditBrush;
}

void CBasicCalcDlg::SetBrushColor(COLORREF color) {
	m_pEditBrush->CreateSolidBrush(color);
}

//---------------------------CBasicSegment------------------------//

CBasicSegment::CBasicSegment(UINT nIDTemplate, CBasicSgmMngDlg* pParent, UINT arrSize)
	: CDialogEx(nIDTemplate, pParent)
	, m_dResult(0)
	, m_BitmapID(NULL)
	, m_szaInput(arrSize)
{
	m_pParent = pParent;
	m_pvfCalculate = NULL;
}

BOOL CBasicSegment::Create(UINT nIDTemplate, CBasicSgmMngDlg* pParent)
{
	m_pParent = pParent;
	return CDialogEx::Create(nIDTemplate, pParent);
}

void CBasicSegment::SetInputArrlen(UINT len) {
	m_szaInput.ClearAndSetLen(len);
	for (int i = 0; i < m_szaInput.arrlen(); i++) {
		m_szaInput[i] = _T("");
	}
}

void CBasicSegment::SetCalcFunc(void(*pvfCalcFunc)(const MrDArray&, double*)) {
	m_pvfCalculate = pvfCalcFunc;
}
pvFunc CBasicSegment::GetCalcFunc() {
	return m_pvfCalculate;
}

void CBasicSegment::SetBitmapID(int bID) {
	m_BitmapID = bID;
}
int CBasicSegment::GetBitmapID() {
	return m_BitmapID;
}

int CBasicSegment::GetDigit() {
	return m_pParent->m_iDigit;
}
double CBasicSegment::GetSpGravity() {
	return m_pParent->m_dSpGravity;
}

CBasicSgmMngDlg* CBasicSegment::GetParent() {
	return m_pParent;
}

//-------------------------CBasicSgmMngDlg--------------------------//

CBasicSgmMngDlg::CBasicSgmMngDlg(UINT nIDTemplate, CWnd* pParent)
	: CBasicCalcDlg(nIDTemplate, pParent)
	, m_pcbaSegments(1)
{
}

BOOL CBasicSgmMngDlg::Create(UINT nIDTemplate, CWnd* pParent, int iDigit, double dSpGravity) {
	return CBasicCalcDlg::Create(nIDTemplate, pParent, iDigit, dSpGravity);
}