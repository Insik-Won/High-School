#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

#include "CCalcDialog.h"
#include "CNozzCalculator.h"
#include "MrDoubleFormat.h"

// CNozzleDlg 대화 상자

class CNozzleDlg : public CCalcDialog
{
	DECLARE_DYNAMIC(CNozzleDlg)

public:
	CNozzleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNozzleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Nozzle_Length_Ckeck };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	static const double m_dmm[16];
	MrpszArray m_pszaInput;
	MrpDArray m_pdaOutput; 
public:
	int m_iCalcMode;
	CNozzCalculator m_NozzCalculator;
	CString m_strShellID;
	CString m_strShellRad;
	CString m_strShellThk;
	CString m_strHeadThk;
	CString m_strNozSize;
	CComboBox m_ComboPipeSize;
	int m_iTempSel;
	CString m_strNozProj;
	CString m_strDistX;
	CString m_strDistY;
	CString m_strVesToCL;
	BOOL m_bVTisZero;

	double m_dResult1;
	double m_dResult2;
	double m_dResult3;
	double m_dResult4;

	CString m_strL1Rr;
	CString m_strL2Rr;
public:
	virtual BOOL MakeFile(CString FileDir, const CString& FileName);
	virtual BOOL LoadFile(const CString& FileDir, const CString& FileName);
	virtual void UpdateDigit();
	virtual void DlgCalculate();
public:
	afx_msg void OnEnUpdateEditShellId();
	afx_msg void OnEnChangeEditShellRad();
	afx_msg void OnCbnSelchangeComboPipeSize();
	afx_msg void OnEnChangeEditxDstnce();
	afx_msg void OnEnChangeEditYDstnce();
	afx_msg void OnEnChangeEditVesClTo();
	afx_msg void OnBnClickedCheckVesClZero();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditShellThk();
	afx_msg void OnEnChangeEditHeadThk();
	afx_msg void OnEnChangeEditNozSize();
	afx_msg void OnEnChangeEditNozProj();
	afx_msg void OnBnClickedRadioShell();
	afx_msg void OnBnClickedRadioAppoval();
	afx_msg void OnBnClickedRadioRghtoval();
};
