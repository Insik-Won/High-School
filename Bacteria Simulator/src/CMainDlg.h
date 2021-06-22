
// Antibiotic Immune SimulatorDlg.h: 헤더 파일
//

#pragma once

#include "CSimulDlg.h"
#include "CGraphDlg.h"
#include "CSumDlg.h"


// CMainDlg 대화 상자
class CMainDlg : public CDialogEx
{
public:
	CMainDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

public:
	CSimulDlg m_SimulDlg; // 세균 시뮬레이터
	CGraphDlg m_GraphDlg; // 그래프 창
	CSumDlg	m_SumDlg;

	BOOL m_bMutateFlag;

public:
	CSumDlg* GetSumDlg();	// 요약창 Get 함수
	void Restart();			// 초기화 함수

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnBnClickedButtonSpreadAntibiotic();
	afx_msg void OnBnClickedButtonMutate();
	afx_msg void OnBnClickedButtonNowstate();

	afx_msg void OnMenuOption();
	afx_msg void OnMenuHelp();

	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
