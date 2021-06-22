// CKoppenIconWnd.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "CKoppenIconWnd.h"


// CKoppenIconWnd

IMPLEMENT_DYNAMIC(CKoppenIconWnd, CWnd)

CKoppenIconWnd::CKoppenIconWnd() 
	: m_koppen_sign(_T(""))
{
	m_icon_colors[0] = Color::Red;
	m_icon_colors[1] = Color::Yellow;
	m_icon_colors[2] = Color::LightGreen;
	m_icon_colors[3] = Color::RoyalBlue;
	m_icon_colors[4] = Color::Aquamarine;
}

CKoppenIconWnd::~CKoppenIconWnd()
{
}


BEGIN_MESSAGE_MAP(CKoppenIconWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CKoppenIconWnd::setKoppenSign(LPCTSTR sign) {
	m_koppen_sign = sign;
}
LPCTSTR CKoppenIconWnd::getKoppenSign() {
	return m_koppen_sign;
}

Color CKoppenIconWnd::getColor(int idx) {
	return m_icon_colors[idx];
}
void CKoppenIconWnd::setColor(Color color, int idx) {
	m_icon_colors[idx] = color;
}

void CKoppenIconWnd::DrawIcon(Graphics& graphics, const CRect& rect, LPCTSTR sign) {
	TCHAR first_letter = sign[0];

	int width = rect.Width();
	int height = rect.Height();

	graphics.FillRectangle(&SolidBrush(Color::White), rect.left, rect.top, rect.Width(), rect.Height());
	graphics.DrawRectangle(&Pen(Color::Black, height * 0.02), rect.left, rect.top, rect.Width() - 1, rect.Height() - 1);

	Color color_letter;

	switch (first_letter) {
	case _T('A') :
		color_letter = m_icon_colors[0];
		break;

	case _T('B') :
		color_letter = m_icon_colors[1];
		break;

	case _T('C') :
		color_letter = m_icon_colors[2];
		break;

	case _T('D') :
		color_letter = m_icon_colors[3];
		break;

	case _T('E') :
		color_letter = m_icon_colors[4];
		break;

	default :
		color_letter = Color::Gray;
	}

	CPoint center_point = rect.CenterPoint();

	GraphicsPath path;
	StringFormat format;
	format.SetAlignment(StringAlignment::StringAlignmentCenter);
	format.SetLineAlignment(StringAlignment::StringAlignmentCenter);

	path.AddString(sign, -1, &Gdiplus::FontFamily(_T("맑은 고딕")), Gdiplus::FontStyleBold, height * 0.5, Point(center_point.x, center_point.y), &format);

	graphics.FillPath(&SolidBrush(color_letter), &path);
	graphics.DrawPath(&Pen(Color::Black, height * 0.02), &path);
}

// CKoppenIconWnd 메시지 처리기


BOOL CKoppenIconWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}


void CKoppenIconWnd::OnPaint()
{
	//CPaintDC dc(this);
	CBufferDC dc(this);
	Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	CRect rect;
	GetClientRect(&rect);
	DrawIcon(graphics, rect, m_koppen_sign);
}

BOOL CKoppenIconWnd::PreTranslateMessage(MSG* pMsg)
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

	return CWnd::PreTranslateMessage(pMsg);
}