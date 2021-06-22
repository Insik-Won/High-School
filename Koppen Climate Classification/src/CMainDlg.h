
// CMainDlg.h: 헤더 파일
//
#pragma once

#include "CGeoGraphDlg.h"
#include "CKoppenCalculator.h"
#include "CKoppenIconWnd.h"
#include "CKoppenOptionDlg.h"
#include "CHelpDlg.h"

// CMainDlg 대화 상자
class CMainDlg : public CDialog
{
public:
	CMainDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN};
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

public:
	static double window_ratio; // width / height;
	const static int left_side;
	const static int right_side;

	const static int info1_side;
	const static int info2_side;
	const static int info3_side;

	int getSplitLine();

private:
	CGeoGraphDlg m_GeoGraphDlg;
	CKoppenCalculator m_KoppenCalculator;

	unordered_map<string, wstring> m_koppen_info_map;

	CMrStatic m_staticInfo1;
	CKoppenIconWnd m_staticInfo2;
	CMrStatic m_staticInfo3;

	CButton m_Button_tempOnly;
	CButton m_Button_BothShow;
	CButton m_Button_precipOnly;

	CFont m_info_font;
	CFont m_button_font;

	CKoppenOptionDlg m_OptionDlg;

private:
	void UpdateClimateInfos(LPCTSTR filename_tag, LPCTSTR filename_info);

	void UpdateInfo1();
	void UpdateInfo2(const string& sign);
	void UpdateInfo3(const string& sign);

public:
	CGeoGraphDlg* getGeoGraphDlg();
	CKoppenIconWnd* getKoppenIconWnd();

public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonOnlyTemp();
	afx_msg void OnBnClickedButtonOnlyPrecip();
	afx_msg void OnBnClickedButtonBoth();
	afx_msg LRESULT OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam);

	afx_msg void OnMenuSave();
	afx_msg void OnMenuLoad();

	afx_msg void OnImageSave1();
	afx_msg void OnImageSave2();

	afx_msg void OnMenuOption();
	afx_msg void OnMenuHelp();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	afx_msg void OnClose();
};
