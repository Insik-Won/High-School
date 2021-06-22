// COption2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "COption2Dlg.h"
#include "afxdialogex.h"


// COption2Dlg 대화 상자

IMPLEMENT_DYNAMIC(COption2Dlg, CDialogEx)

COption2Dlg::COption2Dlg(CWnd* pParent /*=nullptr*/, CGeoGraphDlg* pTarget)
	: CDialogEx(IDD_DIALOG_OPTION2, pParent)
{
	m_pGeoGraphDlg = pTarget;
}

COption2Dlg::~COption2Dlg()
{
}

void COption2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_HELP, m_group_help);
	DDX_Control(pDX, IDC_GROUP_TEMP, m_group_temp);
	DDX_Control(pDX, IDC_GROUP_PRECIP, m_group_prcp);
}


BEGIN_MESSAGE_MAP(COption2Dlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER_UPDATE_CLIMATE_INFO, &COption2Dlg::OnUpdateClimateInfo)
	ON_MESSAGE(WM_USER_CLIMATE_RESET, &COption2Dlg::OnResetInfo)
END_MESSAGE_MAP()

void COption2Dlg::updateToTarget() {
	wstringstream wss;
	for (int i = 0; i < 6; i++) {
		CString str;
		m_edits[i].GetWindowTextW(str);
		wss << str.GetString() << _T(' ');
	}

	int temp_upper, temp_lower, temp_unit;
	int prcp_upper, prcp_lower, prcp_unit;

	wss >> temp_upper >> temp_lower >> temp_unit;
	wss >> prcp_upper >> prcp_lower >> prcp_unit;

	m_pGeoGraphDlg->getTemperGraph()->setGraphLimits(temp_lower, temp_upper);
	m_pGeoGraphDlg->getPrecipGraph()->setGraphLimits(prcp_lower, prcp_upper);

	m_pGeoGraphDlg->getGraphMarker()->removeMark(MrDirection::Left, MrInOut::Outer);
	m_pGeoGraphDlg->getGraphMarker()->removeMark(MrDirection::Right, MrInOut::Outer);
	m_pGeoGraphDlg->getGraphMarker()->addSuccessiveMark(temp_lower, temp_upper, temp_unit, MrDirection::Left, MrInOut::Outer);
	m_pGeoGraphDlg->getGraphMarker()->addSuccessiveMark(prcp_lower, prcp_upper, prcp_unit, MrDirection::Right, MrInOut::Outer);
	m_pGeoGraphDlg->getGraphMarker()->addMark(1.05, _T("(℃)"), MrDirection::Left, MrInOut::Outer);
	m_pGeoGraphDlg->getGraphMarker()->addMark(1.05, _T("(mm)"), MrDirection::Right, MrInOut::Outer);

	m_pGeoGraphDlg->getGraphMarker()->arrange();
}

void COption2Dlg::updateFromTarget() {
	wstringstream wss;
	auto [temp_lower, temp_upper] = m_pGeoGraphDlg->getTemperGraph()->getGraphLimits();
	auto [prcp_lower, prcp_upper] = m_pGeoGraphDlg->getPrecipGraph()->getGraphLimits();

	wss << temp_upper << _T(' ') << temp_lower << _T(' ') << prcp_upper << _T(' ') << prcp_lower << endl;

	wstring buffers[4];

	std::copy(std::istream_iterator<wstring, wchar_t>(wss)
		, std::istream_iterator<wstring, wchar_t>()
		, begin(buffers));

	int i = 0;
	for (auto e : { 0, 1, 3, 4 }) {
		m_edits[e].SetWindowTextW(buffers[i++].c_str());
	}

	auto pVector = m_pGeoGraphDlg->getGraphMarker()->getVector();

	auto temp_finder = [](auto tuple) {
		auto [inOut, direction, ratio, string] = tuple;
		return inOut == MrInOut::Outer && direction == MrDirection::Left;
	};

	auto precip_finder = [](auto tuple) {
		auto [inOut, direction, ratio, string] = tuple;
		return inOut == MrInOut::Outer && direction == MrDirection::Right;
	}; 

	auto temp_beg = std::find_if(pVector->begin(), pVector->end(), temp_finder);
	auto temp_end = std::find_if(pVector->rbegin(), pVector->rend(), temp_finder).base();
	auto prcp_beg = std::find_if(pVector->begin(), pVector->end(), precip_finder);
	auto prcp_end = std::find_if(pVector->rbegin(), pVector->rend(), precip_finder).base();

	temp_end--, prcp_end--;

	double temp_unit = (temp_upper - temp_lower) / std::distance(temp_beg, --temp_end);
	double prcp_unit = (prcp_upper - prcp_lower) / std::distance(prcp_beg, --prcp_end);

	CString str_t, str_p;
	str_t.Format(_T("%d"), static_cast<int>(temp_unit));
	str_p.Format(_T("%d"), static_cast<int>(prcp_unit));

	m_edits[2].SetWindowTextW(str_t);
	m_edits[5].SetWindowTextW(str_p);
}

// COption2Dlg 메시지 처리기


BOOL COption2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFontIndirectW(&lf);
	setFontSize(m_font, 17 * (rect.Height() / 470.0));

	for (int i = 0; i < 6; i++) {
		m_statics[i].Create(_T("STATIC"), WS_VISIBLE | WS_CHILD | SS_RIGHT, CRect(0, 0, 0, 0), this, i + 10);
		m_edits[i].Create(WS_VISIBLE | WS_BORDER | WS_CHILD, CRect(0, 0, 0, 0), this, i + 20);

		m_statics[i].SetFont(&m_font);
		m_edits[i].SetFont(&m_font);
	}

	m_statics[0].SetWindowTextW(_T("상한선 : "));
	m_statics[1].SetWindowTextW(_T("하한선 : "));
	m_statics[2].SetWindowTextW(_T("단위 : "));
	m_statics[3].SetWindowTextW(_T("상한선 : "));
	m_statics[4].SetWindowTextW(_T("하한선 : "));
	m_statics[5].SetWindowTextW(_T("단위 : "));

	m_static_help.Create(_T("STATIC"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 60);
	m_static_help.SetWindowTextW(_T("- 상한선이나 하한선, 단위를 조절해도 데이터에는 영향이 안갑니다.\n\n- 단위는 상한선과 하한선을 정확히 나누어떨어지게끔 해야합니다."));
	m_static_help.SetFont(&m_font);

	updateFromTarget();

	PostMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COption2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);

	int width = Rect.Width();
	int height = Rect.Height();
	int gap = 20 * (Rect.Height() / 470.0);

	CRect rect_group_temp, rect_group_prcp, rect_group_help;

	rect_group_temp = MarginHelper.Initiate().SetMargin(gap)
						.OnLeft(0)
						.OnUp(0)
						.OnRight(width)
						.OnDown(width * 1/3.0, gap / 2)
						.toRect();

	rect_group_prcp = MarginHelper.Initiate().SetMargin(gap)
						.OnLeft(0)
						.OnUp(width * 1 / 3.0, gap / 2)
						.OnRight(width)
						.OnDown(width * 2 / 3.0, gap / 2)
						.toRect();

	rect_group_help = MarginHelper.Initiate().SetMargin(gap)
						.OnLeft(0)
						.OnUp(width * 2 / 3.0, gap / 2)
						.OnRight(width)
						.OnDown(height)
						.toRect();

	int ctrl_gap = 20 * (Rect.Height() / 470.0);
	int static_width = 50 * (Rect.Height() / 470.0);
	int edit_width = 80 * (Rect.Height() / 470.0);
	int ctrl_height = 20 * (Rect.Height() / 470.0);

	if (m_group_temp.GetSafeHwnd() != NULL) {
		m_group_temp.MoveWindow(rect_group_temp);
		m_group_prcp.MoveWindow(rect_group_prcp);
		m_group_help.MoveWindow(rect_group_help);

		rect_group_temp.DeflateRect(10 * (Rect.Height() / 470.0), 10 * (Rect.Height() / 470.0));
		rect_group_prcp.DeflateRect(10 * (Rect.Height() / 470.0), 10 * (Rect.Height() / 470.0));

		rect_group_help.DeflateRect(20 * (Rect.Height() / 470.0), 27 * (Rect.Height() / 470.0));
		m_static_help.MoveWindow(&rect_group_help);

		CRect _static_rect = { 0,0,static_width, ctrl_height };
		CRect _edit_rect = { 0,0,edit_width, ctrl_height };

		int interval_height = rect_group_temp.Height() / 2.0;

		CRect static_rect, edit_rect;
		int i = 0;

		for (CRect rect : {rect_group_temp, rect_group_prcp}) {
			edit_rect = MarginHelper.Initiate()
				.SetRect(_edit_rect)
				.OnCenterLine(interval_height * (0.5) + rect.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachRight(rect.CenterPoint().x, ctrl_gap)
				.toRect();

			static_rect = MarginHelper.Initiate()
				.SetRect(_static_rect)
				.OnCenterLine(interval_height * (0.5) + rect.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachRight(edit_rect.left)
				.toRect();

			m_statics[i].MoveWindow(static_rect);
			m_edits[i++].MoveWindow(edit_rect);

			static_rect = MarginHelper.Initiate()
				.SetRect(_static_rect)
				.OnCenterLine(interval_height * (0.5) + rect.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachLeft(rect.CenterPoint().x, ctrl_gap)
				.toRect();

			edit_rect = MarginHelper.Initiate()
				.SetRect(_edit_rect)
				.OnCenterLine(interval_height * (0.5) + rect.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachLeft(static_rect.right)
				.toRect();

			m_statics[i].MoveWindow(static_rect);
			m_edits[i++].MoveWindow(edit_rect);

			

			edit_rect = MarginHelper.Initiate()
				.SetRect(_edit_rect)
				.OnCenterLine(interval_height * (1.5) + rect.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachRight(rect.CenterPoint().x, ctrl_gap)
				.toRect();

			static_rect = MarginHelper.Initiate()
				.SetRect(_static_rect)
				.OnCenterLine(interval_height * (1.5) + rect.top + 6 * (Rect.Height() / 470.0), MrCenter::Vertical)
				.AttachRight(edit_rect.left)
				.toRect();

			m_statics[i].MoveWindow(static_rect);
			m_edits[i++].MoveWindow(edit_rect);
		}
	}
}


BOOL COption2Dlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->Rectangle(&rect);

	return FALSE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

afx_msg LRESULT COption2Dlg::OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam) {
	if (wParam == TRUE) {
		updateFromTarget();
	}
	else {
		updateToTarget();
	}
	return 0;
}

afx_msg LRESULT COption2Dlg::OnResetInfo(WPARAM wParam, LPARAM lParam) {

	m_edits[0].SetWindowTextW(_T("40"));
	m_edits[1].SetWindowTextW(_T("-40"));
	m_edits[2].SetWindowTextW(_T("10"));
	m_edits[3].SetWindowTextW(_T("500"));
	m_edits[4].SetWindowTextW(_T("0"));
	m_edits[5].SetWindowTextW(_T("50"));

	updateToTarget();

	return 0;
}

BOOL COption2Dlg::PreTranslateMessage(MSG* pMsg)
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
