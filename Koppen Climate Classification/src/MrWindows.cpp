#include "pch.h"
#include "MrWindows.h"

using std::begin;
using std::end;
using std::size;

// 각종 유틸리티 함수 정의
namespace MrWindows::Functions {
	CRect getWindowRect(CWnd* wnd) {
		CRect rect;
		wnd->GetWindowRect(&rect);
		return rect;
	}
	CRect getClientRect(CWnd* wnd) {
		CRect rect;
		wnd->GetClientRect(&rect);
		return rect;
	}
	CString getWindowText(CWnd* wnd) {
		CString str;
		wnd->GetWindowTextW(str);
		return str;
	}

	int getWindowCX(CWnd* wnd) {
		CRect rect;
		GetClientRect(wnd->m_hWnd, &rect);
		return rect.Width();
	}
	int getWindowCY(CWnd* wnd) {
		CRect rect;
		GetClientRect(wnd->m_hWnd, &rect);
		return rect.Height();
	}

	void MrFormatMessage(const LPCTSTR lpszText, ...) {
		va_list vlist;
		va_start(vlist, lpszText);

		int len = _vsctprintf(lpszText, vlist) + 1;
		TCHAR* pBuf = new TCHAR[len];

		_vstprintf_s(pBuf, len, lpszText, vlist);
		AfxMessageBox(pBuf);

		delete[] pBuf;
		va_end(vlist);
	}

	void DrawCircle(Graphics& graphics, const Pen* pen, PointF pointf, SizeF sizef) {
		graphics.DrawEllipse(pen, pointf.X - sizef.Width / 2, pointf.Y - sizef.Height/ 2, sizef.Width, sizef.Height);
	}
	void DrawCircle(Graphics& graphics, const Pen* pen, Point point, Size size) {
		graphics.DrawEllipse(pen, point.X - size.Width / 2, point.Y - size.Height / 2, size.Width, size.Height);
	}
	void DrawCircle(Graphics& graphics, const Pen* pen, int x, int y, int cx, int cy) {
		graphics.DrawEllipse(pen, x - cx / 2, y - cy / 2, cx, cy);
	}
	void DrawCircle(Graphics& graphics, const Pen* pen, REAL x, REAL y, REAL cx, REAL cy) {
		graphics.DrawEllipse(pen, x - cx / 2, y - cy / 2, cx, cy);
	}

	void FillCircle(Graphics& graphics, const Brush* brush, PointF pointf, SizeF sizef) {
		graphics.FillEllipse(brush, pointf.X - sizef.Width / 2, pointf.Y - sizef.Height / 2, sizef.Width, sizef.Height);
	}
	void FillCircle(Graphics& graphics, const Brush* brush, Point point, Size size) {
		graphics.FillEllipse(brush, point.X - size.Width / 2, point.Y - size.Height / 2, size.Width, size.Height);
	}
	void FillCircle(Graphics& graphics, const Brush* brush, int x, int y, int cx, int cy) {
		graphics.FillEllipse(brush, x - cx / 2, y - cy / 2, cx, cy);
	}
	void FillCircle(Graphics& graphics, const Brush* brush, REAL x, REAL y, REAL cx, REAL cy) {
		graphics.FillEllipse(brush, x - cx / 2, y - cy / 2, cx, cy);
	}

	void setFontSize(CFont& font, int height) {
		LOGFONT lf;                       

		font.GetLogFont(&lf);
		lf.lfHeight = height;
		font.DeleteObject();
		font.CreateFontIndirect(&lf);
	}

	void setFontName(CFont& font, LPCTSTR str) {
		LOGFONT lf;
		font.GetLogFont(&lf);
		wsprintf(lf.lfFaceName, _T("%s"), str);
		font.DeleteObject();
		font.CreateFontIndirectW(&lf);
	}

	CRect GetDesktopRect() {
		CRect rect;
		CWnd::GetDesktopWindow()->GetWindowRect(&rect);
		return rect;
	}

	void GetDesktopRect(LPRECT rect) {
		CWnd::GetDesktopWindow()->GetWindowRect(rect);
	}
}

// CMarginHelper 구현
namespace MrWindows::Classes {
	// 전역 객체
	CMarginHelper MarginHelper;

	// 생성자 및 기본적인 것들
	CMarginHelper::CMarginHelper() 
		: rect(0,0,0,0)
		, margin(0) 
	{
	}
	CMarginHelper::CMarginHelper(const CMarginHelper& helper)
		: rect(helper.rect)
		, margin(helper.margin)
	{
	}
	CMarginHelper::CMarginHelper(CMarginHelper&& helper) noexcept
		: rect(std::move(helper.rect))
		, margin(std::move(margin))
	{
		helper.rect = CRect(0, 0, 0, 0);
		helper.margin = 0;
	}

	CMarginHelper::~CMarginHelper() {
	}
	CMarginHelper& CMarginHelper::operator=(const CMarginHelper& helper) {
		rect = helper.rect;
		margin = helper.margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::operator=(CMarginHelper&& helper) noexcept {
		rect = std::move(helper.rect);
		margin = std::move(margin);

		helper.rect = CRect(0, 0, 0, 0);
		helper.margin = 0;
		return *this;
	}

	// 핵심 메소드
	CMarginHelper& CMarginHelper::Initiate() {
		rect = CRect(0, 0, 0, 0);
		margin = 0;
		return *this;
	}
	CMarginHelper& CMarginHelper::SetMargin(int margin) {
		this->margin = margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::SetRect(const CRect& rect) {
		this->rect = rect;
		return *this;
	}
	CMarginHelper& CMarginHelper::SetWidth(int width) {
		rect.left = 0;
		rect.right = width;
		return *this;
	}
	CMarginHelper& CMarginHelper::SetHeight(int height) {
		rect.top = 0;
		rect.bottom = height;
		return *this;
	}

	CMarginHelper& CMarginHelper::OnLeft(int leftside, int margin) {
		rect.left = leftside + margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::OnUp(int upside, int margin) {
		rect.top = upside + margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::OnRight(int rightside, int margin) {
		rect.right = rightside - margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::OnDown(int downside, int margin) {
		rect.bottom = downside - margin;
		return *this;
	}

	CMarginHelper& CMarginHelper::OnLeft(int leftside) {
		rect.left = leftside + margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::OnUp(int upside) {
		rect.top = upside + margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::OnRight(int rightside) {
		rect.right = rightside - margin;
		return *this;
	}
	CMarginHelper& CMarginHelper::OnDown(int downside) {
		rect.bottom = downside - margin;
		return *this;
	}

	CMarginHelper& CMarginHelper::AttachLeft(int leftside, int margin) {
		int width = rect.Width();
		rect.left = leftside + margin;
		rect.right = rect.left + width;
		return *this;
	}
	CMarginHelper& CMarginHelper::AttachUp(int upside, int margin) {
		int height = rect.Height();
		rect.top = upside + margin;
		rect.bottom = rect.top + height;
		return *this;
	}
	CMarginHelper& CMarginHelper::AttachRight(int rightside, int margin) {
		int width = rect.Width();
		rect.right = rightside - margin;
		rect.left = rect.right - width;
		return *this;
	}
	CMarginHelper& CMarginHelper::AttachDown(int downside, int margin) {
		int height = rect.Height();
		rect.bottom = downside - margin;
		rect.top = rect.bottom - height;
		return *this;
	}

	CMarginHelper& CMarginHelper::AttachLeft(int leftside) {
		int width = rect.Width();
		rect.left = leftside + margin;
		rect.right = rect.left + width;
		return *this;
	}
	CMarginHelper& CMarginHelper::AttachUp(int upside) {
		int height = rect.Height();
		rect.top = upside + margin;
		rect.bottom = rect.top + height;
		return *this;
	}
	CMarginHelper& CMarginHelper::AttachRight(int rightside) {
		int width = rect.Width();
		rect.right = rightside - margin;
		rect.left = rect.right - width;
		return *this;
	}
	CMarginHelper& CMarginHelper::AttachDown(int downside) {
		int height = rect.Height();
		rect.bottom = downside - margin;
		rect.top = rect.bottom - height;
		return *this;
	}

	CMarginHelper& CMarginHelper::OnCenterLine(int center, MrCenter direction) {
		int width = rect.Width();
		int height = rect.Height();

		switch (direction) {

		case MrCenter::Horizontal :
			rect.left = center - width / 2;
			rect.right = center + width / 2;
			break;

		case MrCenter::Vertical :
			rect.top = center - height / 2;
			rect.bottom = center + height / 2;
			break;

		default:
			MrFormatMessage(_T("CMarginHelper::SetCenterLine()에서 이상한 에러가 남!"));
			break;
		}
		return *this;
	}
	CMarginHelper& CMarginHelper::OnCenterLine(int upleft, int rightbottom, MrCenter direction) {
		OnCenterLine((upleft + rightbottom) / 2, direction);
		return *this;
	}
	CMarginHelper& CMarginHelper::OnCenterLine(const CPoint& point, MrCenter direction) {
		int width = rect.Width();
		int height = rect.Height();

		switch (direction) {

		case MrCenter::Horizontal:
			rect.left = point.x - width / 2;
			rect.right = point.x + width / 2;
			break;

		case MrCenter::Vertical:
			rect.top = point.y - height / 2;
			rect.bottom = point.y + height / 2;
			break;

		default:
			MrFormatMessage(_T("CMarginHelper::OnCenterLine()에서 방향설정 에러가 남!"));
			break;
		}

		return *this;
	}
	CMarginHelper& CMarginHelper::OnCenterLine(const CPoint& point) {
		OnCenterLine(point, MrCenter::Horizontal);
		OnCenterLine(point, MrCenter::Vertical);
		return *this;
	}

	CMarginHelper& CMarginHelper::MoveLeft(int distance) {
		rect.left -= distance;
		rect.right -= distance;
		return *this;
	}
	CMarginHelper& CMarginHelper::MoveUp(int distance) {
		rect.top -= distance;
		rect.bottom -= distance;
		return *this;
	}
	CMarginHelper& CMarginHelper::MoveRight(int distance) {
		rect.left += distance;
		rect.right += distance;
		return *this;
	}
	CMarginHelper& CMarginHelper::MoveDown(int distance) {
		rect.top += distance;
		rect.bottom += distance;
		return *this;
	}

	CRect CMarginHelper::toRect() {
		return rect;
	}

	// Getter 메소드
	int	CMarginHelper::GetMargin() {
		return margin;
	}
	CRect CMarginHelper::GetRect() {
		return rect;
	}

	#ifdef _GDIPLUSTYPES_H

	Gdiplus::Rect CMarginHelper::toGdiRect() {
		return Gdiplus::Rect(rect.left, rect.top, rect.Width(), rect.Height());
	}
	Gdiplus::RectF CMarginHelper::toGdiRectF() {
		return Gdiplus::RectF(rect.left, rect.top, rect.Width(), rect.Height());
	}

	#endif
}

// CPointer 구현
namespace MrWindows::Classes {
	CPointer::CPointer(CWnd* pWnd) 
		: m_bDragging(false) 
		, m_bCaptured(false)
		, m_pWnd(pWnd)
	{
		
	}

	void CPointer::ClickUp() {
		m_bDragging = false;
	}
	void CPointer::ClickDown() {
		m_bDragging = true;
	}

	bool CPointer::isDragging() {
		return m_bDragging;
	}

	void CPointer::SetCapture() {
		m_bCaptured = true;
		m_pWnd->SetCapture();
	}
	void CPointer::ReleaseCapture() {
		m_bCaptured = false;
		::ReleaseCapture();
	}

	bool CPointer::isCapturing() {
		return m_bCaptured;
	}

}

// CGraph 정의
namespace MrWindows::Classes {

	CGraph::CGraph(int size, MrGraphMode graphMode)
		: m_dataVector(size), m_GraphMode(graphMode), m_font_height(0.03), m_pen_width(0.01), m_init_point(0), m_end_point(100), m_bShowing(true), m_bar_width(0.8), m_ball_radius(0.1)
	{
		m_Color = Color::Black;
		m_Font = new Gdiplus::Font(_T("맑은 고딕"), 1, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		m_Rect = CRect(0, 0, 100, 100);
	}
	CGraph::~CGraph() {
		delete m_Font;
	}
	
	//setter/getter
	void CGraph::setGraphMode(MrGraphMode graphMode) {
		m_GraphMode = graphMode;
	}
	MrGraphMode CGraph::getGraphMode() {
		return m_GraphMode;
	}

	void CGraph::setFontHeight(float height) {
		m_font_height = height;
	}
	float CGraph::getFontHeight() {
		return m_font_height;
	}
	void CGraph::setPenWidth(float width) {
		m_pen_width = width;
	}
	float CGraph::getPenWidth() {
		return m_pen_width;
	}
	void CGraph::setBarWidth(float width) {
		m_bar_width = width;
	}
	float CGraph::getBarWidth() {
		return m_bar_width;
	}
	void CGraph::setBallRadius(float radius) {
		m_ball_radius = radius;
	}
	float CGraph::getBallRadius() {
		return m_ball_radius;
	};

	void CGraph::setGraphLimits(float init, float end) {
		m_init_point = init;
		m_end_point = end;
	}
	tuple<float, float> CGraph::getGraphLimits() {
		return make_tuple(m_init_point, m_end_point);
	}

	void CGraph::show() {
		m_bShowing = true;
	}
	void CGraph::hide() {
		m_bShowing = false;
	}
	bool CGraph::isShowing() {
		return m_bShowing;
	}

	//setter
	void CGraph::setColor(Color color) {
		m_Color = color;
	}
	//font's height is ignored for dynamic sizing;
	void CGraph::setFont(const Gdiplus::Font* font) {
		delete m_Font;
		m_Font = font->Clone();
	}

	void CGraph::setRect(const CRect* rect) {
		m_Rect = *rect;
	}
	void CGraph::setRect(const Rect& rect) {
		m_Rect = CRect(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
	}
	void CGraph::setRect(const RectF& rect) {
		m_Rect = CRect(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
	}


	//getter
	Color CGraph::getColor() {
		return m_Color;
	}
	Gdiplus::Font* CGraph::getFont() {
		return m_Font;
	}

	CRect CGraph::getRect() {
		return m_Rect;
	}
	Rect CGraph::getGdiRect() {
		return Rect(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());
	}
	RectF CGraph::getGdiRectF() {
		return RectF(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());
	}


	// public method functions
	CGraph& CGraph::setData(double data, int index) {
		m_dataVector[index] = data;
		return *this;
	}
	
	CGraph& CGraph::setData(std::initializer_list<double> init_list) {
		std::copy_n(begin(init_list), size(m_dataVector), begin(m_dataVector));
		return *this;
	}

	tuple<double, int> CGraph::getHighest() {
		double highest_data = m_init_point;
		int highest_numth = 0;

		for (int i = 0; i < m_dataVector.size(); i++) {
			if (highest_data < m_dataVector.at(i)) {
				highest_data = m_dataVector.at(i);
				highest_numth = i;
			}
		}

		return make_tuple(highest_data, highest_numth);
	}
	tuple<double, int> CGraph::getLowest() {
		double lowest_data = m_end_point;
		int lowest_numth = 0;

		for (int i = 0; i < m_dataVector.size(); i++) {
			if (lowest_data > m_dataVector.at(i)) {
				lowest_data = m_dataVector.at(i);
				lowest_numth = i;
			}
		}

		return make_tuple(lowest_data, lowest_numth);
	}

	double CGraph::getTotal() {
		return std::accumulate(begin(m_dataVector), end(m_dataVector), 0.0);
	}
	double CGraph::getAverage() {
		return getTotal() / m_dataVector.size();
	}

	vector<double>* CGraph::getVector() {
		return &m_dataVector;
	}

	void CGraph::drawGraph(Graphics& graphics) {
		if (m_bShowing == false) return;

		switch(m_GraphMode) {

		case MrGraphMode::Lines :
			drawLineGraph(graphics);
			break;

		case MrGraphMode::Bars :
			drawBarGraph(graphics);
			break;

		}
	}
	

	void CGraph::drawLineGraph(Graphics& graphics) {
		Color tmp = { GetSysColor(COLOR_BTNFACE) };
		Color BkColor = Color::White;// { tmp.GetR(), tmp.GetB(), tmp.GetG() };

		int width = m_Rect.Width();
		int height = m_Rect.Height();
		int size = m_dataVector.size();
		float interval = (float)width / size;

		Pen pen = Pen( m_Color, m_pen_width * std::pow(height, 0.85));
		const SolidBrush EraseBrush = { BkColor };

		float string_height = (width + height)/2 * m_font_height;

		GraphicsPath path;
		path.AddRectangle(this->getGdiRect());
		
		Region rgn(&path);
		graphics.SetClip(&rgn);

		for (int i = 0; i < size - 1; i++) {
			graphics.DrawLine(&pen, toForge(i, m_Rect), toForge(i+1, m_Rect));
		}
		
		for (int i = 0; i < size; i++) {
			FillCircle(graphics, &EraseBrush, toForge(i, m_Rect), SizeF(interval * m_ball_radius * 2, interval * m_ball_radius * 2));
			DrawCircle(graphics, &pen, toForge(i, m_Rect), SizeF(interval * m_ball_radius * 2, interval * m_ball_radius * 2));
		}

		graphics.ResetClip();
	}
	void CGraph::drawBarGraph(Graphics& graphics) {
		int width = m_Rect.Width();
		int height = m_Rect.Height();
		int size = m_dataVector.size();
		float interval = (float)width / size;

		SolidBrush brush = { m_Color };

		GraphicsPath path;
		path.AddRectangle(this->getGdiRect());

		Region rgn(&path);
		graphics.SetClip(&rgn);

		RectF bar_rect = { 0, 0, 0, 0 };
		MarginHelper.Initiate().SetWidth((double)width / size * m_bar_width);

		for (int i = 0; i < size; i++) {
			bar_rect = MarginHelper
				.OnUp(toForgeY(i,m_Rect))
				.OnDown(m_Rect.top + height)
				.OnCenterLine(toForgeX(i, m_Rect), MrCenter::Horizontal)
				.toGdiRectF();

			graphics.FillRectangle(&brush, bar_rect);
		}

		graphics.ResetClip();
	}


	// prviate method functions
	tuple<int, CPoint> CGraph::isAnyIndexClicked(CPoint point, CSize size) {
		int width = m_Rect.Width();
		int height = m_Rect.Height();

		float data_size = m_dataVector.size();
		float interval = width / data_size;

		// initialize

		if (m_bShowing == false || m_Rect.PtInRect(point) == false) {
			return make_tuple(-1, CPoint());
		}

		if (size.cy < 0) {
			size.cy = width / data_size;
		}

		// error check & modifying


		int point_at = (float)(point.x - m_Rect.left) / width * data_size;


		CRect target_rect = MarginHelper.Initiate()
			.SetWidth(size.cx)
			.SetHeight(size.cy)
			.OnCenterLine(toForgeX(point_at, m_Rect), MrCenter::Horizontal)
			.OnCenterLine(toForgeY(point_at, m_Rect), MrCenter::Vertical)
			.toRect();

		if (PtInRect(target_rect, point)) {
			return make_tuple(point_at, target_rect.CenterPoint());
		}
		else {
			return make_tuple(-1, CPoint());
		}
	}
	float CGraph::convertData(int client_point_y)
	{
		int height = m_Rect.Height();
		int point = client_point_y - m_Rect.top;

		float absolute_point = 1 - (float)point / height;
		float data_width = m_end_point - m_init_point;

		float data = m_init_point + data_width * absolute_point;
		data = max(m_init_point, data);
		data = min(m_end_point, data);
		return data;
	}

	CRect CGraph::getUpdateRect(int index)
	{
		return getUpdateRect(index - 1, index + 1);
	}
	CRect CGraph::getUpdateRect(int start_index, int end_index)
	{
		assert(start_index < end_index);

		CRect UpdateRect = MarginHelper.Initiate()
							.OnUp(m_Rect.top)
							.OnDown(m_Rect.bottom)
							.OnLeft(toForgeX(start_index, m_Rect))
							.OnRight(toForgeX(end_index, m_Rect))
							.toRect();
		return UpdateRect;
	}


	PointF CGraph::toForge(int index) {
		return PointF(toForgeX(index), toForgeY(index));
	}
	float CGraph::toForgeX(int index) {
		float interval = 1.0 / m_dataVector.size();
		return interval * index + interval / 2;
	}
	float CGraph::toForgeY(int index) {
		float length = m_end_point - m_init_point;
		float data = m_dataVector.at(index);

		return 1 - (data - m_init_point) / length;
	}

	PointF CGraph::toForge(int index, const CRect& rect) {
		return PointF(toForgeX(index, rect), toForgeY(index, rect));
	}
	float CGraph::toForgeX(int index, const CRect& rect) {
		return rect.left + toForgeX(index) * rect.Width();
	}
	float CGraph::toForgeY(int index, const CRect& rect) {
		return rect.top + toForgeY(index) * rect.Height();
	}
}



// CGraphMarker 정의
namespace MrWindows::Classes {
	CGraphMarker::CGraphMarker() : m_font_height(0.03)
	{
		m_Pen = new Pen(Color::Black, 1.5F);
		m_Brush = new SolidBrush(Color::Black);
		m_Font = new Gdiplus::Font(_T("맑은 고딕"), 1, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		m_Rect = CRect(0, 0, 100, 100);
	}
	CGraphMarker::~CGraphMarker()
	{
		delete m_Pen;
		delete m_Brush;
		delete m_Font;
	}

	//font height get/setter
	void CGraphMarker::setFontHeight(float height) {
		m_font_height = height;
	}
	float CGraphMarker::getFontHeight() {
		return m_font_height;
	}

	//setter
	void CGraphMarker::setBrush(const SolidBrush* brush) {
		delete m_Brush;
		m_Brush = brush->Clone();
	}
	void CGraphMarker::setPen(const Pen* pen) {
		delete m_Pen;
		m_Pen = pen->Clone();
	}
	//font's height is ignored for dynamic sizing;
	void CGraphMarker::setFont(const Gdiplus::Font* font) {
		delete m_Font;
		m_Font = font->Clone();
	}

	void CGraphMarker::setRect(const CRect* rect) {
		m_Rect = *rect;
	}
	void CGraphMarker::setRect(const Rect& rect) {
		m_Rect = CRect(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
	}
	void CGraphMarker::setRect(const RectF& rect) {
		m_Rect = CRect(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
	}


	//getter
	Brush* CGraphMarker::GetBrush() {
		return m_Brush->Clone();
	}
	Pen* CGraphMarker::getPen() {
		return m_Pen->Clone();
	}
	Gdiplus::Font* CGraphMarker::getFont() {
		return m_Font->Clone();
	}

	CRect CGraphMarker::getRect() {
		return m_Rect;
	}
	Rect CGraphMarker::getGdiRect() {
		return Rect(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());
	}
	RectF CGraphMarker::getGdiRectF() {
		return RectF(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());
	}

	auto CGraphMarker::getVector() -> vector<tuple<MrInOut, MrDirection, double, CString>>* {
		return &m_marking_vector;
	}


	// public method
	CGraphMarker& CGraphMarker::addMark(double d, const CString& string, MrDirection direction, MrInOut InOut) 
	{
		m_marking_vector.push_back(make_tuple(InOut, direction, d, string));
		return *this;
	}
	CGraphMarker& CGraphMarker::addMark(const tuple<double, CString, MrDirection, MrInOut>& tuple) 
	{
		auto [ratio, name, direction, inOut] = tuple;
		m_marking_vector.push_back(make_tuple(inOut, direction, ratio, name));
		return *this;
	}
	CGraphMarker& CGraphMarker::removeMark(MrDirection direction, MrInOut inOut) {
		auto pos = std::remove_if(m_marking_vector.begin(), m_marking_vector.end(), [&](tuple<MrInOut, MrDirection, double, CString> tup) {
			auto [iO, dir, ratio, Cs] = tup;
			if (iO == inOut && dir == direction) {
				return true;
			}
			else {
				return false;
			}
		});

		m_marking_vector.erase(pos, m_marking_vector.end());

		return *this;
	}
	CGraphMarker& CGraphMarker::removeMark(const CString& str, MrDirection direction, MrInOut inOut) {
		auto pos = std::remove_if(m_marking_vector.begin(), m_marking_vector.end(), [&](tuple<MrInOut, MrDirection, double, CString> tup) {
			auto [iO, dir, ratio, Cs] = tup;
			if (iO == inOut && dir == direction && Cs == str) {
				return true;
			}
			else {
				return false;
			}
		});

		m_marking_vector.erase(pos, m_marking_vector.end());

		return *this;
	}
	CGraphMarker& CGraphMarker::removeMark(MrDirection direction, MrInOut inOut, const std::initializer_list<CString>& ilist) {
		for (CString str : ilist) {
			removeMark(str, direction, inOut);
		}

		return *this;
	}

	CString CGraphMarker::arrange() {
		std::sort(begin(m_marking_vector), end(m_marking_vector));
		auto pos = std::unique(begin(m_marking_vector), end(m_marking_vector));
		m_marking_vector.erase(pos, end(m_marking_vector));

		CString str = _T("");

		for (auto e : m_marking_vector) {
			auto [inOut, direction, ratio, name] = e;
			str.AppendFormat(_T("inOut : %d, direction : %d, data : %f, name : %s\n"), inOut, direction, ratio, name);
		}

		return str;
	}

	void CGraphMarker::drawMarkings(Graphics& graphics) 
	{
		int width = m_Rect.Width();
		int height = m_Rect.Height();

		Gdiplus::Font* prev_font = m_Font;

		Gdiplus::FontFamily fontfamily;
		m_Font->GetFamily(&fontfamily);
		m_Font = new Gdiplus::Font(&fontfamily, (width + height) / 2 * m_font_height, m_Font->GetStyle(), m_Font->GetUnit());

		delete prev_font;

		StringFormat stringformat_center, stringformat_left, stringformat_right;
		stringformat_center.SetAlignment(Gdiplus::StringAlignmentCenter);
		stringformat_center.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		stringformat_left.SetAlignment(Gdiplus::StringAlignmentNear);
		stringformat_left.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		stringformat_right.SetAlignment(Gdiplus::StringAlignmentFar);
		stringformat_right.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		for (auto tuple : m_marking_vector) {
			drawMarking(graphics, tuple);
		}
	}

	CGraphMarker& CGraphMarker::addSuccessiveMark(int init, int end, int unit, MrDirection direction, MrInOut inOut) {
		CString str = _T("");

		for (int i = init; i <= end; i += unit) {
			str.Format(_T("%d"), i);
			m_marking_vector.push_back(make_tuple(inOut, direction, static_cast<double>(i-init) / (end-init+1), str));
		}
		return *this;
	}

	// private method
	void CGraphMarker::drawMarking(Graphics& graphics, const tuple<MrInOut, MrDirection, double, CString>& tuple)
	{
		int width = m_Rect.Width();
		int height = m_Rect.Height();

		StringFormat stringformat_center, stringformat_left, stringformat_right;
		stringformat_center.SetAlignment(Gdiplus::StringAlignmentCenter);
		stringformat_center.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		stringformat_left.SetAlignment(Gdiplus::StringAlignmentNear);
		stringformat_left.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		stringformat_right.SetAlignment(Gdiplus::StringAlignmentFar);
		stringformat_right.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		auto [InOut, direction, ratio, name] = tuple;

		int interval_height = (width + height) / 1.5 * m_font_height;

		if (InOut == MrInOut::Outer) interval_height /= 4;

		MarginHelper.Initiate()
			.SetHeight(m_Font->GetHeight(&graphics))
			.SetWidth(0)
			.SetMargin(interval_height);


		switch (direction) {
		case MrDirection::Left:
		{
			if (InOut == MrInOut::Inner) {
				MarginHelper.AttachLeft(m_Rect.left);
			}
			else {
				MarginHelper.AttachRight(m_Rect.left);
			}

			CRect tmp = MarginHelper.OnCenterLine(m_Rect.top + (height * (1 - ratio)), MrCenter::Vertical)
				.toRect();

			PointF pt_str = PointF(tmp.CenterPoint().x, tmp.CenterPoint().y);
			PointF pt_interval1 = PointF(tmp.left, tmp.CenterPoint().y);
			PointF pt_interval2 = PointF(m_Rect.left, tmp.CenterPoint().y);

			pt_interval1.X -= interval_height / 2.5;

			if (InOut == MrInOut::Inner) {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_left, m_Brush);
				graphics.DrawLine(m_Pen, pt_interval1, pt_interval2);
			}
			else {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_right, m_Brush);
			}
			break;
		}

		case MrDirection::Right:
		{
			if (InOut == MrInOut::Inner) {
				MarginHelper.AttachRight(m_Rect.right);
			}
			else {
				MarginHelper.AttachLeft(m_Rect.right);
			}

			CRect tmp = MarginHelper.OnCenterLine(m_Rect.top + (height * (1 - ratio)), MrCenter::Vertical)
				.toRect();

			PointF pt_str = PointF(tmp.CenterPoint().x, tmp.CenterPoint().y);
			PointF pt_interval1 = PointF(tmp.right, tmp.CenterPoint().y);
			PointF pt_interval2 = PointF(m_Rect.right, tmp.CenterPoint().y);

			pt_interval1.X += interval_height / 2.5;

			if (InOut == MrInOut::Inner) {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_right, m_Brush);
				graphics.DrawLine(m_Pen, pt_interval1, pt_interval2);
			}
			else {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_left, m_Brush);
			}
			break;
		}

		case MrDirection::Up:
		{
			if (InOut == MrInOut::Inner) {
				MarginHelper.AttachUp(m_Rect.top);
			}
			else {
				MarginHelper.AttachDown(m_Rect.top);
			}

			CRect tmp = MarginHelper.OnCenterLine(m_Rect.left + (width * ratio), MrCenter::Horizontal)
				.toRect();

			PointF pt_str = PointF(tmp.CenterPoint().x, tmp.CenterPoint().y);
			PointF pt_interval1 = PointF(tmp.CenterPoint().x, tmp.top);
			PointF pt_interval2 = PointF(tmp.CenterPoint().x, m_Rect.top);

			pt_interval1.Y -= interval_height / 2.5;

			if (InOut == MrInOut::Inner) {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_center, m_Brush);
				graphics.DrawLine(m_Pen, pt_interval1, pt_interval2);
			}
			else {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_center, m_Brush);
			}

			break;
		}

		case MrDirection::Down:
		{
			if (InOut == MrInOut::Inner) {
				MarginHelper.AttachDown(m_Rect.bottom);
			}
			else {
				MarginHelper.AttachUp(m_Rect.bottom);
			}

			CRect tmp = MarginHelper.OnCenterLine(m_Rect.left + (width * ratio), MrCenter::Horizontal)
				.toRect();

			PointF pt_str = PointF(tmp.CenterPoint().x, tmp.CenterPoint().y);
			PointF pt_interval1 = PointF(tmp.CenterPoint().x, tmp.bottom);
			PointF pt_interval2 = PointF(tmp.CenterPoint().x, m_Rect.bottom);

			pt_interval1.Y += interval_height / 2.5;

			if (InOut == MrInOut::Inner) {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_center, m_Brush);
				graphics.DrawLine(m_Pen, pt_interval1, pt_interval2);
			}
			else {
				graphics.DrawString(name, -1, m_Font, pt_str, &stringformat_center, m_Brush);
			}

			//break;
		}
		}
	}
}

