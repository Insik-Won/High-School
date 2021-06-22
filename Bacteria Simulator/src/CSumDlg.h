#pragma once
#include "CGraphDlg.h"


// CSumDlg 대화 상자

class CSumDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSumDlg)

public:
	CSumDlg(CWnd* pParent = nullptr, CGraphDlg* pGraphDlg = nullptr);   // 표준 생성자입니다.
	virtual ~CSumDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SUMMARIZE };
#endif

public:
	virtual BOOL Create(UINT nIDTemplate, CGraphDlg* pGraphDlg, CWnd* pParentWnd = NULL);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
	CGraphDlg* m_pGraphDlg; // 데이터를 옮겨받을 CGrpahDlg의 포인터
	RectF m_gRect;			// 그래프를 그릴 직사각형 창

public:
	void DrawGraph(Graphics& graphics, RectF rect, int num, Pen* pen); // 그래프를 그리는 함수

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonClear();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
