// CKoppenOptionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "CKoppenOptionDlg.h"
#include "afxdialogex.h"

#include "COption1Dlg.h"
#include "COption2Dlg.h"
#include "COption3Dlg.h"
#include "CMainDlg.h"


// CKoppenOptionDlg 대화 상자

IMPLEMENT_DYNAMIC(CKoppenOptionDlg, CDialogEx)

CKoppenOptionDlg::CKoppenOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPTIONS, pParent)
{
	auto pMainDlg = reinterpret_cast<CMainDlg*>(pParent);
	m_now_index = 0;
	m_OptionDlgs[0] = new COption1Dlg(this, pMainDlg->getGeoGraphDlg());
	m_OptionDlgs[1] = new COption2Dlg(this, pMainDlg->getGeoGraphDlg());
	m_OptionDlgs[2] = new COption3Dlg(this, pMainDlg->getGeoGraphDlg(), pMainDlg->getKoppenIconWnd());
}

CKoppenOptionDlg::~CKoppenOptionDlg()
{
	for (int i = 0; i < 3; i++) {
		delete m_OptionDlgs[i];
	}
}

void CKoppenOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OPTIONS, m_list_options);
	DDX_Control(pDX, ID_BUTTON_RESET, m_button_reset);
	DDX_Control(pDX, ID_BUTTON_APPLY, m_button_apply);
	DDX_Control(pDX, IDOK, m_button_ok);
	DDX_Control(pDX, IDCANCEL, m_button_cancel);
}


BEGIN_MESSAGE_MAP(CKoppenOptionDlg, CDialogEx)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST_OPTIONS, &CKoppenOptionDlg::OnLbnSelchangeListOptions)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_BUTTON_APPLY, &CKoppenOptionDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, &CKoppenOptionDlg::OnBnClickedOk)
	ON_MESSAGE(WM_USER_UPDATE_CLIMATE_INFO, &CKoppenOptionDlg::OnUpdateClimateInfo)
	ON_BN_CLICKED(ID_BUTTON_RESET, &CKoppenOptionDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CKoppenOptionDlg 메시지 처리기


BOOL CKoppenOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list_options.InsertString(0, _T("- 세부 입력"));
	m_list_options.InsertString(1, _T("- 눈금 설정"));
	m_list_options.InsertString(2, _T("- 색상 설정"));

	m_OptionDlgs[0]->Create(IDD_DIALOG_OPTION1, this);
	m_OptionDlgs[1]->Create(IDD_DIALOG_OPTION2, this);
	m_OptionDlgs[2]->Create(IDD_DIALOG_OPTION3, this);

	m_now_index = 0;
	m_OptionDlgs[m_now_index]->ShowWindow(SW_SHOW);

	PostMessage(WM_SIZE);
	return TRUE;           
}


void CKoppenOptionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);

	int width = Rect.Width();
	int height = Rect.Height();
	int gap = width * 0.03;

	int button_width = width * 0.11;
	int button_height = width * 0.035;
	int button_gap = width * 0.015;

	CRect rect_list, rect_options;
	CRect rect_button_reset, rect_button_apply, rect_button_ok, rect_button_cancel;
	CRect rect_button = { 0,0,button_width, button_height };

	rect_list = MarginHelper.Initiate().SetMargin(gap)
				.OnUp(0)
				.OnLeft(0)
				.OnDown(height, button_height + button_gap *2)
				.OnRight(getCenterLine(), gap / 2)
				.toRect();

	rect_options = MarginHelper.Initiate().SetMargin(gap)
					.OnUp(0)
					.OnRight(width)
					.OnDown(height, button_height + button_gap*2)
					.OnLeft(getCenterLine(), gap / 2)
					.toRect();

	//MrFormatMessage(_T("%d %d"), rect_options.Width(), rect_options.Height());
	//459, 470

	rect_button_reset = MarginHelper.Initiate().SetRect(rect_button).SetMargin(button_gap)
							.AttachLeft(0, gap)
							.AttachDown(height)
							.toRect();
	rect_button_cancel = MarginHelper.Initiate().SetRect(rect_button).SetMargin(button_gap)
							.AttachRight(width, gap)
							.AttachDown(height)
							.toRect();
	rect_button_ok = MarginHelper.Initiate().SetRect(rect_button).SetMargin(button_gap)
							.AttachRight(rect_button_cancel.left)
							.AttachDown(height)
							.toRect();
	rect_button_apply = MarginHelper.Initiate().SetRect(rect_button).SetMargin(button_gap)
							.AttachRight(rect_button_ok.left)
							.AttachDown(height)
							.toRect();

	if (m_list_options.GetSafeHwnd() != NULL) {
		m_list_options.MoveWindow(&rect_list);
		for (int i = 0; i < 3; i++) {
			m_OptionDlgs[i]->MoveWindow(rect_options);
		}

		m_button_reset.MoveWindow(rect_button_reset);
		m_button_apply.MoveWindow(rect_button_apply);
		m_button_ok.MoveWindow(rect_button_ok);
		m_button_cancel.MoveWindow(rect_button_cancel);
	}

}


void CKoppenOptionDlg::OnLbnSelchangeListOptions()
{
	m_OptionDlgs[m_now_index]->ShowWindow(SW_HIDE);
	m_now_index = m_list_options.GetCurSel();
	m_OptionDlgs[m_now_index]->ShowWindow(SW_SHOW);
}


void CKoppenOptionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow) {
		m_OptionDlgs[m_now_index]->ShowWindow(SW_HIDE);
		m_list_options.SetCurSel(0);
		m_now_index = 0;
		m_OptionDlgs[m_now_index]->ShowWindow(SW_SHOW);
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CKoppenOptionDlg::OnBnClickedButtonApply()
{
	for (int i = 0; i < 3; i++) {
		m_OptionDlgs[i]->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO, FALSE);
	}
	GetParent()->Invalidate();
	GetParent()->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO);
}

void CKoppenOptionDlg::OnBnClickedOk()
{
	for (int i = 0; i < 3; i++) {
		m_OptionDlgs[i]->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO, FALSE);
	}
	GetParent()->Invalidate();
	GetParent()->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO);

	CDialogEx::OnOK();
}

void CKoppenOptionDlg::OnBnClickedButtonReset()
{
	for (int i = 0; i < 3; i++) {
		m_OptionDlgs[i]->PostMessageW(WM_USER_CLIMATE_RESET, TRUE);
	}
	GetParent()->Invalidate();
	GetParent()->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO);
}

afx_msg LRESULT CKoppenOptionDlg::OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam) {
	for (int i = 0; i < 3; i++) {
		m_OptionDlgs[i]->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO, TRUE);
	}
	return 0;
}

BOOL CKoppenOptionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return false; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			return false;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


