#pragma once

#include "CGeoGraphDlg.h"

// COption1Dlg 대화 상자

class COption1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(COption1Dlg)

public:
	COption1Dlg(CWnd* pParent = nullptr, CGeoGraphDlg* pTarget = nullptr);   // 표준 생성자입니다.
	virtual ~COption1Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTION1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CMrStatic m_Group_temp;
	CMrStatic m_Group_prcp;

	CMrStatic m_statics_temp[12];
	CMrStatic m_statics_prcp[12];
	CEdit m_edits_temp[12];
	CEdit m_edits_prcp[12];

	CFont m_font;

	CGeoGraphDlg* m_pGeoGraphDlg;

private:
	void updateToTarget();
	void updateFromTarget();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetInfo(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
