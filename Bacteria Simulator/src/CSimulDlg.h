#pragma once
#include "CBacteria.h"

// CSimulDlg 대화 상자

class CSimulDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSimulDlg)

public:
	CSimulDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSimulDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SIMUL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	int num;
	// 가로세로 칸의 개수

	MrColor m_BlueColor;

	bool m_bMutateFlag;
	bool m_bSpreadFlag;
	REAL m_Spread_Radius;
	REAL m_Spread_Speed;
	// 항생제 살포 관련 변수들

	CBacteria** m_bacterias;
	// CBacteria 2차원 동적 배열 

public:
	void DrawBacteria(Graphics& graphic, REAL x, REAL y, REAL radius);	// 박테리아를 그리는 함수
	void ReproduceBacteria(CBacteria** bacterias, int x, int y);		// 박테리아를 번식시키는 함수
	double average_fatality();		// 평균 치사율을 구하는 함수			
	MrColor GetBlueColor();			// 현재 세균색을 구하는 함수
	void Restart();					// 초기화 함수

	void SetGridNumber(int inum);	// 격자의 칸 가로세로 개수를 Set하는 함수
	int GetGridNumber();			// 격자의 칸 가로세로 개수를 Get하는 함수

	void SetSpreadSpeed(double inum);	// 현재 항생제의 속도를 Set하는 함수
	REAL GetSpreadSpeed();				// 현재 항생제의 속도를 Get하는 함수

public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRestart();

	afx_msg LRESULT OnSpreadAntibiotic(WPARAM wParam = NULL, LPARAM lParam = NULL);
	afx_msg LRESULT OnMutateChange(WPARAM wParam = NULL, LPARAM lParam = NULL);

	virtual void PostNcDestroy();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
