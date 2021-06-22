
// CMainDialog.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Calc Ckeck.h"
#include "CMainDialog.h"
#include "afxdialogex.h"

#include "OptionDialog.h"
#include "HelpDialog.h"
#include "SecureWarningDlg.h"
#include "ImageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CMainDialog 대화 상자


CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CBaseCalcDialog(IDD_Main_Dialog, pParent)
	, m_iTempTabSel(0)
	, m_strPassword(_T("3377"))
	, m_dtAfter2years(2022,1,1,0,0,0)
{
	m_pwndShow = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


// CMainDialog 메시지 처리기

BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	if (COleDateTime::GetCurrentTime() > m_dtAfter2years) {
		SecureWarningDlg SecureDlg;
		if (SecureDlg.DoModal() == IDOK) {
			if (SecureDlg.GetPassword() == m_strPassword) {
				AfxMessageBox(_T("비밀번호 확인 완료"));
			}
			else {
				AfxMessageBox(_T("비밀번호가 틀렸습니다."));
				OnOK();
			}
		}
	}

	static CImageList ImgList;

	m_MainTab.SetImageList(&ImgList);

	m_MainTab.InsertItem(0, _T("Nozzle"), 0);
	m_MainTab.InsertItem(1, _T("Clip"), 1);

	CRect rcClient, rcWindow;
	m_MainTab.GetClientRect(&rcClient);
	m_MainTab.AdjustRect(FALSE, &rcClient);
	m_MainTab.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	rcClient.OffsetRect(rcWindow.left, rcWindow.top);
	rcClient.left -= 2;
	rcClient.bottom += 1;


	m_iDigit = AfxGetApp()->GetProfileIntW(_T("Option"), _T("Digit"), 1);
	m_strFileDir = AfxGetApp()->GetProfileStringW(_T("Option"), _T("FileDir"), _T("C:\\Users\\User\\Documents"));
	CreateDirectory(m_strFileDir+_T("\\Calc Save"), NULL);
	CreateDirectory(m_strFileDir + _T("\\Calc Save\\Nozzle"), NULL);
	CreateDirectory(m_strFileDir + _T("\\Calc Save\\Clip"), NULL);

	m_NozzleDlg.Create(IDD_Nozzle_Length_Ckeck, m_MainTab.GetWindow(IDD_Nozzle_Length_Ckeck), m_iDigit);
	m_NozzleDlg.MoveWindow(&rcClient);
	m_NozzleDlg.ShowWindow(SW_SHOW);

	m_ClipDlg.Create(IDD_Clip_Length_Ckeck, m_MainTab.GetWindow(IDD_Clip_Length_Ckeck), m_iDigit);
	m_ClipDlg.MoveWindow(&rcClient);
	m_ClipDlg.ShowWindow(SW_HIDE);

	m_pwndShow = &m_NozzleDlg;

	//int i = AfxGetApp()->GetProfileIntW(_T("Option"), _T("TabSel"), 0);
	m_MainTab.SetCurSel(0);
	OnTcnSelchangeTabMain(NULL, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMainDialog::OnDestroy()
{
	m_iTempTabSel = m_MainTab.GetCurSel();
	CBaseCalcDialog::OnDestroy();
}

void CMainDialog::PostNcDestroy()
{
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("Digit"), m_iDigit);
	AfxGetApp()->WriteProfileStringW(_T("Option"), _T("FileDir"), m_strFileDir);
	//AfxGetApp()->WriteProfileInt(_T("Option"), _T("TabSel"), m_iTempTabSel);

	CDialogEx::PostNcDestroy();
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Tab_Main, m_MainTab);
	DDX_Control(pDX, IDC_Combo_Img, m_ImgCombo);
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_Tab_Main, &CMainDialog::OnTcnSelchangeTabMain)
	ON_COMMAND(ID_Menu_FileLoad, &CMainDialog::OnMenuFileload)
	ON_COMMAND(ID_Menu_FileSave, &CMainDialog::OnMenuFilesave)
	ON_COMMAND(ID_Menu_Help, &CMainDialog::OnMenuHelp)
	ON_COMMAND(ID_Menu_Option, &CMainDialog::OnMenuOption)
	ON_BN_CLICKED(IDC_Button_SetImage, &CMainDialog::OnBnClickedButtonSetimage)
	ON_BN_CLICKED(IDOK, &CMainDialog::OnBnClickedOk)
	ON_WM_DESTROY()
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
		CDialogEx::OnPaint();
	}
}
HCURSOR CMainDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString CMainDialog::GetFileDir() {
	return m_strFileDir;
}

void CMainDialog::SetFileDir(const CString& FileDir) {
	m_strFileDir = FileDir;
}

void CMainDialog::OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_pwndShow != NULL) {
		m_pwndShow->ShowWindow(SW_HIDE);
		m_pwndShow = NULL;
	}

	CString strTmp = _T("");
	int nIndex = m_MainTab.GetCurSel();

	switch (nIndex) {
	case 0:
		m_NozzleDlg.ShowWindow(SW_SHOW);
		m_pwndShow = &m_NozzleDlg;
		m_ImgCombo.ResetContent();
		for (int i = 0; i < 4; i++) {
			strTmp.LoadStringW(IDS_IMG_NZ1 + i);
			m_ImgCombo.AddString(strTmp);
		}
		m_pwndShow->DlgCalculate();
		break;
	case 1:
		m_ClipDlg.ShowWindow(SW_SHOW);
		m_pwndShow = &m_ClipDlg;
		m_ImgCombo.ResetContent();
		for (int i = 0; i < 5; i++) {
			strTmp.LoadStringW(IDS_IMG_CLP1 + i);
			m_ImgCombo.AddString(strTmp);
		}
		break;
	}

	m_ImgCombo.SetCurSel(0);
	
	if (pResult != NULL)
		*pResult = 0;
}

void CMainDialog::OnMenuFilesave()
{
	int nIndex = m_MainTab.GetCurSel();
	CString strTmp1, strTmp2;
	strTmp1.LoadStringW(IDS_DIR_NOZZLE + nIndex);
	strTmp2.LoadStringW(IDS_FNS_NOZZLE + nIndex);

	CFileDialog fd(FALSE, _T("txt"), m_strFileDir + _T("\\Calc Save\\") + strTmp1 + strTmp2, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("텍스트 파일(*.txt)|*.txt||"), this);

	if (fd.DoModal() == IDOK) {
		m_pwndShow->MakeFile(fd.GetPathName(), fd.GetFileName());
	}
}

void CMainDialog::OnMenuFileload()
{
	int nIndex = m_MainTab.GetCurSel();
	CString strTmp;
	strTmp.LoadStringW(IDS_DIR_NOZZLE + nIndex);

	CFileDialog fd(TRUE, _T("txt"), m_strFileDir + _T("\\Calc Save\\") + strTmp, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("텍스트 파일(*txt)|*.txt||"), this);

	if (fd.DoModal() == IDOK) {
		m_pwndShow->LoadFile(fd.GetPathName(), fd.GetFileName());
	}
}

void CMainDialog::OnMenuOption()
{
	UpdateData(TRUE);

	OptionDialog OptionDlg(this);

	OptionDlg.DoModal();

	m_NozzleDlg.SetDigit(m_iDigit);
	m_ClipDlg.SetDigit(m_iDigit);

	m_NozzleDlg.UpdateDigit();
	m_ClipDlg.UpdateDigit();
}

void CMainDialog::OnMenuHelp()
{
	static HelpDialog HelpDlg(this);

	if (HelpDlg.GetSafeHwnd() == NULL) {
		HelpDlg.Create(IDD_Dialog_Help, this);
	}
	else {
		HelpDlg.SetCurFileDir(m_strFileDir);
	}

	HelpDlg.ShowWindow(SW_SHOW);
}

void CMainDialog::OnBnClickedButtonSetimage()
{
	static ImageDialog ImgDlg[9];

	int n = m_ImgCombo.GetCurSel() + 4*(m_MainTab.GetCurSel()==1);

	if (ImgDlg[n].GetSafeHwnd() == NULL) {
		CString strTmp = _T("");
		ImgDlg[n].Create(IDD_Dialog_Image, this);
		strTmp.LoadStringW(IDS_IMG_NZ1 + n);
		ImgDlg[n].SetImageResource(IDR_IMG_NZ1 + n);
		ImgDlg[n].SetWindowTextW(strTmp);
	}

	CRect DskRct, prRct, chdRct;
	GetWindowRect(&prRct);

	GetDesktopWindow()->GetWindowRect(&DskRct);
	if((prRct.left+prRct.right)/2 < (DskRct.left + DskRct.right)/2 + 200)
		prRct += CPoint(200, 0);


	chdRct.left = prRct.left - 30 - ImgDlg[n].GetBitmap()->m_pBitmap->GetWidth();
	chdRct.top = prRct.top;
	chdRct.right = prRct.left - 10;
	chdRct.bottom = prRct.top + 43 + ImgDlg[n].GetBitmap()->m_pBitmap->GetHeight();

	MoveWindow(prRct);
	ImgDlg[n].MoveWindow(chdRct);
	ImgDlg[n].ShowWindow(SW_SHOW);
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
			m_pwndShow->DlgCalculate();
			return true;
		}
	}


	return CBaseCalcDialog::PreTranslateMessage(pMsg);
}


void CMainDialog::OnBnClickedOk()
{
	/*if (AfxMessageBox(_T("프로그램을 종료하시겠습니까?"), MB_YESNO) == IDYES) {
		OnOK();
	}*/

	OnOK();
}
