#pragma once
#include <afxdialogex.h>
#include "MrDoubleFormat.h"

class CBasicCalcDlg;
class CBasicSegment;
class CBasicSgmMngDlg;

//---------------------------CBasicCalcDlg------------------------//

class CBasicCalcDlg :
	public CDialogEx
{
public:
	CBasicCalcDlg(UINT nIDTemplate, CWnd* pParent, int iDigit = 1, double dSpGravity = 7.85);
	BOOL Create(UINT nIDTemplate, CWnd* pParent, int iDigit, double dSpGravity);
	~CBasicCalcDlg();

protected:
	int m_iDigit;
	double m_dSpGravity;
protected:
	CBrush* m_pEditBrush;

public:
	void SetDigit(int iDigit);
	int GetDigit();
	void SetSpGravity(double dSpGravity);
	double GetSpGravity();

public:
	CBrush* GetEditBrush();
	void SetBrushColor(COLORREF color);

};

//---------------------------CBasicSegment------------------------//
typedef void(*pvFunc)(const MrDArray&, double*);

class CBasicSegment :
	public CDialogEx
{
public:
	CBasicSegment(UINT nIDTemplate, CBasicSgmMngDlg* pParent, UINT arrSize = 1);
	BOOL Create(UINT nIDTemplate, CBasicSgmMngDlg* pParent);

protected:
	CBasicSgmMngDlg* m_pParent;
	pvFunc m_pvfCalculate;
	int m_BitmapID;
protected:
	MrszArray m_szaInput;
	double m_dResult;

public:
	void SetCalcFunc(void (*Calculate)(const MrDArray&, double*));
	pvFunc GetCalcFunc();
	void SetBitmapID(int bID);
	int GetBitmapID();
	CBasicSgmMngDlg* GetParent();

	int GetDigit();
	double GetSpGravity();

	void SetInputArrlen(UINT len);

private:
	virtual LRESULT afx_msg OnCalculate(WPARAM wParam = NULL, LPARAM lParam = NULL) = 0;
};

//-------------------------CBasicSgmMngDlg--------------------------//

class CBasicSgmMngDlg :
	public CBasicCalcDlg
{
public:
	CBasicSgmMngDlg(UINT nIDTemplate, CWnd* pParent);
	BOOL Create(UINT nIDTemplate, CWnd* pParent, int iDIgit = 1, double dSpGravity = 7.85);

public:
	MrArray<CBasicSegment*> m_pcbaSegments;
	friend class CBasicSegment;
};
