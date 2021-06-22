// CNozzleDlg.cpp: 구현 파일
//
#include "pch.h"
#include "Calc Ckeck.h"
#include "CNozzleDlg.h"
#include "afxdialogex.h"

#include "MrDoubleFormat.h"

using namespace std;
using namespace MrDoubleFormat;

// CNozzleDlg 대화 상자

const double CNozzleDlg::m_dmm[16] = { 21.34, 27.67, 33.4, 48.26, 60.33, 88.9, 114.3, 168.28, 219.08, 273.05, 323.85, 355.6, 406.4, 457.2, 508.0, 609.6 };

IMPLEMENT_DYNAMIC(CNozzleDlg, CDialogEx)

CNozzleDlg::CNozzleDlg(CWnd* pParent /*=nullptr*/)
	: CCalcDialog(IDD_Nozzle_Length_Ckeck, pParent)
	, m_iCalcMode(0)
	, m_strShellID(_T(""))
	, m_strShellRad(_T(""))
	, m_strShellThk(_T(""))
	, m_strHeadThk(_T(""))
	, m_strNozSize(_T(""))
	, m_strNozProj(_T(""))
	, m_strDistX(_T(""))
	, m_strDistY(_T(""))
	, m_strVesToCL(_T(""))
	, m_bVTisZero(TRUE)
	, m_dResult1(0)
	, m_dResult2(0)
	, m_dResult3(0)
	, m_dResult4(0)
	, m_strL1Rr(_T(""))
	, m_strL2Rr(_T(""))
	, m_pszaInput(5)
	, m_pdaOutput(4)
	, m_iTempSel(0)
{
	m_pszaInput[0] = &m_strShellID;
	m_pszaInput[2] = &m_strNozSize;
	m_pszaInput[3] = &m_strNozProj;
	m_pszaInput[4] = &m_strVesToCL;

	m_pdaOutput[0] = &m_dResult1;
	m_pdaOutput[1] = &m_dResult2;
	m_pdaOutput[2] = &m_dResult3;
	m_pdaOutput[3] = &m_dResult4;
}

// 시작 시 레지스트리 공간에서 수치 퍼오기
BOOL CNozzleDlg::OnInitDialog() {
	CBaseCalcDialog::OnInitDialog();

	m_iCalcMode		= AfxGetApp()->GetProfileIntW(_T("Nozzle"), _T("Mode"), 0);
	m_strShellID	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("SID"), _T("1000"));
	m_strShellRad	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("SRad"), _T("500"));
	m_strShellThk	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("STK"), _T("10"));
	m_strHeadThk	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("HTK"), _T("10"));
	m_strNozSize	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("NS"), _T("21.34"));
	int Sel			= AfxGetApp()->GetProfileIntW(_T("Nozzle"), _T("PS"), 0);
	m_ComboPipeSize.SetCurSel(Sel);
	m_strNozProj	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("NP"), _T("700"));
	m_strDistX		= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("DX"), _T("0"));
	m_strDistY		= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("DY"), _T("0"));
	m_strVesToCL	= AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("VT"), _T("0"));
	m_bVTisZero		= AfxGetApp()->GetProfileIntW(_T("Nozzle"), _T("ID"), TRUE);

	CString strTmp[4];
	strTmp[0] = AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("L1"), _T("0"));
	strTmp[1] = AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("L2"), _T("0"));
	strTmp[2] = AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("L3"), _T("0"));
	strTmp[3] = AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("L4"), _T("0"));

	for (int i = 0; i < 4; i++) {
		CheckAndAtof(strTmp[i], m_pdaOutput[i]);
	}

	m_strL1Rr = AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("Rr1"), _T(""));
	m_strL2Rr = AfxGetApp()->GetProfileStringW(_T("Nozzle"), _T("Rr2"), _T(""));

	m_pszaInput[1] = (m_iCalcMode == 0) ? &m_strShellThk : &m_strHeadThk;

	UpdateData(FALSE);

	return TRUE;
}

// 모든 게 다 사라지기 전에 UpdateData(TRUE) 및 m_ComboPipeSize에서 현재 셀 입력
void CNozzleDlg::OnDestroy()
{
	UpdateData(TRUE);
	m_iTempSel = m_ComboPipeSize.GetCurSel();
	CCalcDialog::OnDestroy();
}

// 끝나고 레지스트리 공간에 수치 등록
void CNozzleDlg::PostNcDestroy()
{
	AfxGetApp()->WriteProfileInt(_T("Nozzle"), _T("Mode"), m_iCalcMode);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("SID"), m_strShellID);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("SRad"), m_strShellRad);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("STK"), m_strShellThk);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("HTK"), m_strHeadThk);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("NS"), m_strNozSize);
	AfxGetApp()->WriteProfileInt(_T("Nozzle"), _T("PS"), m_iTempSel);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("NP"), m_strNozProj);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("DX"), m_strDistX);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("DY"), m_strDistY);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("VT"), m_strVesToCL);
	AfxGetApp()->WriteProfileInt(_T("Nozzle"), _T("ID"), m_bVTisZero);

	CString strTmp[4];
	for (int i = 0; i < 4; i++) {
		FormatDouble(&strTmp[i], *m_pdaOutput[i], GetDigit());
	}
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("L1"), strTmp[0]);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("L2"), strTmp[1]);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("L3"), strTmp[2]);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("L4"), strTmp[3]);

	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("Rr1"), m_strL1Rr);
	AfxGetApp()->WriteProfileStringW(_T("Nozzle"), _T("Rr2"), m_strL2Rr);

	CCalcDialog::PostNcDestroy();
}


CNozzleDlg::~CNozzleDlg()
{
}

void CNozzleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_Radio_Shell, m_iCalcMode);
	DDX_Check(pDX, IDC_Check_Ves_CL_0, m_bVTisZero);
	DDX_Text(pDX, IDC_Static_NL1, m_dResult1);
	DDX_Text(pDX, IDC_Static_NL2, m_dResult2);
	DDX_Text(pDX, IDC_Static_NL3, m_dResult3);
	DDX_Text(pDX, IDC_Static_NL4, m_dResult4);
	DDX_Text(pDX, IDC_Edit_Shell_ID, m_strShellID);
	DDX_Text(pDX, IDC_Edit_Shell_Rad, m_strShellRad);
	DDX_Text(pDX, IDC_Edit_Shell_THK, m_strShellThk);
	DDX_Text(pDX, IDC_Edit_Head_THK, m_strHeadThk);
	DDX_Text(pDX, IDC_Edit_Noz_Size, m_strNozSize);
	DDX_Text(pDX, IDC_Edit_Noz_Proj, m_strNozProj);
	DDX_Control(pDX, IDC_Combo_Pipe_Size, m_ComboPipeSize);
	DDX_Text(pDX, IDC_Edit_x_Dstnce, m_strDistX);
	DDX_Text(pDX, IDC_Edit_Y_Dstnce, m_strDistY);
	DDX_Text(pDX, IDC_Edit_Ves_CL_TO, m_strVesToCL);
	DDX_Text(pDX, IDC_L1_Rr, m_strL1Rr);
	DDX_Text(pDX, IDC_L2_Rr, m_strL2Rr);
}


BEGIN_MESSAGE_MAP(CNozzleDlg, CDialogEx)
	ON_EN_UPDATE(IDC_Edit_Shell_ID, &CNozzleDlg::OnEnUpdateEditShellId)
	ON_EN_CHANGE(IDC_Edit_Shell_Rad, &CNozzleDlg::OnEnChangeEditShellRad)
	ON_EN_CHANGE(IDC_Edit_Shell_THK, &CNozzleDlg::OnEnChangeEditShellThk)
	ON_EN_CHANGE(IDC_Edit_Head_THK, &CNozzleDlg::OnEnChangeEditHeadThk)
	ON_EN_CHANGE(IDC_Edit_Noz_Size, &CNozzleDlg::OnEnChangeEditNozSize)
	ON_EN_CHANGE(IDC_Edit_Noz_Proj, &CNozzleDlg::OnEnChangeEditNozProj)
	ON_CBN_SELCHANGE(IDC_Combo_Pipe_Size, &CNozzleDlg::OnCbnSelchangeComboPipeSize)
	ON_EN_CHANGE(IDC_Edit_x_Dstnce, &CNozzleDlg::OnEnChangeEditxDstnce)
	ON_EN_CHANGE(IDC_Edit_Y_Dstnce, &CNozzleDlg::OnEnChangeEditYDstnce)
	ON_EN_CHANGE(IDC_Edit_Ves_CL_TO, &CNozzleDlg::OnEnChangeEditVesClTo)
	ON_BN_CLICKED(IDC_Check_Ves_CL_0, &CNozzleDlg::OnBnClickedCheckVesClZero)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Radio_Shell, &CNozzleDlg::OnBnClickedRadioShell)
	ON_BN_CLICKED(IDC_Radio_AppOval, &CNozzleDlg::OnBnClickedRadioAppoval)
	ON_BN_CLICKED(IDC_Radio_RghtOval, &CNozzleDlg::OnBnClickedRadioRghtoval)
END_MESSAGE_MAP()



// CNozzleDlg 메시지 처리기

void CNozzleDlg::DlgCalculate()
{
	UpdateData(TRUE);

	static int ErrorEgg = 0;
	MrDArray daInput(5);

	m_pszaInput[1] = (m_iCalcMode == 0) ? &m_strShellThk : &m_strHeadThk;

	m_NozzCalculator.SetCalcMode(m_iCalcMode);	

	for (int i = 0; i < 5; i++) {
		if (!CheckAndAtof(*(m_pszaInput[i]), &daInput[i], TRUE)) {
			/*CString strTmp;
			strTmp.LoadStringW(IDS_STRING102 + ErrorEgg);
			AfxMessageBox(strTmp);
			++ErrorEgg;
			if (ErrorEgg == 28) ErrorEgg = 0;
			// 계산하기 버튼이 사라지면서 더이상 쓰지 않게 됨.
			*/
			return;
		}
	}

	m_NozzCalculator.Calculate(daInput, m_pdaOutput);

	for (int i = 0; i < 4; i++) {
		*m_pdaOutput[i] = RoundOff(*m_pdaOutput[i], GetDigit());
	}

	if (m_iCalcMode != 1) {
		m_strL1Rr = _T("");
		m_strL2Rr = _T("");
	}
	else {
		CString strTmp = _T("");
		m_strL1Rr = m_NozzCalculator.GetbResultRr(0) ? _T("(LARGE ARC PART)") : _T("(SMALL ARC PART)");
		m_strL2Rr = m_NozzCalculator.GetbResultRr(1) ? _T("(LARGE ARC PART) : R") : _T("(SMALL ARC PART) : r");
		FormatDouble(&strTmp, m_NozzCalculator.GetdResultRr(1), GetDigit());
		m_strL2Rr += strTmp;
	}

	UpdateData(FALSE);
}

// 1st 라디오버튼 계산
void CNozzleDlg::OnBnClickedRadioShell()
{
	DlgCalculate();
}
// 2st 라디오버튼 계산
void CNozzleDlg::OnBnClickedRadioAppoval()
{
	DlgCalculate();
}
// 3st 라디오버튼 계산
void CNozzleDlg::OnBnClickedRadioRghtoval()
{
	DlgCalculate();
}

// ID 입력
void CNozzleDlg::OnEnUpdateEditShellId()
{
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strShellID, &dTemp)) {
		FormatDouble(&m_strShellRad, dTemp / 2, GetDigit());
		UpdateData(FALSE);
	}
	DlgCalculate();
}

// Rad 입력
void CNozzleDlg::OnEnChangeEditShellRad()
{
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strShellRad, &dTemp)) {
		FormatDouble(&m_strShellID, dTemp * 2, GetDigit());
		UpdateData(FALSE);
	}
	DlgCalculate();
}

// Shell Thk입력
void CNozzleDlg::OnEnChangeEditShellThk()
{
	DlgCalculate();
}
// Head Thk 입력
void CNozzleDlg::OnEnChangeEditHeadThk()
{
	DlgCalculate();
}
// NOZZLE SIZE 입력
void CNozzleDlg::OnEnChangeEditNozSize()
{
	DlgCalculate();
}
// NOZZLE PROJ 입력
void CNozzleDlg::OnEnChangeEditNozProj()
{
	DlgCalculate();
}

// PIPE SIZE 콤보박스
void CNozzleDlg::OnCbnSelchangeComboPipeSize()
{
	UpdateData(TRUE);
	int iTemp = m_ComboPipeSize.GetCurSel();
	FormatDouble(&m_strNozSize, m_dmm[iTemp], GetDigit());
	UpdateData(FALSE);
	DlgCalculate();
}

// X 거리 입력
void CNozzleDlg::OnEnChangeEditxDstnce()
{
	UpdateData(TRUE);

	double dTempX = 0, dTempY = 0;
	if (CheckAndAtof(m_strDistX, &dTempX) && CheckAndAtof(m_strDistY, &dTempY)) {
		double dTemp = hypot(dTempX, dTempY);
		FormatDouble(&m_strVesToCL, dTemp, GetDigit());
		m_bVTisZero = (dTemp == 0);
		UpdateData(FALSE);
	}
	DlgCalculate();
}

// Y 거리 입력
void CNozzleDlg::OnEnChangeEditYDstnce()
{
	UpdateData(TRUE);

	double dTempX = 0, dTempY = 0;
	if (CheckAndAtof(m_strDistX, &dTempX) && CheckAndAtof(m_strDistY, &dTempY)) {
		double dTemp = hypot(dTempX, dTempY);
		FormatDouble(&m_strVesToCL, dTemp, GetDigit());
		m_bVTisZero = (dTemp == 0);
		UpdateData(FALSE);
	}
	DlgCalculate();
}

// VESSEL CL TO 입력
void CNozzleDlg::OnEnChangeEditVesClTo()
{
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strVesToCL, &dTemp)) {
		m_bVTisZero = (dTemp == 0);
		UpdateData(FALSE);
	}
	DlgCalculate();
}

// VESSEL C.L 버튼
void CNozzleDlg::OnBnClickedCheckVesClZero()
{
	UpdateData(TRUE);
	if (m_bVTisZero) {
		m_strVesToCL = _T("0");
		UpdateData(FALSE);
	}
	else {
		double dTempX = 0, dTempY = 0;
		if (CheckAndAtof(m_strDistX, &dTempX) && CheckAndAtof(m_strDistY, &dTempY)) {
			double dTemp = hypot(dTempX, dTempY);
			FormatDouble(&m_strVesToCL, dTemp, GetDigit());
			m_bVTisZero = dTemp == 0;
			UpdateData(FALSE);
		}
	}
	DlgCalculate();
}

// 파일 저장 관련 함수

void CNozzleDlg::UpdateDigit() {
	OnEnUpdateEditShellId();
	OnEnChangeEditxDstnce();
	if (m_dResult1 != 0) {
		DlgCalculate();
	}
}

// 파일 저장하기
BOOL CNozzleDlg::MakeFile(CString FileDir, const CString& FileName) {
	CString OriginDir = FileDir;
	OriginDir.TrimRight(_T(".txt"));

	int i = 0;
	CFileFind finder;
	while (finder.FindFile(FileDir)) {
		++i;
		FileDir = OriginDir;
		FileDir.AppendFormat(_T("%d.txt"), i);
	}

	wofstream wfout(FileDir);

	wfout << _T("NOZZLE") << _T(" | ") << m_iCalcMode << endl;

	wfout << m_strShellID.GetString() << _T(" | ") << m_strShellRad.GetString() << endl;
	wfout << m_strShellThk.GetString() << _T(" | ") << m_strHeadThk.GetString() << endl;
	wfout << m_strNozSize.GetString() << _T(" | ") << m_ComboPipeSize.GetCurSel() << endl;
	wfout << m_strNozProj.GetString() << endl;
	wfout << m_strDistX.GetString() << _T(" | ") << m_strDistY.GetString() << endl;
	wfout << m_strVesToCL.GetString() << _T(" | ") << m_bVTisZero;

	if (i == 0) {
		AfxMessageBox(_T("파일이 성공적으로 만들어졌습니다."));
	}
	else {
		CString MdfiedFileName = FileName;
		MdfiedFileName.TrimRight(_T(".txt"));
		MdfiedFileName.AppendFormat(_T("%d.txt"), i);
		AfxMessageBox(_T("같은 이름의 파일이 있어 \"") + MdfiedFileName + _T("\" 라는 이름으로 저장하였습니다"));
	}

	return TRUE;
}

// 파일 불러오기
BOOL CNozzleDlg::LoadFile(const CString& FileDir, const CString& FileName) {
	wifstream wfin(FileDir);
	CString strTmp = _T("");
	CString* strAll[] = { &m_strShellID, &m_strShellRad, &m_strShellThk, &m_strHeadThk, &m_strNozSize, NULL, &m_strNozProj, &m_strDistX, &m_strDistY, &m_strVesToCL, NULL };
	wstring wstr;

	wfin >> wstr;
	strTmp.Format(_T("%s"), wstr.c_str());
	strTmp.Trim(_T("|"));
	if (strTmp != _T("NOZZLE")) {
		AfxMessageBox(_T("선택하신 텍스트 파일은 \"") + strTmp + _T("\" 전용입니다"));
		return FALSE;
	}
	wfin >> wstr >> m_iCalcMode;

	int i = 0;
	while (i < 11) {
		if (wfin.eof()) break;
		wfin >> wstr;
		if (wstr == _T("|")) continue;
		strTmp.Format(_T("%s"), wstr.c_str());
		strTmp.Trim(_T("|"));

		if (strAll[i] != NULL) {
			*strAll[i] = strTmp;
		}
		else {
			int iTemp = _wtoi(strTmp);
			if (i == 5) {
				m_ComboPipeSize.SetCurSel(iTemp - 1);
			}
			else if (i == 10) {
				m_bVTisZero = iTemp;
			}
		}
		++i;
	}

	UpdateData(FALSE);

	AfxMessageBox(_T("성공적으로 불러왔습니다."));
	DlgCalculate();
	return TRUE;
}

// Enter/ESC키를 누를 시 무시
BOOL CNozzleDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			DlgCalculate();
			return true;
		}
	}

	return CCalcDialog::PreTranslateMessage(pMsg);
}

// 색 바꾸기
HBRUSH CNozzleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CCalcDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) {
	case CTLCOLOR_EDIT:
		if (1000 < pWnd->GetDlgCtrlID() && pWnd->GetDlgCtrlID() < 1029)
		{
			pDC->SetTextColor(RGB(0, 51, 204));
			return (HBRUSH)(GetEditBrush()->GetSafeHandle());
		}
	default:
		return hbr;
	}
	return hbr;
}

