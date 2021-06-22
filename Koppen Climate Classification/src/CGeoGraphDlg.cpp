// CGeoGraphDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "CGeoGraphDlg.h"
#include "afxdialogex.h"


// CGeoGraphDlg 대화 상자

IMPLEMENT_DYNAMIC(CGeoGraphDlg, CDialogEx)

CGeoGraphDlg::CGeoGraphDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GEOGRAPH, pParent)
	, m_Pointer(this)
	, m_Graph_Temper(12,MrGraphMode::Lines)
	, m_Graph_Precip(12,MrGraphMode::Bars)
	, m_Graph_Marker()
	, m_now_index(-1)
	, m_now_pGraph(nullptr)
	, m_pointer_offset(0)
{

}

CGeoGraphDlg::~CGeoGraphDlg()
{
}

void CGeoGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGeoGraphDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CGraph* CGeoGraphDlg::getTemperGraph() {
	return &m_Graph_Temper;
}
CGraph* CGeoGraphDlg::getPrecipGraph() {
	return &m_Graph_Precip;
}
CGraphMarker* CGeoGraphDlg::getGraphMarker() {
	return &m_Graph_Marker;
}

void CGeoGraphDlg::paintImage1(CImage& image) {
	CDC* pDC = CDC::FromHandle(image.GetDC());
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	CRect Rect = { 0, 0, image.GetWidth(), image.GetHeight() };

	int width = Rect.Width();
	int height = Rect.Height();

	graphics.FillRectangle(&SolidBrush(Color::White), -2, -2, Rect.Width() + 2, Rect.Height() + 2);
	graphics.DrawRectangle(&Pen(Color::White, 1.0F), 0, 0, Rect.Width() - 1, Rect.Height() * (1 - 0.01));

	Rect.DeflateRect(width * 0.12, height * 0.12);

	graphics.FillRectangle(&SolidBrush(Color::White), Rect.left, Rect.top, Rect.Width(), Rect.Height());

	m_Graph_Temper.setRect(&Rect);
	m_Graph_Precip.setRect(&Rect);
	m_Graph_Marker.setRect(&Rect);

	m_Graph_Marker.drawMarkings(graphics);
	m_Graph_Precip.drawGraph(graphics);
	m_Graph_Temper.drawGraph(graphics);

	graphics.DrawRectangle(&Pen(Color::Black), Rect.left, Rect.top, Rect.Width(), Rect.Height());

	image.ReleaseDC();
}

void CGeoGraphDlg::paintImage2(CImage& image) {
	CDC* pDC = CDC::FromHandle(image.GetDC());
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	CRect Rect = { 0, 0, image.GetWidth(), image.GetHeight() };

	int width = Rect.Width();
	int height = Rect.Height();

	graphics.FillRectangle(&SolidBrush(Color::White), -2, -2, Rect.Width() + 2, Rect.Height() + 2);
	graphics.DrawRectangle(&Pen(Color::White, 1.0F), 0, 0, Rect.Width() - 1, Rect.Height() * (1 - 0.01));

	Rect.DeflateRect(width * 0.18, height * 0.18);

	graphics.FillRectangle(&SolidBrush(Color::White), Rect.left, Rect.top, Rect.Width(), Rect.Height());

	m_Graph_Temper.setRect(&Rect);
	m_Graph_Precip.setRect(&Rect);
	m_Graph_Marker.setRect(&Rect);

	m_Graph_Temper.setBallRadius(0.25);
	m_Graph_Precip.setBarWidth(0.5);
	m_Graph_Marker.removeMark(MrDirection::Down, MrInOut::Outer);

	double month_interval = 1 / 12.0;

	for (int i : {1, 3, 5, 7, 9, 11}) {
		CString str;
		str.Format(_T("%d"), i);
		m_Graph_Marker.addMark(month_interval * (i - 0.5), str, MrDirection::Down, MrInOut::Outer);
	}

	m_Graph_Marker.removeMark(_T("11"), MrDirection::Down, MrInOut::Outer);
	m_Graph_Marker.addMark(1 - month_interval * 1.5, _T("      11 (월)"), MrDirection::Down, MrInOut::Outer);
	m_Graph_Marker.arrange();

	m_Graph_Temper.setFontHeight(0.05);
	m_Graph_Precip.setFontHeight(0.05);
	m_Graph_Marker.setFontHeight(0.05);

	m_Graph_Marker.drawMarkings(graphics);
	m_Graph_Precip.drawGraph(graphics);
	m_Graph_Temper.drawGraph(graphics);

	m_Graph_Temper.setFontHeight(0.023);
	m_Graph_Precip.setFontHeight(0.023);
	m_Graph_Marker.setFontHeight(0.027);

	m_Graph_Temper.setBallRadius(0.1);
	m_Graph_Precip.setBarWidth(0.8);
	m_Graph_Marker.removeMark(MrDirection::Down, MrInOut::Outer);

	for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}) {
		CString str;
		str.Format(_T("%d"), i);
		m_Graph_Marker.addMark(month_interval * (i - 0.5), str, MrDirection::Down, MrInOut::Outer);
	}

	m_Graph_Marker.removeMark(_T("12"), MrDirection::Down, MrInOut::Outer);
	m_Graph_Marker.addMark(1 - month_interval / 2, _T("      12 (월)"), MrDirection::Down, MrInOut::Outer);
	m_Graph_Marker.arrange();

	graphics.DrawRectangle(&Pen(Color::Black), Rect.left, Rect.top, Rect.Width(), Rect.Height());

	image.ReleaseDC();
}

// CGeoGraphDlg 메시지 처리기


BOOL CGeoGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	double month_interval = 1 / 12.0;

	for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}) {
		CString str;
		str.Format(_T("%d"), i);
		m_Graph_Marker.addMark(month_interval * (i - 0.5), str, MrDirection::Down, MrInOut::Outer);
	}

	m_Graph_Marker.addMark(1.05, _T("(℃)"), MrDirection::Left, MrInOut::Outer);
	m_Graph_Marker.addMark(1.05, _T("(mm)"), MrDirection::Right, MrInOut::Outer);
	m_Graph_Marker.removeMark(_T("12"), MrDirection::Down, MrInOut::Outer);
	m_Graph_Marker.addMark(1 - month_interval / 2, _T("      12 (월)"), MrDirection::Down, MrInOut::Outer);

	m_Graph_Temper.setGraphLimits(-40.0f, 40.0f);
	m_Graph_Temper.setGraphMode(MrGraphMode::Lines);
	m_Graph_Temper.setFontHeight(0.023);
	m_Graph_Temper.setPenWidth(0.005);
	m_Graph_Temper.setColor(Color::Black);

	m_Graph_Precip.setGraphLimits(0.0f, 500.0f);
	m_Graph_Precip.setGraphMode(MrGraphMode::Bars);
	m_Graph_Precip.setFontHeight(0.023);
	m_Graph_Precip.setColor(Color::Blue);

	m_Graph_Marker.setFontHeight(0.027);

	float tempers_Changwon[12] = { 2.8, 4.7, 8.7, 14.1, 18.4, 21.7, 25.2, 26.5, 22.8, 17.4, 11.0 ,5.4 };
	float precips_Changwon[12] = { 36.9, 45.3, 74.3, 129.9, 142.2, 232.3, 293.8, 299.0, 167.3, 50.2, 52.2, 22.0 };

	for (int i = 0; i < 12; i++) {
		CString str_t, str_p;
		str_t.Format(_T("data_t_%d"), i);
		str_p.Format(_T("data_p_%d"), i);
		m_Graph_Temper.getVector()->at(i) = AfxGetApp()->GetProfileInt(_T("KoppenDlg"), str_t, tempers_Changwon[i] * 10) / 10.0;
		m_Graph_Precip.getVector()->at(i) = AfxGetApp()->GetProfileInt(_T("KoppenDlg"), str_p, precips_Changwon[i] * 10) / 10.0;
	}

	int original_datas[6] = { -40, 40, 10, 0, 500, 50 };
	int marking_datas[6];

	for (int i = 0; i < 6; i++) {
		CString str_m;
		str_m.Format(_T("data_m_%d"), i);
		marking_datas[i] = AfxGetApp()->GetProfileInt(_T("KoppenDlg"), str_m, original_datas[i]);
	}

	m_Graph_Temper.setGraphLimits(marking_datas[0], marking_datas[1]);
	m_Graph_Precip.setGraphLimits(marking_datas[3], marking_datas[4]);

	m_Graph_Marker.addSuccessiveMark(marking_datas[0], marking_datas[1], marking_datas[2], MrDirection::Left, MrInOut::Outer);
	m_Graph_Marker.addSuccessiveMark(marking_datas[3], marking_datas[4], marking_datas[5], MrDirection::Right, MrInOut::Outer);

	m_Graph_Marker.arrange();

	GetParent()->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO);

	return TRUE;
}

void CGeoGraphDlg::PostNcDestroy()
{
	for (int i = 0; i < 12; i++) {
		CString str_t, str_p;
		str_t.Format(_T("data_t_%d"), i);
		str_p.Format(_T("data_p_%d"), i);
		AfxGetApp()->WriteProfileInt(_T("KoppenDlg"), str_t, m_Graph_Temper.getVector()->at(i) * 10);
		AfxGetApp()->WriteProfileInt(_T("KoppenDlg"), str_p, m_Graph_Precip.getVector()->at(i) * 10);
	}

	int marking_datas[6];
	std::tie(marking_datas[0], marking_datas[1]) = m_Graph_Temper.getGraphLimits();
	std::tie(marking_datas[3], marking_datas[4]) = m_Graph_Precip.getGraphLimits();

	auto temp_finder = [](auto tuple) {
		auto [inOut, direction, ratio, string] = tuple;
		return inOut == MrInOut::Outer && direction == MrDirection::Left;
	};

	auto precip_finder = [](auto tuple) {
		auto [inOut, direction, ratio, string] = tuple;
		return inOut == MrInOut::Outer && direction == MrDirection::Right;
	};

	auto pVector = m_Graph_Marker.getVector();

	auto temp_beg = std::find_if(pVector->begin(), pVector->end(), temp_finder);
	auto temp_end = std::find_if(pVector->rbegin(), pVector->rend(), temp_finder).base();
	auto prcp_beg = std::find_if(pVector->begin(), pVector->end(), precip_finder);
	auto prcp_end = std::find_if(pVector->rbegin(), pVector->rend(), precip_finder).base();
	temp_end--, prcp_end--;

	marking_datas[2] = (marking_datas[1] - marking_datas[0]) / std::distance(temp_beg, --temp_end);
	marking_datas[5] = (marking_datas[4] - marking_datas[3]) / std::distance(prcp_beg, --prcp_end);

	for (int i = 0; i < 6; i++) {
		CString str_m;
		str_m.Format(_T("data_m_%d"), i);
		AfxGetApp()->WriteProfileInt(_T("KoppenDlg"), str_m, marking_datas[i]);
	}

	CDialogEx::PostNcDestroy();
}

BOOL CGeoGraphDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CGeoGraphDlg::OnPaint()
{
	CBufferDC dc(this);

	Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	CRect Rect;
	GetClientRect(&Rect);

	int width = Rect.Width();
	int height = Rect.Height();

	Color sys_color = { RGB_TO_ARGB(255,GetSysColor(COLOR_BTNFACE)) };
	graphics.FillRectangle(&SolidBrush(sys_color), -2, -2, Rect.Width() + 2, Rect.Height() + 2);
	graphics.DrawRectangle(&Pen(Color::DarkGray, 2.0F), 0, 0, Rect.Width() - 1, Rect.Height() * (1 - 0.01));

	Rect.DeflateRect(width*0.12, height*0.12);

	graphics.FillRectangle(&SolidBrush(Color::White), Rect.left, Rect.top, Rect.Width(), Rect.Height());

	m_Graph_Temper.setRect(&Rect);
	m_Graph_Precip.setRect(&Rect);
	m_Graph_Marker.setRect(&Rect);

	m_Graph_Marker.drawMarkings(graphics);
	m_Graph_Precip.drawGraph(graphics);
	m_Graph_Temper.drawGraph(graphics);

	graphics.DrawRectangle(&Pen(Color::Black), Rect.left, Rect.top, Rect.Width(), Rect.Height());
}


void CGeoGraphDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Pointer.ClickDown();
	m_Pointer.SetCapture();

	for (CGraph* pGraph : {&m_Graph_Temper, &m_Graph_Precip} ) {
		auto [idx, target_point] = pGraph->isAnyIndexClicked(point, CSize(50, 50));

		if (idx >= 0) {
			m_now_index = idx;
			m_now_pGraph = pGraph;
			m_pointer_offset = target_point.y - point.y;
			break;
		} 
		else {
			m_now_pGraph = nullptr;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CGeoGraphDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Pointer.ClickUp();
	m_Pointer.ReleaseCapture();

	GetParent()->PostMessageW(WM_USER_UPDATE_CLIMATE_INFO);

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CGeoGraphDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Pointer.isDragging() && m_now_pGraph != nullptr) {
		int actual_point = point.y + m_pointer_offset;

		float data = m_now_pGraph->convertData(point.y);
		m_now_pGraph->setData(data, m_now_index);

		CRect UpdateRect = m_now_pGraph->getUpdateRect(m_now_index);

		RedrawWindow(UpdateRect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CGeoGraphDlg::PreTranslateMessage(MSG* pMsg)
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
