// CStandardSegm.cpp: 구현 파일
//

#include "pch.h"
#include "Weight Ckeck.h"
#include "CStandardSegm.h"
#include "afxdialogex.h"

using namespace std;
using namespace MrDoubleFormat;

// CStandardSegm 대화 상자

IMPLEMENT_DYNAMIC(CStandardSegm, CDialogEx)

CStandardSegm::CStandardSegm(CBasicSgmMngDlg* pParent /*=nullptr*/)
	: CBasicSegment(IDD_Segm_Standard, pParent)
	, m_dResult(0)
	, m_strNum(_T("1"))
	, m_bIsNumBased(FALSE)
{
	SetInputArrlen(2);
}


BOOL CStandardSegm::OnInitDialog()
{
	CBasicSegment::OnInitDialog();

	m_szaInput[0] = _T("");
	m_szaInput[1] = _T("1000");
	UpdateData(FALSE);

	return TRUE;
}


CStandardSegm::~CStandardSegm()
{
}

void CStandardSegm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Edit_1, m_szaInput[0]);
	DDX_Text(pDX, IDC_Edit_2, m_szaInput[1]);
	DDX_Text(pDX, IDC_Edit_Num, m_strNum);
	DDX_Text(pDX, IDC_Edit_Result, m_dResult);
	DDX_Control(pDX, IDC_Combo_Standard, m_Combo);
}


BEGIN_MESSAGE_MAP(CStandardSegm, CDialogEx)
	ON_MESSAGE(WM_SCALC, &CStandardSegm::OnCalculate)
	ON_CBN_SELCHANGE(IDC_Combo_Standard, &CStandardSegm::OnCbnSelchangeComboStandard)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_Edit_1, &CStandardSegm::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_Edit_2, &CStandardSegm::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_Edit_Num, &CStandardSegm::OnEnChangeEdit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


CEdit* CStandardSegm::GetEdit(int nIndex) {
	ASSERT(nIndex < 3);
	return (CEdit*)GetDlgItem(IDC_Edit_1 + 1);
}

CComboBox* CStandardSegm::GetComboBox() {
	return &m_Combo;
}

CArray<CString>* CStandardSegm::GetArray() {
	return &m_dcaValue;
}

void CStandardSegm::SetFlag(BOOL bFlag) {
	m_bIsNumBased = bFlag;
	GetEdit(2)->ShowWindow(SW_SHOW*!bFlag);
}
BOOL CStandardSegm::GetFlag() {
	return m_bIsNumBased;
}

// CStandardSegm 메시지 처리기

void CStandardSegm::OnPaint()
{
	CPaintDC dc(this);
	CImage Image;
	Image.LoadFromResource(AfxGetInstanceHandle(), m_BitmapID);
	Image.BitBlt(dc.m_hDC, 3, 4);
}


LRESULT CStandardSegm::OnCalculate(WPARAM wParam, LPARAM lParam) {
	UpdateData(TRUE);

	MrDArray daInput(2);
	for (int i = 0; i < 2; i++) {
		CheckAndAtof(m_szaInput[i], &daInput[i]);
	}

	int totalnum = 0;
	CheckAndAtoi(m_strNum, &totalnum);

	m_dResult = daInput[0];
	if (m_bIsNumBased == FALSE) {
		m_dResult *= daInput[1];
		m_dResult *= 0.001;
	}

	m_dResult *= totalnum;
	m_dResult = RoundOff(m_dResult, GetDigit());
	UpdateData(FALSE);

	return 0;
}

void CStandardSegm::LoadDataFromFile(const CString& FileName, BOOL bFlag) {
	wifstream wfin(_T("Data\\Standard\\") + FileName + _T(".DAT"));
	CString strTmp = _T("");
	wstring wstr;
	double dTemp = 0;

	if (bFlag == TRUE) {
		while (!wfin.eof()) {
			wfin >> wstr;
			strTmp.Format(_T("%s"), wstr.c_str());
			strTmp.Trim();
			strTmp.Replace('_', ' ');
			m_Combo.InsertString(-1, strTmp);

			wfin >> wstr;
			strTmp.Format(_T("%s"), wstr.c_str());
			strTmp.Trim();
			m_dcaValue.Add(strTmp);
		}
	}
	else {
		while (!wfin.eof()) {
			wfin >> wstr;
			strTmp.Format(_T("%s"), wstr.c_str());
			strTmp.Trim();
			strTmp.Replace('_', ' ');
			m_Combo.InsertString(-1, strTmp);

			wfin >> wstr;
			strTmp.Format(_T("%s"), wstr.c_str());
			strTmp.Trim();
			CheckAndAtof(strTmp, &dTemp);
			FormatDouble(&strTmp, dTemp * 0.001, -1);
			m_dcaValue.Add(strTmp);
		}
	}

	wfin.close();

	m_Combo.SetCurSel(0);
	m_szaInput[0] = m_dcaValue[m_Combo.GetCurSel()];
	UpdateData(FALSE);

	PostMessage(WM_SCALC, 0, 0);
}

void CStandardSegm::OnCbnSelchangeComboStandard()
{
	UpdateData(TRUE);
	m_szaInput[0] = m_dcaValue[m_Combo.GetCurSel()];
	UpdateData(FALSE);
	PostMessage(WM_SCALC, 0, 0);
}


void CStandardSegm::OnEnChangeEdit()
{
	PostMessage(WM_SCALC, 0, 0);
}


BOOL CStandardSegm::PreTranslateMessage(MSG* pMsg)
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


HBRUSH CStandardSegm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
