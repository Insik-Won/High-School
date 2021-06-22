// CMainDialog.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Weight Ckeck.h"
#include "CMainDialog.h"
#include "afxdialogex.h"

#include "ImageDialog.h"
#include "OptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace MrDoubleFormat;

const UINT ImageID[3] = { IDR_SteelCopper, IDR_METAL, IDR_ELEMENT };

// CMainDialog 대화 상자

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CBasicCalcDlg(IDD_Main_Dialog, pParent, 4, 7.85)
	, m_strSpGravity(_T("7.85"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pwndShow = NULL;
}

BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	static CImageList ImgList;
	m_MainTab.SetImageList(&ImgList);

	m_MainTab.InsertItem(0, _T("형상"), 0);
	m_MainTab.InsertItem(1, _T("형강 규격"), 1);

	CRect rcClient, rcWindow;
	m_MainTab.GetClientRect(&rcClient);
	m_MainTab.AdjustRect(FALSE, &rcClient);
	m_MainTab.GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	rcClient.OffsetRect(rcWindow.left, rcWindow.top);
	rcClient.left += 0;
	rcClient.right += 3;
	rcClient.top += 2;
	rcClient.bottom += 4;

	m_FigureDlg.Create(IDD_Dialog_Figure, m_MainTab.GetWindow(IDD_Dialog_Figure));
	m_FigureDlg.MoveWindow(&rcClient);
	m_FigureDlg.ShowWindow(SW_SHOW);

	m_StandardDlg.Create(IDD_Dialog_Standard, m_MainTab.GetWindow(IDD_Dialog_Standard));
	m_StandardDlg.MoveWindow(&rcClient);
	m_StandardDlg.ShowWindow(SW_HIDE);

	m_pwndShow = &m_FigureDlg;

	((CComboBox*)GetDlgItem(IDC_Combo_Img))->SetCurSel(0);
	
	int digit = AfxGetApp()->GetProfileIntW(_T("Option"), _T("Digit"), 1);
	SetDigit(digit);

	GetSgDataFromFIle(&m_ComboSpGravity, _T("비중"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CMainDialog::PostNcDestroy()
{
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("Digit"), GetDigit());
	CBasicCalcDlg::PostNcDestroy();
}


void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Tab_Main, m_MainTab);
	DDX_Text(pDX, IDC_Edit_SpGravity, m_strSpGravity);
	DDX_Control(pDX, IDC_Combo_SpGrvty, m_ComboSpGravity);
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_Edit_SpGravity, &CMainDialog::OnEnChangeEditSpgravity)
	ON_NOTIFY(TCN_SELCHANGE, IDC_Tab_Main, &CMainDialog::OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDC_Button_ImgOn, &CMainDialog::OnBnClickedButtonImgon)
	ON_COMMAND(ID_MnItem_Help, &CMainDialog::OnMnitemHelp)
	ON_COMMAND(ID_MnItem_Option, &CMainDialog::OnMnitemOption)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_Combo_SpGrvty, &CMainDialog::OnCbnSelchangeComboSpgrvty)
	ON_CBN_EDITCHANGE(IDC_Combo_SpGrvty, &CMainDialog::OnCbnEditchangeComboSpgrvty)
END_MESSAGE_MAP()


void CMainDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		CFont font;
		CFont* pOldFont;
		
		LOGFONT lf;
		::ZeroMemory(&lf, sizeof(lf));
		wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 30;
		lf.lfWidth = 14;

		font.CreateFontIndirect(&lf);

		dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
		dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));

		pOldFont = dc.SelectObject(&font);
		dc.TextOutW(245, 15, TEXT("WEIGHT"));
		dc.SelectObject(pOldFont);

		CDialogEx::OnPaint();
	}
}
HCURSOR CMainDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CMainDialog 메시지 처리기

void CMainDialog::OnEnChangeEditSpgravity()
{
	UpdateData(TRUE);
	double dTemp = 0;
	if (CheckAndAtof(m_strSpGravity, &dTemp, TRUE)) {
		SetSpGravity(dTemp);
	}
	m_FigureDlg.PostMessageW(WM_FCALCRESULT, 0, 0);
}

void CMainDialog::SetDigit(int iDigit) {
	CBasicCalcDlg::SetDigit(iDigit);
	m_FigureDlg.SetDigit(iDigit);
	m_StandardDlg.SetDigit(iDigit);
}

void CMainDialog::SetSpGravity(double dSpGravity) {
	CBasicCalcDlg::SetSpGravity(dSpGravity);
	m_FigureDlg.SetSpGravity(dSpGravity);
	m_StandardDlg.SetSpGravity(dSpGravity);
}


void CMainDialog::OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_pwndShow != NULL) {
		m_pwndShow->ShowWindow(SW_HIDE);
		m_pwndShow = NULL;
	}

	int nIndex = m_MainTab.GetCurSel();

	switch (nIndex) {
	case 0:
		m_FigureDlg.ShowWindow(SW_SHOW);
		m_pwndShow = &m_FigureDlg;
		break;
	case 1:
		m_StandardDlg.ShowWindow(SW_SHOW);
		m_pwndShow = &m_StandardDlg;
		break;
	}

	*pResult = 0;
}


void CMainDialog::OnBnClickedButtonImgon()
{
	static ImageDialog ImgDlg[3];

	int n = ((CComboBox*)GetDlgItem(IDC_Combo_Img))->GetCurSel();

	if (ImgDlg[n].GetSafeHwnd() == NULL) {
		CString strTmp = _T("");
		strTmp.LoadStringW(IDS_JPG_SteelCopper + n);
		ImgDlg[n].Create(IDD_Dialog_Image, this);
		ImgDlg[n].SetImageResource(ImageID[n]);
		ImgDlg[n].SetWindowTextW(strTmp);
	}

	CRect DskRct, prRct, chdRct;
	GetWindowRect(&prRct);

	GetDesktopWindow()->GetWindowRect(&DskRct);
	if ((prRct.left + prRct.right) / 2 < (DskRct.left + DskRct.right) / 2 + 300)
		prRct += CPoint(300, 0);

	chdRct.left = prRct.left - 30 - ImgDlg[n].GetBitmap()->m_pBitmap->GetWidth();
	chdRct.top = prRct.top;
	chdRct.right = prRct.left - 10;
	chdRct.bottom = prRct.top + 50 + ImgDlg[n].GetBitmap()->m_pBitmap->GetHeight();

	MoveWindow(prRct);
	ImgDlg[n].MoveWindow(chdRct);
	ImgDlg[n].ShowWindow(SW_SHOW);
}

// 비중 데이터에서 비중 뽑아오기
void CMainDialog::GetSgDataFromFIle(CComboBox* pCombo, const CString& FileName) {
	wifstream wfin(_T("Data\\") + FileName + _T(".txt"));
	CString strTmp = _T("");
	wstring wstr;
	double dTemp = 0;

	while (!wfin.eof()) {
		wfin >> wstr;
		strTmp.Format(_T("%s"), wstr.c_str());
		strTmp.Trim();
		strTmp.Replace('_', ' ');

		wfin >> wstr;
		if (wstr.compare(_T("0"))) {
			strTmp.AppendFormat(_T("  %s"), wstr.c_str());
		}
		pCombo->AddString(strTmp);
	}

	pCombo->SetCurSel(0);
}

// 비중 처리하기
void CMainDialog::OnCbnSelchangeComboSpgrvty()
{
	UpdateData(TRUE);
	CString strTmp = _T("");
	double dTemp = 0;

	int nIndex = m_ComboSpGravity.GetCurSel();
	m_ComboSpGravity.GetLBText(nIndex, strTmp);
	int n = strTmp.ReverseFind(' ') + 1;
	strTmp = strTmp.Right(strTmp.GetLength()-n);

	if (CheckAndAtof(strTmp, &dTemp, TRUE)) {
		SetSpGravity(dTemp);
	}

	strTmp.Trim('-');
	if (strTmp == _T("")) {
		strTmp = _T("0");
	}

	m_strSpGravity = strTmp;
	UpdateData(FALSE);
}


void CMainDialog::OnCbnEditchangeComboSpgrvty()
{
	UpdateData(TRUE);
	CString strTmp = _T("");
	double dTemp = 0;

	m_ComboSpGravity.GetWindowTextW(strTmp);

	if (CheckAndAtof(strTmp, &dTemp, TRUE)) {
		SetSpGravity(dTemp);
	}
}


void CMainDialog::OnMnitemOption()
{
	UpdateData(TRUE);

	OptionDialog OptionDlg(this);

	OptionDlg.DoModal();
}


void CMainDialog::OnMnitemHelp()
{
	static CDialogEx HelpDlg;

	if (HelpDlg.GetSafeHwnd() == NULL) {
		HelpDlg.Create(IDD_Dialog_Help, this);
	}

	CRect pntRct, chdRct;
	int cx, cy;
	GetWindowRect(pntRct);
	HelpDlg.GetWindowRect(chdRct);

	cx = (pntRct.left + pntRct.right) / 2 - chdRct.Width()/2 ;
	cy = (pntRct.top + pntRct.bottom) / 2 - chdRct.Height()/2 ;

	chdRct.right = cx + chdRct.Width();
	chdRct.bottom = cy + chdRct.Height();
	chdRct.left = cx;
	chdRct.top = cy;

	HelpDlg.MoveWindow(chdRct);
	HelpDlg.ShowWindow(SW_SHOW);
}


BOOL CMainDialog::PreTranslateMessage(MSG* pMsg)
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

	return CBasicCalcDlg::PreTranslateMessage(pMsg);
}


HBRUSH CMainDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBasicCalcDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) {
	case CTLCOLOR_EDIT:
		if (pWnd->GetDlgCtrlID() == IDC_Edit_SpGravity)
		{
			pDC->SetTextColor(RGB(0, 51, 204));
			return (HBRUSH)(m_pEditBrush->GetSafeHandle());
		}
	default:
		return hbr;
	}
}
