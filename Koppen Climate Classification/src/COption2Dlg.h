#pragma once
#include "CGeoGraphDlg.h"

// COption2Dlg 대화 상자

class COption2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(COption2Dlg)

public:
	COption2Dlg(CWnd* pParent = nullptr, CGeoGraphDlg* pTarget = nullptr);   // 표준 생성자입니다.
	virtual ~COption2Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTION2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CMrStatic m_group_temp;
	CMrStatic m_group_prcp;
	CMrStatic m_group_help;

	CMrStatic m_statics[6];
	CEdit m_edits[6];

	CMrStatic m_static_help;

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
