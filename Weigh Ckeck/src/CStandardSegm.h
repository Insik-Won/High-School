#pragma once
#include "CBasicClasses.h"

#include <iostream>
#include <fstream>

// CStandardSegm 대화 상자

class CStandardSegm : public CBasicSegment
{
	DECLARE_DYNAMIC(CStandardSegm)

public:
	CStandardSegm(CBasicSgmMngDlg* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStandardSegm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Segm_Standard };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_Combo;
	CString m_strNum;
	double m_dResult;

private:
	CArray<CString> m_dcaValue;
	BOOL m_bIsNumBased;

public:
	CEdit* GetEdit(int nIndex);
	CComboBox* GetComboBox();
	void SetFlag(BOOL bFlag);
	BOOL GetFlag();
	CArray<CString>* GetArray();


public:
	void LoadDataFromFile(const CString& FileName, BOOL bFlag = TRUE);

public:
	virtual BOOL OnInitDialog();
	LRESULT afx_msg OnCalculate(WPARAM wParam = NULL, LPARAM lParam = NULL);
	afx_msg void OnCbnSelchangeComboStandard();
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
