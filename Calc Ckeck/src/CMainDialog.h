
// CMainDialog.h: 헤더 파일
//

#pragma once
#include "CBaseCalcDialog.h"
#include "CNozzleDlg.h"
#include "CClipDlg.h"
#include <filesystem>


// CMainDialog 대화 상자
class CMainDialog : public CBaseCalcDialog
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
	DECLARE_MESSAGE_MAP();

private:
	CString m_strFileDir;
public:
	CString GetFileDir();
	void SetFileDir(const CString& FileDir);

private:
	COleDateTime m_dtAfter2years;
	CString m_strPassword;

public:
	int m_iTempTabSel;
	CTabCtrl m_MainTab;
	CNozzleDlg m_NozzleDlg;
	CClipDlg m_ClipDlg;
	CCalcDialog* m_pwndShow;

public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuFileload();
	afx_msg void OnMenuFilesave();
	afx_msg void OnMenuHelp();
	afx_msg void OnMenuOption();

	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();

public:
	CComboBox m_ImgCombo;
	afx_msg void OnBnClickedButtonSetimage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
