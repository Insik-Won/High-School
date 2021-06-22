#pragma once
#include <afxdialogex.h>

class CBaseCalcDialog :	public CDialogEx
{
public:
	CBaseCalcDialog(UINT nIDTemplate, CWnd* pParent);
	BOOL Create(UINT nIDTemplate, CWnd* pParent, int Digit = 0);
	virtual ~CBaseCalcDialog();

protected:
	int m_iDigit;
	CBrush* m_pEditBrush;
public:
	int GetDigit();
	void SetDigit(int iDigit);
	CBrush* GetEditBrush();
	void SetBrushColor(COLORREF color);
};

