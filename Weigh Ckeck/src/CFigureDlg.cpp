// CFigureDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Weight Ckeck.h"
#include "CFigureDlg.h"
#include "afxdialogex.h"

using namespace CalculateFFunc;
using namespace MrDoubleFormat;

LPCTSTR szaDefault[fnum][4] =
{
	 {_T("t"),	_T("W"),	_T("L"),	_T("")}
	,{_T("t"),	_T("ID"),	_T("L"),	_T("")}
	,{_T("L"),	_T(""),		_T("OD"),	_T("")}
	,{_T("t"),	_T("ID"),	_T("OD"),	_T("")}
	,{_T("t"),	_T(""),		_T("OD"),	_T("")}
	,{_T("t"),	_T("OD"),	_T("L"),	_T("")}
	,{_T("t"),	_T("H"),	_T("L1"),	_T("L2")}
	,{_T("t"),	_T("IR"),	_T("OR"),	_T("degree")}
	,{_T("t"),	_T("ID"),	_T("SF"),	_T("")}
	,{_T("t"),	_T("IR"),	_T("SF"),	_T("")}
	,{_T("t"),	_T("L.ID"),	_T("S.ID"),	_T("H")}
};

// CFigureDlg 대화 상자

IMPLEMENT_DYNAMIC(CFigureDlg, CDialogEx)

CFigureDlg::CFigureDlg(CWnd* pParent /*=nullptr*/)
	: CBasicSgmMngDlg(IDD_FIGURE, pParent)
{
	m_pcbaSegments.ClearAndSetLen(fnum);
	for (int i = 0; i < fnum; i++) {
		m_pcbaSegments[i] = &m_FigureSegm[i];
	}
	m_pCalcFuncs = &CalcfFuncs;
}

CFigureDlg::~CFigureDlg()
{
}

void CFigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFigureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_FCALCRESULT, &CFigureDlg::OnCalcResult)
END_MESSAGE_MAP()


// CFigureDlg 메시지 처리기


BOOL CFigureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect Rect(0, 0, 750, 24);
	Rect.top += 40;
	Rect.bottom += 40;


	CString strTmp = _T("");
	for (int i = 0; i < fnum; i++) {
		m_FigureSegm[i].Create(IDD_Segm_Figure, this);
		m_FigureSegm[i].SetCalcFunc((*m_pCalcFuncs)[i]);
		m_FigureSegm[i].SetBitmapID(fBID[i]);
		m_FigureSegm[i].SetDefaults(szaDefault[i]);

		strTmp.LoadStringW(IDS_FN_PLATE + i);
		m_FigureSegm[i].GetDlgItem(IDC_Static_Name)->SetWindowText(strTmp);

		m_FigureSegm[i].SetWindowPos(NULL, Rect.left, Rect.top, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW | SWP_SHOWWINDOW);
		Rect.top += 24 + 10;
		Rect.bottom += 24 +10;
	}

	m_FigureSegm[2].SetEditVisible(2, FALSE);
	m_FigureSegm[4].SetEditVisible(2, FALSE);

	m_FigureSegm[6].SetVisibleEx(TRUE);
	m_FigureSegm[7].SetVisibleEx(TRUE);
	m_FigureSegm[10].SetVisibleEx(TRUE);

	m_fsResult.Create(IDD_Segm_Figure, this);
	m_fsResult.GetDlgItem(IDC_Static_Name)->SetWindowTextW(_T(""));
	m_fsResult.GetDlgItem(IDC_Static_Asterisk)->SetWindowTextW(_T("합계"));
	m_fsResult.GetDlgItem(IDC_Edit_Num)->ShowWindow(SW_HIDE);
	
	CRect rct;
	m_fsResult.GetDlgItem(IDC_Edit_Num)->GetWindowRect(&rct);
	m_fsResult.ScreenToClient(rct);
	m_fsResult.GetDlgItem(IDC_Static_Asterisk)->SetWindowPos(NULL, rct.left, rct.top + 3, rct.Width(), rct.Height(), SWP_SHOWWINDOW | SWP_SHOWWINDOW);
	m_fsResult.SetWindowPos(NULL, Rect.left, Rect.top, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW | SWP_SHOWWINDOW);
	for (int i = 1; i <= 4; i++) m_fsResult.SetEditVisible(i, false);

	return TRUE;
}

LRESULT afx_msg CFigureDlg::OnCalcResult(WPARAM wParam, LPARAM lParam) {
	double Result = 0;
	for (int i = 0; i < fnum; i++) {
		Result += m_FigureSegm[i].GetResult();
	}

	m_fsResult.SetResult(Result);
	return 0;
}

void CFigureDlg::SetDigit(int iDigit) {
	CBasicSgmMngDlg::SetDigit(iDigit);
	for (int i = 0; i < fnum; i++) {
		m_FigureSegm[i].PostMessage(WM_FCALC);
	}
}

void CFigureDlg::SetSpGravity(double dSpGravity) {
	CBasicSgmMngDlg::SetSpGravity(dSpGravity);
	for (int i = 0; i < fnum; i++) {
		m_FigureSegm[i].PostMessage(WM_FCALC);
	}
}

namespace CalculateFFunc {

	void DefaultCalc(const MrDArray& daInput, double* Result) {
		*Result = daInput[0] + daInput[1] + daInput[2];
	}

	pvFunc CalcfFuncs[fnum] = {
		 PlateCalc
		,ShellCalc
		,RoundBarCalc
		,DonutCalc
		,CircleCalc
		,PipeCalc
		,TrapezoidCalc
		,SectorCalc
		,HeadCalc
		,HemiHeadCalc
		,ConeCalc
	};

	void PlateCalc(const MrDArray& daInput, double* Result) {
		*Result = daInput[0] * daInput[1] * daInput[2];
	} //0

	void ShellCalc(const MrDArray& daInput, double* Result) {
		double t = daInput[0];
		double ID = daInput[1];
		double L = daInput[2];
		*Result = (CA(ID/2+t) - CA(ID/2)) * L;
	} //1

	void RoundBarCalc(const MrDArray& daInput, double* Result) {
		double L = daInput[0];
		double OD = daInput[2];
		*Result = CA(OD / 2) * L;
	} //2

	void DonutCalc(const MrDArray& daInput, double* Result) {
		double t = daInput[0];
		double ID = daInput[1];
		double OD = daInput[2];
		*Result = (CA(OD / 2) - CA(ID / 2)) * t;
	} //3

	void CircleCalc(const MrDArray& daInput, double* Result) {
		double t = daInput[0];
		double OD = daInput[2];
		*Result = CA(OD / 2) * t;
	} //4

	void PipeCalc(const MrDArray& daInput, double* Result) {
		double t = daInput[0];
		double OD = daInput[1];
		double L = daInput[2];
		*Result = (CA(OD / 2) - CA(OD / 2 - t)) * L;
	} //5

	void TrapezoidCalc(const MrDArray& daInput, double* Result) {
		ASSERT(daInput.arrlen() == 4);

		double t = daInput[0];
		double H = daInput[1];
		double L1 = daInput[2];
		double L2 = daInput[3];

		*Result = (L1 + L2) / 2 * H * t;
	} //6

	void SectorCalc(const MrDArray& daInput, double* Result) {
		ASSERT(daInput.arrlen() == 4);

		double t = daInput[0];
		double IR = daInput[1];
		double OR = daInput[2];
		double degree = daInput[3];

		*Result = (CA(OR) - CA(IR)) * degree / 360 * t;
	} //7

	void HeadCalc(const MrDArray& daInput, double* Result) {
		double t = daInput[0];
		double ID = daInput[1];
		double L = daInput[2];

		double D = (ID * 1.2212) + (L * 2);

		*Result = CA(D / 2) * t;
	} //8

	void HemiHeadCalc(const MrDArray& daInput, double* Result) {
		double t = daInput[0];
		double IR = daInput[1];
		double SF = daInput[2];
		double h = IR - SF;

		double VO = PI * pow(h + t, 2) * (3 * (IR + t) - (h + t)) / 3;
		double VI = PI * pow(h, 2) * (3 * (IR) - h) / 3;

		*Result = VO - VI;
	} //9

	void ConeCalc(const MrDArray& daInput, double* Result) {
		ASSERT(daInput.arrlen() == 4);
		double t = daInput[0];
		double LID = daInput[1];
		double SID = daInput[2];
		double H = daInput[3];
		
		double a = atan2((LID - SID)/2,H);
		double tt = t / cos(a);

		*Result = TruncCone(LID / 2 + tt, SID / 2 + tt, H) - TruncCone(LID / 2, SID / 2, H);
	} //10
}

BOOL CFigureDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			return true;
		}
	}

	return CBasicSgmMngDlg::PreTranslateMessage(pMsg);
}
