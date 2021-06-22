// COption1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "COption1Dlg.h"
#include "afxdialogex.h"


// COption1Dlg 대화 상자

IMPLEMENT_DYNAMIC(COption1Dlg, CDialogEx)

COption1Dlg::COption1Dlg(CWnd* pParent /*=nullptr*/ , CGeoGraphDlg* pTarget)
	: CDialogEx(IDD_DIALOG_OPTION1, pParent)
{
	m_pGeoGraphDlg = pTarget;
}

COption1Dlg::~COption1Dlg()
{
}

void COption1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_TEMP, m_Group_temp);
	DDX_Control(pDX, IDC_GROUP_PRCP, m_Group_prcp);
}


BEGIN_MESSAGE_MAP(COption1Dlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER_UPDATE_CLIMATE_INFO, &COption1Dlg::OnUpdateClimateInfo)
	ON_MESSAGE(WM_USER_CLIMATE_RESET, &COption1Dlg::OnResetInfo)
END_MESSAGE_MAP()

void COption1Dlg::updateToTarget() {
	wstringstream wss;

	for (int i = 0; i < 12; i++) {
		CString temp_str, precip_str;
		m_edits_temp[i].GetWindowTextW(temp_str);
		m_edits_prcp[i].GetWindowTextW(precip_str);

		wss << temp_str.GetString() << _T(" ") << precip_str.GetString() << endl;

		double temp, precip;
		wss >> temp >> precip;

		m_pGeoGraphDlg->getTemperGraph()->setData(temp, i);
		m_pGeoGraphDlg->getPrecipGraph()->setData(precip, i);
	}
}


void COption1Dlg::updateFromTarget() {
	wstringstream wss;
	wss << std::setprecision(1) << std::fixed;

	for (int i = 0; i < 12; i++) {
		wstring temp_wstr, precip_wstr;

		wss << m_pGeoGraphDlg->getTemperGraph()->getVector()->at(i) << std::endl;
		wss << m_pGeoGraphDlg->getPrecipGraph()->getVector()->at(i) << std::endl;
		wss >> temp_wstr;
		wss >> precip_wstr;

		m_edits_temp[i].SetWindowText(temp_wstr.c_str());
		m_edits_prcp[i].SetWindowText(precip_wstr.c_str());
	}
}

// COption1Dlg 메시지 처리기


BOOL COption1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFontIndirectW(&lf);
	setFontSize(m_font, 18 * (rect.Height() / 470.0));

	CString str;

	for (int i = 0; i < 12; i++) {
		m_statics_temp[i].Create(_T("STATIC"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, i + 0);
		m_statics_prcp[i].Create(_T("STATIC"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, i + 20);
		m_edits_temp[i].Create(WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 100, 100), this, i + 40);
		m_edits_prcp[i].Create(WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 100, 100), this, i + 60);

		str.Format(_T("%d월"), i+1);
		m_statics_temp[i].SetWindowTextW(str);
		m_statics_prcp[i].SetWindowTextW(str);

		m_statics_temp[i].SetFont(&m_font);
		m_statics_prcp[i].SetFont(&m_font);
		m_edits_temp[i].SetFont(&m_font);
		m_edits_prcp[i].SetFont(&m_font);
	}

	updateFromTarget();

	PostMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COption1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);
	
	int width = Rect.Width();
	int height = Rect.Height();
	int gap = 20 * (Rect.Height() / 470.0);

	CRect rect_group_temp;
	CRect rect_group_prcp;

	rect_group_temp = MarginHelper.Initiate().SetMargin(gap)
						.OnLeft(0)
						.OnUp(0)
						.OnRight(width)
						.OnDown(Rect.CenterPoint().y, gap/2)
						.toRect();

	rect_group_prcp = MarginHelper.Initiate().SetMargin(gap)
						.OnLeft(0)
						.OnUp(Rect.CenterPoint().y, gap/2)
						.OnRight(width)
						.OnDown(height)
						.toRect();

	int ctrl_gap = 20 * (Rect.Height() / 470.0);
	int static_width = 40 * (Rect.Height() / 470.0);
	int edit_width = 80 * (Rect.Height() / 470.0);
	int ctrl_height = 20 * (Rect.Height() / 470.0);

	if (m_Group_temp.GetSafeHwnd() != NULL) {
		m_Group_temp.MoveWindow(rect_group_temp);
		m_Group_prcp.MoveWindow(rect_group_prcp);

		CRect _static_rect = MarginHelper.Initiate()
							.SetWidth(static_width)
							.SetHeight(ctrl_height)
							.toRect();

		CRect _edit_rect = MarginHelper.Initiate()
							.SetWidth(edit_width)
							.SetHeight(ctrl_height)
							.toRect();

		rect_group_temp.DeflateRect(10 * (Rect.Height() / 470.0), 10 * (Rect.Height() / 470.0));
		rect_group_prcp.DeflateRect(10 * (Rect.Height() / 470.0), 10 * (Rect.Height() / 470.0));

		int interval_height = rect_group_temp.Height() / 6.0;
		int gap_between_groups = rect_group_prcp.top - rect_group_temp.top;

		CRect static_rect, edit_rect;
		for (int i = 0; i < 6; i++) {
			static_rect = MarginHelper.Initiate()
							.SetRect(_static_rect)
							.OnCenterLine(interval_height*(i+0.5) + rect_group_temp.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
							.AttachLeft(rect_group_temp.CenterPoint().x, ctrl_gap)
							.toRect();

			edit_rect = MarginHelper.Initiate()
							.SetRect(_edit_rect)
							.OnCenterLine(interval_height * (i + 0.5) + rect_group_temp.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
							.AttachLeft(static_rect.right)
							.toRect();

			m_statics_temp[i + 6].MoveWindow(static_rect);
			m_edits_temp[i+ 6].MoveWindow(edit_rect);

			static_rect.OffsetRect(0, gap_between_groups);
			edit_rect.OffsetRect(0, gap_between_groups);

			m_statics_prcp[i + 6].MoveWindow(static_rect);
			m_edits_prcp[i + 6].MoveWindow(edit_rect);

			edit_rect = MarginHelper.Initiate()
				.SetRect(_edit_rect)
				.OnCenterLine(interval_height * (i + 0.5) + rect_group_temp.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachRight(rect_group_temp.CenterPoint().x, ctrl_gap)
				.toRect();

			static_rect = MarginHelper.Initiate()
				.SetRect(_static_rect)
				.OnCenterLine(interval_height * (i + 0.5) + rect_group_temp.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachRight(edit_rect.left)
				.toRect();

			m_statics_temp[i].MoveWindow(static_rect);
			m_edits_temp[i].MoveWindow(edit_rect);

			static_rect.OffsetRect(0, gap_between_groups);
			edit_rect.OffsetRect(0, gap_between_groups);

			m_statics_prcp[i].MoveWindow(static_rect);
			m_edits_prcp[i].MoveWindow(edit_rect);
		}

		
	}
}


BOOL COption1Dlg::OnEraseBkgnd(CDC* pDC)
{
	CRect Rect;
	GetClientRect(&Rect);
	pDC->Rectangle(&Rect);

	return FALSE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

afx_msg LRESULT COption1Dlg::OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam) {
	if (wParam == TRUE) {
		updateFromTarget();
	}
	else {
		updateToTarget();
	}
	return 0;
}

afx_msg LRESULT COption1Dlg::OnResetInfo(WPARAM wParam, LPARAM lParam) {
	auto tempers_Changwon = { 2.8, 4.7, 8.7, 14.1, 18.4, 21.7, 25.2, 26.5, 22.8, 17.4, 11.0 ,5.4 };
	auto precips_Changwon = { 36.9, 45.3, 74.3, 129.9, 142.2, 232.3, 293.8, 299.0, 167.3, 50.2, 52.2, 22.0 };

	auto pvec_t = m_pGeoGraphDlg->getTemperGraph()->getVector();
	auto pvec_p = m_pGeoGraphDlg->getPrecipGraph()->getVector();

	std::copy(tempers_Changwon.begin(), tempers_Changwon.end(), pvec_t->begin());
	std::copy(precips_Changwon.begin(), precips_Changwon.end(), pvec_p->begin());

	updateFromTarget();

	return 0;
}

BOOL COption1Dlg::PreTranslateMessage(MSG* pMsg)
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
