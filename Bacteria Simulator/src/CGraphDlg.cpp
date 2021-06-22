// CGraphDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Antibiotic Immune Simulator.h"
#include "CGraphDlg.h"
#include "CMainDlg.h"
#include "afxdialogex.h"


// CGraphDlg 대화 상자

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GRAPH, pParent)
	, m_vector()
	, m_offset(0)
{
	num = 10;
}

BOOL CGraphDlg::Create(UINT nIDTemplate, CSimulDlg* pSimulDlg, CWnd* pParentWnd)
{
	m_pSimulDlg = pSimulDlg;

	return CDialogEx::Create(nIDTemplate, pParentWnd);
}


CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

vector<double>& CGraphDlg::getVector() {
	return m_vector;
}

CSimulDlg* CGraphDlg::getSimulDlg() {
	return m_pSimulDlg;
}

int CGraphDlg::getOffset() {
	return m_offset;
}
void CGraphDlg::setOffset(int offset) {
	m_offset = offset;
}

void CGraphDlg::Restart() {
	m_vector.clear();
	m_offset = 0;
}

// CGraphDlg 메시지 처리기


void CGraphDlg::OnPaint()
{
	//CPaintDC dc(this); 
	CBufferDC dc(this);
	Graphics graphics(dc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(&rect);

	REAL width = rect.Width();
	REAL height = rect.Height();

	Pen BlackPen(Color::Black);
	Pen BluePen(m_pSimulDlg->GetBlueColor() , height * 0.004);
	SolidBrush BlackBrush(Color(255,10,10,10));
	Pen BigBluePen(m_pSimulDlg->GetBlueColor(), height*0.006);
	SolidBrush WhiteBrush(Color::White);

	REAL interval = (REAL)rect.Width() / num;
	REAL interval_height = (REAL)rect.Height() / 40;
	REAL offset = -10;
	// 기본적인 수치 및 그래픽 도구 선언

	for (int i = 1; i < num+1; i++) {
		graphics.DrawLine(&BlackPen, interval * i + offset, height - interval_height, interval * i + offset, height);
	}
	// 첫번쨰로 눈금을 그림

	CString str = _T("");

	Gdiplus::Font font(_T("맑은 고딕"), height*0.03, FontStyleBold, UnitPixel);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	Gdiplus::Font SmallFont(_T("맑은 고딕"), height * 0.03, FontStyleBold, UnitPixel);
	Gdiplus::StringFormat SmallFormat;
	SmallFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	SmallFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	// 표시할 숫자를 그릴 폰트 선언

	PointF points[11];
	int i = 0;
	for (auto pos = cbegin(m_vector) + m_offset; pos < cend(m_vector); ++pos) {
		points[i] = PointF(interval * i + offset, height * (1 - *pos));
		i++;
	}
	// PointF 배열에다가 각 눈금에 표시할 데이터를 집어넣음

	REAL crosshair_radius = height * 0.01;
	graphics.DrawLines(&BluePen, points, i);
	graphics.FillEllipse(&WhiteBrush, points[i - 1].X - crosshair_radius, points[i - 1].Y - crosshair_radius, crosshair_radius * 2, crosshair_radius * 2);
	graphics.DrawEllipse(&BigBluePen, points[i - 1].X - crosshair_radius, points[i - 1].Y - crosshair_radius, crosshair_radius * 2, crosshair_radius * 2);
	// 데이터를 저장할 PointF 배열로 그래프를 그린 뒤 포인터도 그림

	str = _T("");
	if (!m_vector.empty()) {
		str.Format(_T("%0.2f"), m_vector.back());
	}
	graphics.DrawString(str, -1, &SmallFont, PointF(points[i - 1].X, points[i - 1].Y - height * 0.03), &SmallFormat, &BlackBrush);
	// 포인터 위에다가 현재 수치를 표시함

	for (int i = 0; i < 11; i++) {
		if ((m_offset + i) % 5 == 0) {
			str.Format(_T("%d"), m_offset + i);
			graphics.DrawString(str, -1, &font, PointF(interval*i +offset, height - interval_height*1.65), &format, &BlackBrush);
		}
	}
	// 5n번째 눈금마다 5n을 표시함

	SmallFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	SmallFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);

	graphics.DrawString(_T("100"), -1, &SmallFont, PointF(1, 1),  &SmallFormat, &BlackBrush);
	graphics.DrawString(_T("50"), -1, &SmallFont, PointF(1, height / 2 - height*0.03), &SmallFormat, &BlackBrush);
	graphics.DrawString(_T(" 0"), -1, &SmallFont, PointF(0, height - height * 0.04), &SmallFormat, &BlackBrush);

	graphics.DrawLine(&BlackPen, 0.0, height-1, width, height-1);
	// 좌측 변에 0,50,100을 표시함
}

int CGraphDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (SetTimer(TIMER_GP_RECORD, 1000, NULL) != TIMER_GP_RECORD) {
		AfxMessageBox(_T("에러: 그래프 기록용 타이머 설정에 실패하였습니다."));
	}

	return 0;
}
// 데이터 기록용 타이머 설정

void CGraphDlg::OnClose()
{
	KillTimer(TIMER_GP_RECORD);

	CDialogEx::OnClose();
}
// 타이머 해제


void CGraphDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_GP_RECORD) {
		double average = m_pSimulDlg->average_fatality();
		m_vector.push_back(average);
		

		if (m_vector.size() > 10) {
			m_offset++;
		}

		CMainDlg* pMainDlg = reinterpret_cast<CMainDlg*>(m_pSimulDlg->GetParent());
		pMainDlg->GetSumDlg()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	CDialogEx::OnTimer(nIDEvent);
}
// 타이머 발동 시, 데이터를 벡터에 넣고 현재 현황 창에도 다시 그래프를 그리라는 명령을 함

BOOL CGraphDlg::PreTranslateMessage(MSG* pMsg)
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

	return CDialogEx::PreTranslateMessage(pMsg);
}
// Enter/Esc 키 무효화
