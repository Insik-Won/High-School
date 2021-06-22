
// CMainDialog.h: 헤더 파일
//

#pragma once
#include "CFigureDlg.h"
#include "CStandardDlg.h"

#include <iostream>
#include <fstream>

// CMainDialog 대화 상자
class CMainDialog : public CBasicCalcDlg
{
// 생성입니다.
public:
	CMainDialog(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Main_Dialog };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	CComboBox m_ComboSpGravity;

private:
	CString m_strSpGravity;

public:
	CTabCtrl m_MainTab;
	CBasicSgmMngDlg* m_pwndShow;

public:
	CFigureDlg m_FigureDlg;
	CStandardDlg m_StandardDlg;

public:
	void SetDigit(int Digit);
	void SetSpGravity(double dSpGravity);
	void GetSgDataFromFIle(CComboBox* combo, const CString& fileName);

public:
	afx_msg void OnEnChangeEditSpgravity();
	afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonImgon();

public:
	afx_msg void OnMnitemHelp();
	afx_msg void OnMnitemOption();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboSpgrvty();
	afx_msg void OnCbnEditchangeComboSpgrvty();
};
