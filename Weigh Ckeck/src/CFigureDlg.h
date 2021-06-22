#pragma once
#include "CFigureSegm.h"

// CFigureDlg 대화 상자

const int fnum = 11;
const int fBID[fnum] = {
	IDB_FIGURE_PLATE
	,IDB_FIGURE_SHELL
	,IDB_FIGURE_RoundBar
	,IDB_FIGURE_DONUT
	,IDB_FIGURE_CIRCLE
	,IDB_FIGURE_PIPE
	,IDB_FIGURE_TRAPEZOID
	,IDB_FIGURE_SECTOR
	,IDB_FIGURE_HEAD
	,IDB_FIGURE_HEAD
	,IDB_FIGURE_CONE
};

extern LPCTSTR szaDefault[fnum][4];

class CFigureDlg : public CBasicSgmMngDlg
{
	DECLARE_DYNAMIC(CFigureDlg)

public:
	CFigureDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFigureDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIGURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	CFigureSegm m_FigureSegm[fnum];
	CFigureSegm m_fsResult;
	pvFunc (*m_pCalcFuncs)[fnum];
public:
	virtual BOOL OnInitDialog();
	void SetDigit(int iDigit);
	void SetSpGravity(double dSpGravity);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT afx_msg OnCalcResult(WPARAM wParam = NULL, LPARAM lParam = NULL);
};

namespace CalculateFFunc {
	extern pvFunc CalcfFuncs[fnum];

	void DefaultCalc	(const MrDArray& daInput, double* Result);	//Default
	void PlateCalc		(const MrDArray& daInput, double* Result);	//0
	void ShellCalc		(const MrDArray& daInput, double* Result);	//1
	void RoundBarCalc	(const MrDArray& daInput, double* Result);	//2
	void DonutCalc		(const MrDArray& daInput, double* Result);	//3
	void CircleCalc		(const MrDArray& daInput, double* Result);	//4
	void PipeCalc		(const MrDArray& daInput, double* Result);	//5
	void TrapezoidCalc	(const MrDArray& daInput, double* Result);	//6
	void SectorCalc		(const MrDArray& daInput, double* Result);	//7
	void HeadCalc		(const MrDArray& daInput, double* Result);	//8
	void HemiHeadCalc	(const MrDArray& daInput, double* Result);	//9
	void ConeCalc		(const MrDArray& daInput, double* Result);	//10

	inline double TruncCone(double R, double r, double H) {
		return (pow(R, 2) + R * r + pow(r, 2)) * PI * H / 3;
	}
}
