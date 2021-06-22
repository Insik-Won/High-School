// CFigureSegm.cpp: 구현 파일
//

#include "pch.h"
#include "Weight Ckeck.h"
#include "CFigureSegm.h"
#include "afxdialogex.h"

using namespace MrDoubleFormat;

// CFigureSegm 대화 상자

IMPLEMENT_DYNAMIC(CFigureSegm, CDialogEx)

CFigureSegm::CFigureSegm(CBasicSgmMngDlg* pParent /*=nullptr*/)
	: CBasicSegment(IDD_Segm_Figure, pParent)
	, m_dResult(0)
	, m_strNum(_T("1"))
	, m_bIsExVisible(FALSE)
{
	SetInputArrlen(4);
	for (int i = 0; i < 4; i++) {
		m_szaDefault[i] = _T("");
	}
}

BOOL CFigureSegm::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);

	return TRUE;
}

CFigureSegm::~CFigureSegm()
{
}

void CFigureSegm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Edit_1, m_szaInput[0]);
	DDX_Text(pDX, IDC_Edit_2, m_szaInput[1]);
	DDX_Text(pDX, IDC_Edit_3, m_szaInput[2]);
	DDX_Text(pDX, IDC_Edit_4, m_szaInput[3]);
	DDX_Text(pDX, IDC_Edit_Num, m_strNum);
	DDX_Text(pDX, IDC_Edit_Result, m_dResult);
}


BEGIN_MESSAGE_MAP(CFigureSegm, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_FCALC, &CFigureSegm::OnCalculate)
	ON_EN_CHANGE(IDC_Edit_1, &CFigureSegm::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_Edit_2, &CFigureSegm::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_Edit_3, &CFigureSegm::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_Edit_4, &CFigureSegm::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_Edit_Num, &CFigureSegm::OnEnChangeEdit)
	ON_EN_KILLFOCUS(IDC_Edit_1, &CFigureSegm::OnEnKillFocusEdit1)
	ON_EN_KILLFOCUS(IDC_Edit_2, &CFigureSegm::OnEnKillFocusEdit2)
	ON_EN_KILLFOCUS(IDC_Edit_3, &CFigureSegm::OnEnKillFocusEdit3)
	ON_EN_KILLFOCUS(IDC_Edit_4, &CFigureSegm::OnEnKillFocusEdit4)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CFigureSegm::SetEditVisible(int index, BOOL bFlag) {
	GetDlgItem(IDC_Edit_1 + index - 1)->ShowWindow(SW_SHOW * bFlag);
}

//BOOL CFigureSegm::isEditVisible(int index) {
//	return GetDlgItem(IDC_Edit_1 + index - 1)->IsWindowVisible();
//}

void CFigureSegm::SetVisibleEx(BOOL bFlag) {
	GetDlgItem(IDC_Edit_4)->ShowWindow(SW_SHOW * bFlag);
	m_bIsExVisible = bFlag;
}
BOOL CFigureSegm::IsVisibleEx() {
	return m_bIsExVisible;
}

int CFigureSegm::HowManyEdits() {
	return 3 + m_bIsExVisible;
}


void CFigureSegm::SetDefaults(LPCTSTR* strArr) {
	for (int i = 0; i < 4; i++) {
		m_szaDefault[i] = strArr[i];
		m_szaInput[i] = m_szaDefault[i];
	}
	UpdateData(FALSE);
}

CString* CFigureSegm::GetDefaults() {
	return m_szaDefault;
}

double CFigureSegm::GetResult() {
	return m_dResult;
}

void CFigureSegm::SetResult(const double dnum) {
	m_dResult = dnum;
	UpdateData(FALSE);
}

// CFigureSegm 메시지 처리기

void CFigureSegm::OnPaint()
{
	CPaintDC dc(this);

	CImage Image;
	if (m_BitmapID != 0) {
		Image.LoadFromResource(AfxGetInstanceHandle(), m_BitmapID);
		Image.BitBlt(dc.m_hDC, 5, 4);
	}
}

LRESULT CFigureSegm::OnCalculate(WPARAM wParam, LPARAM lParam) {
	UpdateData(TRUE);

	int editnum = HowManyEdits();
	MrDArray daInput(editnum);

	for (int i = 0; i < editnum; i++) {
		CheckAndAtof(m_szaInput[i], &daInput[i]);
	}

	int totalnum;
	CheckAndAtoi(m_strNum, &totalnum);

	m_pvfCalculate(daInput, &m_dResult);
	m_dResult *= 0.000001;
	m_dResult *= GetSpGravity();
	m_dResult *= totalnum;
	m_dResult = RoundOff(m_dResult, GetDigit());
	UpdateData(FALSE);

	return 0;
}

void CFigureSegm::OnEnChangeEdit()
{
	PostMessage(WM_FCALC, 0, 0);
	m_pParent->PostMessage(WM_FCALCRESULT, 0, 0);
}

void CFigureSegm::OnEnKillFocusEdit1()
{
	UpdateData(TRUE);
	if (m_szaInput[0] == _T("")) {
		m_szaInput[0] = m_szaDefault[0];
		UpdateData(FALSE);
		return;
	}
}

void CFigureSegm::OnEnKillFocusEdit2()
{
	UpdateData(TRUE);
	if (m_szaInput[1] == _T("")) {
		m_szaInput[1] = m_szaDefault[1];
		UpdateData(FALSE);
		return;
	}
}

void CFigureSegm::OnEnKillFocusEdit3()
{
	UpdateData(TRUE);
	if (m_szaInput[2] == _T("")) {
		m_szaInput[2] = m_szaDefault[2];
		UpdateData(FALSE);
		return;
	}
}

void CFigureSegm::OnEnKillFocusEdit4()
{
	UpdateData(TRUE);
	if (m_szaInput[3] == _T("")) {
		m_szaInput[3] = m_szaDefault[3];
		UpdateData(FALSE);
		return;
	}
}


BOOL CFigureSegm::PreTranslateMessage(MSG* pMsg)
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

	return CBasicSegment::PreTranslateMessage(pMsg);
}


HBRUSH CFigureSegm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBasicSegment::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) {
	case CTLCOLOR_EDIT:
		if (1005 < pWnd->GetDlgCtrlID() && pWnd->GetDlgCtrlID() < 1015)
		{
			pDC->SetTextColor(RGB(0, 51, 204));
			return (HBRUSH)(m_pParent->GetEditBrush()->GetSafeHandle());
		}
	default:
		return hbr;
	}
}
