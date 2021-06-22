#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

#include "CCalcDialog.h"

#include "CClipCalculator.h"
#include "MrDoubleFormat.h"

// CClipDlg 대화 상자

class CClipDlg : public CCalcDialog
{
	DECLARE_DYNAMIC(CClipDlg)

public:
	CClipDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CClipDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Clip_Length_Ckeck };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CClipCalculator m_ClipCalculator;
	MrpszArray m_pszaInput;
	MrpDArray m_pdaOutput;
	void UpdateClipLength();
public:
	virtual BOOL MakeFile(CString FileDir, const CString& FileName);
	virtual BOOL LoadFile(const CString& FileDir, const CString& FileName);
	virtual void UpdateDigit();
	virtual void DlgCalculate();

public:
	CString m_strShellOD;
	CString m_strShellOR;
	CString m_strDblWidth;
	CString m_strWidth;
	CString m_strPadThk;
	CString m_strClipThk;
	CString m_strClipProj;
	CString m_strClipLtD;
	CString m_strClipLength;
	CString m_strLengthB;

	double m_dResult1;
	double m_dResult2;
	double m_dResult3;
	CString m_strResult4;
public:
	afx_msg void OnEnChangeEditShellOd();
	afx_msg void OnEnChangeEditShellOr();
	afx_msg void OnEnChangeEditDblWidth();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditClipProj();
	afx_msg void OnEnChangeEditClipLtD();
	afx_msg void OnEnChangeEditClipLength();
	afx_msg void OnEnChangeEditPadThk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditClipThk();
	afx_msg void OnEnChangeEditLengthB();
};
