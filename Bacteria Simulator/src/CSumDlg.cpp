// CSumDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Antibiotic Immune Simulator.h"
#include "CSumDlg.h"
#include "afxdialogex.h"

const double sum_ratio = 1076.0 / 609.0;

// CSumDlg 대화 상자

IMPLEMENT_DYNAMIC(CSumDlg, CDialogEx)

CSumDlg::CSumDlg(CWnd* pParent, CGraphDlg* pGraphDlg)
	: CDialogEx(IDD_DIALOG_SUMMARIZE, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pGraphDlg = pGraphDlg;
}

CSumDlg::~CSumDlg()
{
}

BOOL CSumDlg::Create(UINT nIDTemplate, CGraphDlg* pGraphDlg, CWnd* pParentWnd)
{
	m_pGraphDlg = pGraphDlg;

	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

BOOL CSumDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CRect rect;
	GetClientRect(&rect);

	int height = rect.Height();
	int width = rect.Width();

	CSize button(140, 35);

	GetDlgItem(IDC_BUTTON_OK)->MoveWindow(width / 2 + 10, height - 40 - button.cy / 2, button.cx, button.cy);
	GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow(width / 2 - 10 - button.cx, height - 40 - button.cy / 2, button.cx, button.cy);

	return TRUE; 
}

void CSumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSumDlg, CDialogEx)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CSumDlg::OnBnClickedButtonOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSumDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()

void CSumDlg::DrawGraph(Graphics& graphics,  RectF rect, int num, Pen* pen) {
	REAL width = rect.Width;
	REAL height = rect.Height;

	REAL interval = width / num;
	REAL offset = -10;

	// 각종 수치를 담은 변수 선언

	double prev_value = 0;
	if (num > 0) {
		 prev_value= m_pGraphDlg->getVector().at(0);
	}

	int i = 0;
	for (auto value : m_pGraphDlg->getVector()) {
		if (i == 0) {
			i++;
			continue;
		}

		graphics.DrawLine(pen, rect.X + interval * (i - 1) + offset, rect.Y + height * (1 - prev_value), rect.X + interval * i + offset, rect.Y + height * (1 - value));
		prev_value = value;
		i++;
	}
	// prev_value에다가 선분의 앞부분을, value에는 선분의 뒷부분을 저장하고 그래프를 그림

	Color color;
	pen->GetColor(&color);
	Pen BlackPen(Color::Black);
	Pen BigBluePen(color, height * 0.006);
	SolidBrush WhiteBrush(Color::White);
	SolidBrush BlackBrush(Color::Black);
	
	Gdiplus::Font SmallFont(_T("Helvetica"), height * 0.03, FontStyleBold, UnitPixel);
	Gdiplus::StringFormat SmallFormat;
	SmallFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	SmallFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	// 그래프를 그릴 각종 도구 선언

	REAL crosshair_radius = height * 0.01;
	
	double back = 0;
	if (!m_pGraphDlg->getVector().empty()) {
		back = m_pGraphDlg->getVector().back();
	}

	graphics.FillEllipse(&WhiteBrush, rect.X + interval * (i-1) - crosshair_radius + offset, rect.Y + height * (1 - back) - crosshair_radius, crosshair_radius * 2, crosshair_radius * 2);
	graphics.DrawEllipse(&BigBluePen, rect.X + interval * (i-1) - crosshair_radius + offset, rect.Y + height * (1 - back) - crosshair_radius, crosshair_radius * 2, crosshair_radius * 2);
	// 포인터를 그리는 부분

	CString str = _T("");

	if (!m_pGraphDlg->getVector().empty()) {
		str.Format(_T("%0.2f"), m_pGraphDlg->getVector().back());
	}
	graphics.DrawString(str, -1, &SmallFont, PointF(rect.X + interval * (i - 1) + offset, rect.Y + height * (1 - back - 0.03)), &SmallFormat, &BlackBrush);
	// 포인터 위에 현재 수치를 표시하는 함수

	REAL interval_height = height / 40;

	for (int i = 1; i < num + 1; i++) {
		graphics.DrawLine(&BlackPen, rect.X + interval * i + offset, rect.Y + height - interval_height, rect.X + interval * i + offset, rect.Y + height);
	}
	for (int i = 1; i < num; i++) {
		if (i % 5 == 0) {
			str.Format(_T("%d"), i);
			graphics.DrawString(str, -1, &SmallFont, PointF(rect.X + interval * i + offset, rect.Y + height - interval_height * 1.65), &SmallFormat, &BlackBrush);
		}
	}
	// 눈금을 표시하고 5n번째 눈금 위에 5n을 표시하는 함수

	SmallFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	SmallFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);

	graphics.DrawString(_T("100"), -1, &SmallFont, PointF(rect.X + 1, rect.Y + 1), &SmallFormat, &BlackBrush);
	graphics.DrawString(_T("50"), -1, &SmallFont, PointF(rect.X + 1, rect.Y + height / 2 - height * 0.03), &SmallFormat, &BlackBrush);
	graphics.DrawString(_T(" 0"), -1, &SmallFont, PointF(rect.X + 0, rect.Y + height - height * 0.04), &SmallFormat, &BlackBrush);
	// 그래프의 좌측 변에 100, 50, 0을 표시하는 함수
}

// CSumDlg 메시지 처리기

void CSumDlg::OnPaint()
{
	//CPaintDC dc(this); 
	CBufferDC dc(this);
	Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	CRect rect;
	GetClientRect(&rect);

	int height = rect.Height();
	int width = rect.Width();

	m_gRect = RectF(width / 2 - width * 0.4, height / 2 - height * 0.45, width* 0.8, height - height*0.05 - 80);
	// 각종 수치를 저장하는 변수 선언

	Color color(GetSysColor(COLOR_BTNFACE));
	Color color2(color.GetR(), color.GetB(), color.GetG());

	Pen BlackPen(Color::Black);
	Pen BluePen(m_pGraphDlg->getSimulDlg()->GetBlueColor(), m_gRect.Height * 0.004);
	SolidBrush WhiteBrush(Color::White);
	SolidBrush GrayBrush(color2);
	// 각종 그림도구를 선언

	graphics.FillRectangle(&GrayBrush, RectF(0, 0, width, height));

	graphics.FillRectangle(&WhiteBrush, m_gRect);
	graphics.DrawRectangle(&BlackPen, m_gRect);
	// 그래프를 그릴 바탕을 그림

	DrawGraph(graphics, m_gRect, m_pGraphDlg->getVector().size() , &BluePen);
	// 그래프 그리기

	graphics.FillRectangle(&GrayBrush, RectF(1, 1, m_gRect.X - 2, m_gRect.Height));
	graphics.FillRectangle(&GrayBrush, RectF(1, 1, m_gRect.X * 1.5, height * 0.05 - 3));
	// 그래프에서 삐져나오는 선을 감춤
}

void CSumDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	int height = rect.Height();
	int width = rect.Width();

	CSize button(140, 35);

	m_gRect = RectF(width / 2 - width * 0.4, height / 2 - height * 0.45, width * 0.8, height - height * 0.05 - 80);

	if (GetDlgItem(IDC_BUTTON_OK)->GetSafeHwnd() != NULL) {
		GetDlgItem(IDC_BUTTON_OK)->MoveWindow(width / 2 + 10, height - 40 - button.cy / 2, button.cx, button.cy);
		GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow(width / 2 - 10 - button.cx, height - 40 - button.cy / 2, button.cx, button.cy);
	}

	RedrawWindow();
}
void CSumDlg::OnSizing(UINT fwSide, LPRECT pRect) // 이 메세지 핸들러 함수는 유저가 윈도우의 창 크기를 조절할 때 호출되며, 창의 비율 같이 창의 크기를 미리 조정할 수 있게 함
{
	CDialogEx::OnSizing(fwSide, pRect);
	int height = pRect->bottom - pRect->top;
	int width = pRect->right - pRect->left;
	int offset_x = (height * sum_ratio - width) / 2;
	int offset_y = (width / sum_ratio - height) / 2;
	// 특정 비율을 유지하도록 함, offset_?? 변수는 사용자가 x,y축으로 크기를 변경 시 그에 상응하는 축의 변경될 크기를 저장함

	switch (fwSide) {
	case WMSZ_TOP:
	case WMSZ_BOTTOM:
		pRect->left -= offset_x;
		pRect->right += offset_x;
		break;
	case WMSZ_LEFT:
	case WMSZ_RIGHT:
		pRect->top -= offset_y;
		pRect->bottom += offset_y;
		break;

	case WMSZ_TOPLEFT:
		pRect->top -= offset_y;
		pRect->left -= offset_x;
		break;
	case WMSZ_TOPRIGHT:
		pRect->top -= offset_y;
		pRect->right += offset_x;
		break;
	case WMSZ_BOTTOMLEFT:
		pRect->bottom += offset_y;
		pRect->left -= offset_x;
		break;
	case WMSZ_BOTTOMRIGHT:
		pRect->bottom += offset_y;
		pRect->right += offset_x;
		break;
	} // 각 방향마다 offset을 더해주거나 뺴줌
}
void CSumDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1092;
	lpMMI->ptMinTrackSize.y = 648;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CSumDlg::OnBnClickedButtonOk()
{
	OnOK();
}
// 확인 버튼을 누르면 나감

void CSumDlg::OnBnClickedButtonClear()
{
	m_pGraphDlg->getVector().clear();
	m_pGraphDlg->setOffset(0);

	AfxMessageBox(_T("박테리아 시뮬레이터의 기록을 초기화하였습니다."));
}
// 기록을 초기화하는 함수

BOOL CSumDlg::PreTranslateMessage(MSG* pMsg)
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