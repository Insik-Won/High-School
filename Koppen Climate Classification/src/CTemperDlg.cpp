// CTemperDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "CTemperDlg.h"
#include "afxdialogex.h"


// CTemperDlg 대화 상자

IMPLEMENT_DYNAMIC(CTemperDlg, CDialogEx)

CTemperDlg::CTemperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GRAPH_TMP, pParent)
	, m_Pointer(this)
	, m_Temper_Graph(11, MrGraphMode::Lines)
	, m_pointer_offset(0)
	, m_dragging_index(-1)
{

}

CTemperDlg::~CTemperDlg()
{
}

void CTemperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTemperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CTemperDlg::WriteTitleAsBool(bool flag) {
	DWORD style = GetStyle();
	if (style & WS_CHILD) {
		if (flag == true)	SetWindowText(_T("true"));
		if (flag == false)	SetWindowText(_T("false"));
	}
}


// CTemperDlg 메시지 처리기

BOOL CTemperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MrDirection tmp1 = MrDirection::Left;
	MrInOut tmp2 = MrInOut::Outer;

	m_Temper_Graph.addMark(1.0, _T("100%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.9, _T("90%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.8, _T("80%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.7, _T("70%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.6, _T("60%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.5, _T("50%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.4, _T("40%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.3, _T("30%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.2, _T("20%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.1, _T("10%"), tmp1, tmp2);
	m_Temper_Graph.addMark(0.0, _T("0%"), tmp1, tmp2);

	m_Temper_Graph.setGraphLimits(0.0f, 100.0f);
	m_Temper_Graph.setGraphMode(MrGraphMode::Lines);
	m_Temper_Graph.setFontHeight(0.023);

	for (int i = 0; i < 11; i++) {
		m_Temper_Graph.setData(i*10, i);
	}

	WriteTitleAsBool(true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTemperDlg::OnPaint()
{
	CPaintDC dc(GetParent());
	//CBufferDC dc(GetParent());

	Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	CRect Rect;
	GetClientRect(&Rect);

	int width = Rect.Width();
	int height = Rect.Height();

	SolidBrush WhiteBrush(Color::White);
	SolidBrush CarnageBrush(Color::Crimson);

	Rect.DeflateRect(70, 70);
	m_Temper_Graph.setRect(&Rect);
	m_Temper_Graph.drawGraph(graphics);

	graphics.DrawRectangle(&Pen(Color::Black), Rect.left, Rect.top, Rect.Width(), Rect.Height());

	graphics.DrawRectangle(&Pen(Color::Crimson), 0, 0, width -1, height -1);
}


BOOL CTemperDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CTemperDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Pointer.ClickDown();

	auto [i, target_point] = m_Graph.isAnyIndexClicked(point, CSize(20, 20));

	if (i >= 0) {
		m_dragging_index = i;
		m_pointer_offset = target_point.y - point.y;
	}
	else {
		m_dragging_index = i;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTemperDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Pointer.ClickUp();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CTemperDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Pointer.isDragging() && m_dragging_index >= 0) {
		int actual_point = point.y + m_pointer_offset;

		float data = m_Temper_Graph.convertData(point.y);		
		m_Temper_Graph.setData(data, m_dragging_index);

		CRect UpdateRect = m_Temper_Graph.getUpdateRect(m_dragging_index);
		RedrawWindow(UpdateRect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}



