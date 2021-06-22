// COption3Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "COption3Dlg.h"
#include "afxdialogex.h"


// COption3Dlg 대화 상자

IMPLEMENT_DYNAMIC(COption3Dlg, CDialogEx)

COption3Dlg::COption3Dlg(CWnd* pParent /*=nullptr*/, CGeoGraphDlg* pTarget1, CKoppenIconWnd* pTarget2)
	: CDialogEx(IDD_DIALOG_OPTION3, pParent)
{
	m_pGeoGraphDlg = pTarget1;
	m_pKoppenIconWnd = pTarget2;

	for (int i = 0; i < 2; i++) {
		m_colors_graph[i] = Color::Gray;
	}

	for (int i = 0; i < 5; i++) {
		m_colors_icon[i] = Color::Gray;
	}
}

COption3Dlg::~COption3Dlg()
{
}

void COption3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_GRAPH, m_group_graph);
	DDX_Control(pDX, IDC_GROUP_ICON, m_group_icon);
	DDX_Control(pDX, IDC_BUTTON_GRAPH1, m_buttons_graph[0]);
	DDX_Control(pDX, IDC_BUTTON_GRAPH2, m_buttons_graph[1]);
	DDX_Control(pDX, IDC_BUTTON_ICON1, m_buttons_icon[0]);
	DDX_Control(pDX, IDC_BUTTON_ICON2, m_buttons_icon[1]);
	DDX_Control(pDX, IDC_BUTTON_ICON3, m_buttons_icon[2]);
	DDX_Control(pDX, IDC_BUTTON_ICON4, m_buttons_icon[3]);
	DDX_Control(pDX, IDC_BUTTON_ICON5, m_buttons_icon[4]);
}


BEGIN_MESSAGE_MAP(COption3Dlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER_UPDATE_CLIMATE_INFO, &COption3Dlg::OnUpdateClimateInfo)
	ON_MESSAGE(WM_USER_CLIMATE_RESET, &COption3Dlg::OnResetInfo)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1, &COption3Dlg::OnBnClickedButtonGraph1)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH2, &COption3Dlg::OnBnClickedButtonGraph2)
	ON_BN_CLICKED(IDC_BUTTON_ICON1, &COption3Dlg::OnBnClickedButtonIcon1)
	ON_BN_CLICKED(IDC_BUTTON_ICON2, &COption3Dlg::OnBnClickedButtonIcon2)
	ON_BN_CLICKED(IDC_BUTTON_ICON3, &COption3Dlg::OnBnClickedButtonIcon3)
	ON_BN_CLICKED(IDC_BUTTON_ICON4, &COption3Dlg::OnBnClickedButtonIcon4)
	ON_BN_CLICKED(IDC_BUTTON_ICON5, &COption3Dlg::OnBnClickedButtonIcon5)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void COption3Dlg::updateToTarget() {
	m_pGeoGraphDlg->getTemperGraph()->setColor(m_colors_graph[0]);
	m_pGeoGraphDlg->getPrecipGraph()->setColor(m_colors_graph[1]);

	for (int i = 0; i < 5; i++) {
		m_pKoppenIconWnd->setColor(m_colors_icon[i], i);
	}
}
void COption3Dlg::updateFromTarget() {
	m_colors_graph[0] = m_pGeoGraphDlg->getTemperGraph()->getColor();
	m_colors_graph[1] = m_pGeoGraphDlg->getPrecipGraph()->getColor();

	for (int i = 0; i < 5; i++) {
		m_colors_icon[i] = m_pKoppenIconWnd->getColor(i);
	}
}

void COption3Dlg::openColorDlgForGraph(int idx) {
	CMFCColorDialog ColorDialog(m_colors_graph[idx].ToCOLORREF(), NULL, this);

	if (ColorDialog.DoModal()) {
		COLORREF color = ColorDialog.GetColor();
		m_colors_graph[idx].SetFromCOLORREF(color);

		RedrawWindow();
	}
}
void COption3Dlg::openColorDlgForIcons(int idx) {
	CMFCColorDialog ColorDialog(m_colors_icon[idx].ToCOLORREF(), NULL, this);

	if (ColorDialog.DoModal()) {
		COLORREF color = ColorDialog.GetColor();
		m_colors_icon[idx].SetFromCOLORREF(color);

		RedrawWindow();
	}
}
// COption3Dlg 메시지 처리기

BOOL COption3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFontIndirectW(&lf);
	setFontSize(m_font, 17 * (rect.Height() / 470.0));

	for (int i = 0; i < 2; i++) {
		m_statics_graph[i].Create(_T("STATIC"), WS_CHILD | WS_VISIBLE | SS_RIGHT, CRect(0, 0, 0, 0), this, i + 10);
		m_statics_graph[i].SetFont(&m_font);
	}

	for (int i = 0; i < 5; i++) {
		m_statics_icon[i].Create(_T("STATIC"), WS_CHILD | WS_VISIBLE | SS_RIGHT, CRect(0, 0, 0, 0), this, i + 20);
		m_statics_icon[i].SetFont(&m_font);
	}

	m_statics_graph[0].SetWindowTextW(_T("온도 그래프 색상 : "));
	m_statics_graph[1].SetWindowTextW(_T("강수량 그래프 색상 : "));

	m_statics_icon[0].SetWindowTextW(_T("A_ 아이콘 색상 : "));
	m_statics_icon[1].SetWindowTextW(_T("B_ 아이콘 색상 : "));
	m_statics_icon[2].SetWindowTextW(_T("C_ 아이콘 색상 : "));
	m_statics_icon[3].SetWindowTextW(_T("D_ 아이콘 색상 : "));
	m_statics_icon[4].SetWindowTextW(_T("E_ 아이콘 색상 : "));

	updateFromTarget();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COption3Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);

	int width = Rect.Width();
	int height = Rect.Height();
	int gap = 20 * (Rect.Height() / 470.0);

	CRect rect_group_graph, rect_group_icon;

	rect_group_graph = MarginHelper.Initiate().SetMargin(gap)
		.OnLeft(0)
		.OnUp(0)
		.OnRight(width)
		.OnDown(width * 1 / 3.0, gap / 2)
		.toRect();

	rect_group_icon = MarginHelper.Initiate().SetMargin(gap)
		.OnLeft(0)
		.OnUp(width * 1 / 3.0, gap / 2)
		.OnRight(width)
		.OnDown(height)
		.toRect();

	int ctrl_gap = 20 * (Rect.Height() / 470.0);
	int static_width = 120 * (Rect.Height() / 470.0);
	int button_width = 40 * (Rect.Height() / 470.0);
	int ctrl_height = 20 * (Rect.Height() / 470.0);

	if (m_group_graph.GetSafeHwnd() != NULL) {
		m_group_graph.MoveWindow(&rect_group_graph);
		m_group_icon.MoveWindow(&rect_group_icon);

		rect_group_graph.DeflateRect(10 * (Rect.Height() / 470.0), 10 * (Rect.Height() / 470.0));
		rect_group_icon.DeflateRect(10 * (Rect.Height() / 470.0), 10 * (Rect.Height() / 470.0));

		CRect _static_rect = { 0,0,static_width, ctrl_height };
		CRect _button_rect = { 0,0,button_width, ctrl_height };

		CRect static_rect = MarginHelper.Initiate().SetRect(_static_rect).SetMargin(ctrl_gap)
							.AttachUp(rect_group_graph.top)
							.AttachLeft(rect_group_graph.left)
							.toRect();
		CRect button_rect = MarginHelper.Initiate().SetRect(_button_rect).SetMargin(ctrl_gap)
							.AttachUp(rect_group_graph.top)
							.AttachLeft(static_rect.right, 30 * (Rect.Height() / 470.0))
							.toRect();

		button_rect.OffsetRect(0, -2 * (Rect.Height() / 470.0));

		for (int i = 0; i < 2; i++) {
			m_statics_graph[i].MoveWindow(&static_rect);
			m_buttons_graph[i].MoveWindow(&button_rect);

			static_rect = MarginHelper.Initiate().SetRect(static_rect).SetMargin(ctrl_gap)
							.AttachUp(static_rect.bottom)
							.toRect();
			button_rect = MarginHelper.Initiate().SetRect(button_rect).SetMargin(ctrl_gap)
							.AttachUp(button_rect.bottom)
							.toRect();
		}

		static_rect = MarginHelper.Initiate().SetRect(_static_rect).SetMargin(ctrl_gap)
						.AttachUp(rect_group_icon.top)
						.AttachLeft(rect_group_icon.left)
						.toRect();
		button_rect = MarginHelper.Initiate().SetRect(_button_rect).SetMargin(ctrl_gap)
						.AttachUp(rect_group_icon.top)
						.AttachLeft(static_rect.right, 30 * (Rect.Height() / 470.0))
						.toRect();

		button_rect.OffsetRect(0, -2 * (Rect.Height() / 470.0));

		for (int i = 0; i < 5; i++) {
			m_statics_icon[i].MoveWindow(&static_rect);
			m_buttons_icon[i].MoveWindow(&button_rect);

			static_rect = MarginHelper.Initiate().SetRect(static_rect).SetMargin(ctrl_gap)
				.AttachUp(static_rect.bottom)
				.toRect();
			button_rect = MarginHelper.Initiate().SetRect(button_rect).SetMargin(ctrl_gap)
				.AttachUp(button_rect.bottom)
				.toRect();
		}
	}

}

void COption3Dlg::OnPaint()
{
	CBufferDC dc(this); 
	//CPaintDC dc(this);

	CRect Rect;
	GetClientRect(&Rect);
	dc.Rectangle(&Rect);

	Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	for (int i = 0; i < 2; i++) {
		CRect rect;
		m_statics_graph[i].GetWindowRect(&rect);
		ScreenToClient(&rect);

		graphics.FillRectangle(&SolidBrush(m_colors_graph[i]), rect.right + 7 * (Rect.Height() / 470.0), rect.top, 16 * (Rect.Height() / 470.0), 16 * (Rect.Height() / 470.0));
	}

	for (int i = 0; i < 5; i++) {
		CRect rect;
		m_statics_icon[i].GetWindowRect(&rect);
		ScreenToClient(&rect);

		graphics.FillRectangle(&SolidBrush(m_colors_icon[i]), rect.right + 7 * (Rect.Height() / 470.0), rect.top, 16 * (Rect.Height() / 470.0), 16 * (Rect.Height() / 470.0));
		graphics.DrawRectangle(&Pen(Color::Black, 1.0F), rect.right + 7 * (Rect.Height() / 470.0), rect.top, 16 * (Rect.Height() / 470.0), 16 * (Rect.Height() / 470.0));
	}
}


BOOL COption3Dlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

afx_msg LRESULT COption3Dlg::OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam) {
	if (wParam == TRUE) {
		updateFromTarget();
	}
	else {
		updateToTarget();
	}
	return 0;
}

afx_msg LRESULT COption3Dlg::OnResetInfo(WPARAM wParam, LPARAM lParam) {
	m_colors_graph[0] = Color::Black;
	m_colors_graph[1] = Color::Blue;

	m_colors_icon[0] = Color::Red;
	m_colors_icon[1] = Color::Yellow;
	m_colors_icon[2] = Color::LightGreen;
	m_colors_icon[3] = Color::RoyalBlue;
	m_colors_icon[4] = Color::Aquamarine;

	updateToTarget();
	RedrawWindow();

	return 0;
}


void COption3Dlg::OnBnClickedButtonGraph1()
{
	openColorDlgForGraph(0);
}


void COption3Dlg::OnBnClickedButtonGraph2()
{
	openColorDlgForGraph(1);
}


void COption3Dlg::OnBnClickedButtonIcon1()
{
	openColorDlgForIcons(0);
}


void COption3Dlg::OnBnClickedButtonIcon2()
{
	openColorDlgForIcons(1);
}


void COption3Dlg::OnBnClickedButtonIcon3()
{
	openColorDlgForIcons(2);
}


void COption3Dlg::OnBnClickedButtonIcon4()
{
	openColorDlgForIcons(3);
}


void COption3Dlg::OnBnClickedButtonIcon5()
{
	openColorDlgForIcons(4);
}

BOOL COption3Dlg::PreTranslateMessage(MSG* pMsg)
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