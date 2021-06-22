#pragma once
#include "CGeoGraphDlg.h"
#include "CKoppenIconWnd.h"


// COption3Dlg 대화 상자

class COption3Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(COption3Dlg)

public:
	COption3Dlg(CWnd* pParent = nullptr, CGeoGraphDlg* pTarget1 = nullptr, CKoppenIconWnd* pTarget2 = nullptr);   // 표준 생성자입니다.
	virtual ~COption3Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTION3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CMrStatic m_group_graph;
	CMrStatic m_group_icon;

	CMrStatic m_statics_graph[2];
	CMrStatic m_statics_icon[5];

	CButton m_buttons_graph[2];
	CButton m_buttons_icon[5];

	Color m_colors_graph[2];
	Color m_colors_icon[5];

	CFont m_font;

	CGeoGraphDlg* m_pGeoGraphDlg;
	CKoppenIconWnd* m_pKoppenIconWnd;
	
private:
	void updateToTarget();
	void updateFromTarget();

	void openColorDlgForGraph(int idx);
	void openColorDlgForIcons(int idx);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetInfo(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButtonGraph1();
	afx_msg void OnBnClickedButtonGraph2();
	afx_msg void OnBnClickedButtonIcon1();
	afx_msg void OnBnClickedButtonIcon2();
	afx_msg void OnBnClickedButtonIcon3();
	afx_msg void OnBnClickedButtonIcon4();
	afx_msg void OnBnClickedButtonIcon5();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
