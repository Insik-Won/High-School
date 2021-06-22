// CClipDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Calc Ckeck.h"
#include "CClipDlg.h"
#include "afxdialogex.h"


using namespace std;
using namespace MrDoubleFormat;

// CClipDlg 대화 상자

IMPLEMENT_DYNAMIC(CClipDlg, CDialogEx)

CClipDlg::CClipDlg(CWnd* pParent /*=nullptr*/)
	: CCalcDialog(IDD_Clip_Length_Ckeck, pParent)
	, m_strShellOD(_T(""))
	, m_strShellOR(_T(""))
	, m_strDblWidth(_T(""))
	, m_strWidth(_T(""))
	, m_strPadThk(_T(""))
	, m_strClipThk(_T(""))
	, m_strClipProj(_T(""))
	, m_strClipLtD(_T(""))
	, m_strClipLength(_T(""))
	, m_strLengthB(_T(""))
	, m_dResult1(0)
	, m_dResult2(0)
	, m_dResult3(0)
	, m_strResult4(_T(""))
	, m_ClipCalculator()
	, m_pszaInput(6)
	, m_pdaOutput(3)
{
	m_pszaInput[0] = &m_strShellOR;
	m_pszaInput[1] = &m_strWidth;
	m_pszaInput[2] = &m_strPadThk;
	m_pszaInput[3] = &m_strClipThk;
	m_pszaInput[4] = &m_strClipProj;
	m_pszaInput[5] = &m_strLengthB;
	
	m_pdaOutput[0] = &m_dResult1;
	m_pdaOutput[1] = &m_dResult2;
	m_pdaOutput[2] = &m_dResult3;
}

// 시작 시 레지스트리 공간에서 수치 퍼오기
BOOL CClipDlg::OnInitDialog()
{
	CCalcDialog::OnInitDialog();

	m_strShellOD	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("OD"), _T("1000"));
	m_strShellOR	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("OR"), _T("500"));
	m_strDblWidth	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("DW"), _T("200"));
	m_strWidth		= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("WD"), _T("100"));
	m_strPadThk		= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("PT"), _T("0"));
	m_strClipThk	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("CT"), _T("0"));
	m_strClipProj	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("CP"), _T("700"));
	m_strClipLtD	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("LtD"), _T("200"));
	m_strClipLength	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("CL"), _T("210.1"));
	m_strLengthB	= AfxGetApp()->GetProfileStringW(_T("Clip"), _T("LB"), _T("0"));

	CString strTmp[3];
	strTmp[0] = AfxGetApp()->GetProfileStringW(_T("Clip"), _T("L1"), _T("0"));
	strTmp[1] = AfxGetApp()->GetProfileStringW(_T("Clip"), _T("L2"), _T("0"));
	strTmp[2] = AfxGetApp()->GetProfileStringW(_T("Clip"), _T("L3"), _T("0"));

	for (int i = 0; i < 3; i++) {
		CheckAndAtof(strTmp[i], m_pdaOutput[i]);
	}

	m_strResult4 = AfxGetApp()->GetProfileStringW(_T("Clip"), _T("L4"), _T(""));

	m_pEditBrush = new CBrush(RGB(255, 255, 255));

	UpdateData(FALSE);

	return TRUE;
}

// 모든 게 다 사라지기 전에 UpdateData(TRUE) 및 m_ComboPipeSize에서 현재 셀 입력
void CClipDlg::OnDestroy()
{
	UpdateData();
	CCalcDialog::OnDestroy();
}

// 끝나고 레지스트리 공간에 수치 등록
void CClipDlg::PostNcDestroy()
{
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("OD"), m_strShellOD);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("OR"), m_strShellOR);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("DW"), m_strDblWidth);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("WD"), m_strWidth);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("PT"), m_strPadThk);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("CT"), m_strClipThk);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("CP"), m_strClipProj);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("LtD"), m_strClipLtD);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("CL"), m_strClipLength);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("LB"), m_strLengthB);

	CString strTmp[3];
	for (int i = 0; i < 3; i++) {
		FormatDouble(&strTmp[i], *m_pdaOutput[i], GetDigit());
	}
	
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("L1"), strTmp[0]);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("L2"), strTmp[1]);
	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("L3"), strTmp[2]);

	AfxGetApp()->WriteProfileStringW(_T("Clip"), _T("L4"), m_strResult4);

	CCalcDialog::PostNcDestroy();
}

CClipDlg::~CClipDlg()
{
}

void CClipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Edit_Shell_OD, m_strShellOD);
	DDX_Text(pDX, IDC_Edit_Shell_OR, m_strShellOR);
	DDX_Text(pDX, IDC_Edit_Dbl_Width, m_strDblWidth);
	DDX_Text(pDX, IDC_Edit_Width, m_strWidth);
	DDX_Text(pDX, IDC_Edit_Pad_THK, m_strPadThk);
	DDX_Text(pDX, IDC_Edit_Clip_THK, m_strClipThk);
	DDX_Text(pDX, IDC_Edit_Clip_Proj, m_strClipProj);
	DDX_Text(pDX, IDC_Edit_Clip_LTD, m_strClipLtD);
	DDX_Text(pDX, IDC_Edit_Clip_Length, m_strClipLength);
	DDX_Text(pDX, IDC_Edit_Length_B, m_strLengthB);
	DDX_Text(pDX, IDC_Static_CL1, m_dResult1);
	DDX_Text(pDX, IDC_Static_CL2, m_dResult2);
	DDX_Text(pDX, IDC_Static_L3, m_dResult3);
	DDX_Text(pDX, IDC_Static_Clp_Stat, m_strResult4);
}


BEGIN_MESSAGE_MAP(CClipDlg, CDialogEx)
	ON_EN_CHANGE(IDC_Edit_Shell_OD, &CClipDlg::OnEnChangeEditShellOd)
	ON_EN_CHANGE(IDC_Edit_Shell_OR, &CClipDlg::OnEnChangeEditShellOr)
	ON_EN_CHANGE(IDC_Edit_Dbl_Width, &CClipDlg::OnEnChangeEditDblWidth)
	ON_EN_CHANGE(IDC_Edit_Width, &CClipDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_Edit_Clip_Proj, &CClipDlg::OnEnChangeEditClipProj)
	ON_EN_CHANGE(IDC_Edit_Clip_LTD, &CClipDlg::OnEnChangeEditClipLtD)
	ON_EN_CHANGE(IDC_Edit_Clip_Length, &CClipDlg::OnEnChangeEditClipLength)
	ON_EN_CHANGE(IDC_Edit_Pad_THK, &CClipDlg::OnEnChangeEditPadThk)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_Edit_Clip_THK, &CClipDlg::OnEnChangeEditClipThk)
	ON_EN_CHANGE(IDC_Edit_Length_B, &CClipDlg::OnEnChangeEditLengthB)
END_MESSAGE_MAP()

// 대화상자의 기본 계산 함수 설정
void CClipDlg::DlgCalculate() {
	UpdateData(TRUE);

	static int ErrorEgg = 0;
	MrDArray daInput(6);

	for (int i = 0; i < 6; i++) {
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

	if (m_ClipCalculator.Calculate(daInput, m_pdaOutput) == TRUE) {
		m_strResult4 = _T("SHELL 바깥쪽");
	}
	else {
		m_strResult4 = _T("SHELL 안쪽");
	}

	for (int i = 0; i < 3; i++) {
		*m_pdaOutput[i] = RoundOff(*m_pdaOutput[i], GetDigit());
	}

	UpdateData(FALSE);
}

// CLIP 길이 업데이트
void CClipDlg::UpdateClipLength() {
	UpdateData(TRUE);

	double dR, dW, dPT, dCP;
	dR = dW = dPT = dCP = 0;
	if (CheckAndAtof(m_strShellOR, &dR) &&
		CheckAndAtof(m_strWidth, &dW) &&
		CheckAndAtof(m_strPadThk, &dPT) &&
		CheckAndAtof(m_strClipProj, &dCP) )
	{
		double dTemp = m_ClipCalculator.CalcClipLength(dR, dW, dPT, dCP);
		FormatDouble(&m_strClipLength, dTemp, GetDigit());
		UpdateData(FALSE);
	}
}

// CClipDlg 메시지 처리기
	

// Shell O.D 입력
void CClipDlg::OnEnChangeEditShellOd()
{
	UpdateData(TRUE);
	double dTemp = 0, dTempL=0;
	if (CheckAndAtof(m_strShellOD, &dTemp)) {
		FormatDouble(&m_strShellOR, dTemp/2, GetDigit());

		if (CheckAndAtof(m_strClipProj, &dTempL)) {
			FormatDouble(&m_strClipLtD, dTempL - dTemp / 2, GetDigit());
		}

		UpdateData(FALSE);
		UpdateClipLength();
	}
	DlgCalculate();
}

// Shell O.R 입력
void CClipDlg::OnEnChangeEditShellOr()
{
	UpdateData(TRUE);
	double dTemp = 0, dTempL = 0;
	if (CheckAndAtof(m_strShellOR, &dTemp)) {
		FormatDouble(&m_strShellOD, dTemp * 2, GetDigit());

		if (CheckAndAtof(m_strClipProj, &dTempL)) {
			FormatDouble(&m_strClipLtD, dTempL - dTemp, GetDigit());
		}

		UpdateData(FALSE);
		UpdateClipLength();
	}
	DlgCalculate();
}

// Width*2 입력
void CClipDlg::OnEnChangeEditDblWidth()
{
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strDblWidth, &dTemp)) {
		FormatDouble(&m_strWidth, dTemp / 2, GetDigit());
		UpdateData(FALSE);
		UpdateClipLength();
	}
	DlgCalculate();
}

// Width 입력
void CClipDlg::OnEnChangeEditWidth()
{
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strWidth, &dTemp)) {
		FormatDouble(&m_strDblWidth, dTemp * 2, GetDigit());
		UpdateData(FALSE);
		UpdateClipLength();
	}
	DlgCalculate();
}

// Pad t 입력
void CClipDlg::OnEnChangeEditPadThk()
{
	UpdateClipLength();
	DlgCalculate();
}

// 두께/폭 클릭 
void CClipDlg::OnEnChangeEditClipThk()
{
	DlgCalculate();
}

// TO C.L 입력
void CClipDlg::OnEnChangeEditClipProj()
{
	UpdateData(TRUE);
	double dTemp1 = 0, dTemp2 = 0;
	if (CheckAndAtof(m_strClipProj, &dTemp1) && CheckAndAtof(m_strShellOR, &dTemp2)) {
		FormatDouble(&m_strClipLtD, dTemp1 - dTemp2, GetDigit());
		UpdateData(FALSE);
		UpdateClipLength();
	}
	DlgCalculate();
}

// O.D 입력
void CClipDlg::OnEnChangeEditClipLtD()
{
	UpdateData(TRUE);
	double dTemp1 = 0, dTemp2 = 0;
	if (CheckAndAtof(m_strClipLtD, &dTemp1) && CheckAndAtof(m_strShellOR, &dTemp2)) {
		FormatDouble(&m_strClipProj, dTemp1 + dTemp2, GetDigit());
		UpdateData(FALSE);
		UpdateClipLength();
	}
	DlgCalculate();
}

// CLIP LENGTH 입력
void CClipDlg::OnEnChangeEditClipLength()
{
	UpdateData(TRUE);
	double dTempR, dTempW, dTempPT, dTempL, dTempCP;
	dTempR = dTempW = dTempPT = dTempL = dTempCP = 0;
	if (CheckAndAtof(m_strShellOR, &dTempR) &&
		CheckAndAtof(m_strWidth, &dTempW) &&
		CheckAndAtof(m_strPadThk, &dTempPT) &&
		CheckAndAtof(m_strClipLength, &dTempL)) 
	{		
		dTempCP = dTempL + pitaGY(dTempR + dTempPT, dTempW);
		FormatDouble(&m_strClipProj, dTempCP, GetDigit());
		FormatDouble(&m_strClipLtD, dTempCP - dTempR, GetDigit());
		UpdateData(FALSE);
	}
	DlgCalculate();
}

// 길이(B) 입력
void CClipDlg::OnEnChangeEditLengthB()
{
	DlgCalculate();
}

// Enter/Esc키 무시하기
BOOL CClipDlg::PreTranslateMessage(MSG* pMsg)
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

// 파일 저장 관련 함수

void CClipDlg::UpdateDigit() {
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strShellOD, &dTemp)) {
		FormatDouble(&m_strShellOR, dTemp / 2, GetDigit());
		UpdateData(FALSE);
	}

	if (CheckAndAtof(m_strDblWidth, &dTemp)) {
		FormatDouble(&m_strWidth, dTemp / 2, GetDigit());
		UpdateData(FALSE);
	}

	double dTemp1 = 0, dTemp2 = 0;
	if (CheckAndAtof(m_strClipLtD, &dTemp1) && CheckAndAtof(m_strShellOR, &dTemp2)) {
		FormatDouble(&m_strClipProj, dTemp1 + dTemp2, GetDigit());
		UpdateData(FALSE);
	}

	UpdateClipLength();
}

// 파일 저장하기
BOOL CClipDlg::MakeFile(CString FileDir, const CString& FileName) {
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

	wfout << _T("CLIP") << endl;

	wfout << m_strShellOD.GetString() << _T(" | ") << m_strShellOR.GetString() << endl;
	wfout << m_strDblWidth.GetString() << _T(" | ") << m_strWidth.GetString() << endl;
	wfout << m_strPadThk.GetString() << _T(" | ") << m_strClipThk.GetString() << endl;
	wfout << m_strClipProj.GetString() << _T(" | ") << m_strClipLtD.GetString() << endl;
	wfout << m_strClipLength.GetString() << _T(" | ") << m_strLengthB.GetString();

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
BOOL CClipDlg::LoadFile(const CString& FileDir, const CString& FileName) {
	wifstream wfin(FileDir);
	CString strTmp = _T("");
	CString* strAll[] = {	&m_strShellOD, &m_strShellOR, &m_strDblWidth, &m_strWidth, &m_strPadThk, 
							&m_strClipThk, &m_strClipProj, &m_strClipLtD, &m_strClipLength, &m_strLengthB };
	wstring wstr;

	wfin >> wstr;
	strTmp.Format(_T("%s"), wstr.c_str());
	strTmp.Trim(_T("|"));
	if (strTmp != _T("CLIP")) {
		AfxMessageBox(_T("선택하신 텍스트 파일은 \"") + strTmp + _T("\" 전용입니다"));
		return FALSE;
	}

	int i = 0;
	while (i < 10) {
		if (wfin.eof()) break;
		wfin >> wstr;
		if (wstr == _T("|")) continue;
		strTmp.Format(_T("%s"), wstr.c_str());
		strTmp.Trim(_T("|"));

		if (strAll[i] != NULL) {
			*strAll[i] = strTmp;
		}
		++i;
	}

	UpdateData(FALSE);

	AfxMessageBox(_T("성공적으로 불러왔습니다."));
	DlgCalculate();
	return TRUE;
}

// 색 바꾸기
HBRUSH CClipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

