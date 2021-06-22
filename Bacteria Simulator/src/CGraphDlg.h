#pragma once
#include "CSimulDlg.h"

// CGraphDlg 대화 상자

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGraphDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CSimulDlg* m_pSimulDlg;		// 자료를 수집할 세균 시뮬레이터의 포인터를 가져옴
	vector<double> m_vector;	// 수집한 자료를 넣어둘 동적배열 선언
	int m_offset;				// 동적 배열에서 그래프에 보여줄 시작 인덱스

	int num;

public:
	vector<double>& getVector(); // 벡터 구하는 함수
	CSimulDlg* getSimulDlg();	 // 세균 세뮬레이터의 포인터를 구하는 함수
	int getOffset();			 // m_offset의 get 함수
	void setOffset(int offset);  // m_offset의 set 함수
	void Restart();				 // 초기화 함수

public:
	virtual BOOL Create(UINT nIDTemplate, CSimulDlg* pSimulDlg, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnPaint();
	afx_msg void OnClose();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
