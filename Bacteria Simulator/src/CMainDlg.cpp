// Antibiotic Immune SimulatorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Antibiotic Immune Simulator.h"
#include "CMainDlg.h"
#include "afxdialogex.h"

#include "CSumDlg.h"
#include "COptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double ratio = 1136.0 / 677.0; // 창의 비율

// CMainDlg 대화 상자

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	srand(time(NULL));

	m_bMutateFlag = FALSE;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SPREAD_ANTIBIOTIC, &CMainDlg::OnBnClickedButtonSpreadAntibiotic)
	ON_WM_SIZING()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_MUTATE, &CMainDlg::OnBnClickedButtonMutate)
	ON_BN_CLICKED(IDC_BUTTON_NOWSTATE, &CMainDlg::OnBnClickedButtonNowstate)
	ON_COMMAND(ID_MENU_OPTION, &CMainDlg::OnMenuOption)
	ON_COMMAND(ID_MENU_HELP, &CMainDlg::OnMenuHelp)
END_MESSAGE_MAP()

CSumDlg* CMainDlg::GetSumDlg() {
	return &m_SumDlg;
}

void CMainDlg::Restart() {
	CBacteria::mutation_rate = 0.2;

	m_SimulDlg.Restart();
	m_GraphDlg.Restart();
}

// CMainDlg 메시지 처리기

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CRect Rect;
	GetClientRect(&Rect);

	int height = Rect.Height();
	int width = Rect.Width();
	int gap = 20;
	// 각종 수치 변수 설정

	CSize button = { 70,65 };
	button.cx = (width - height - gap * 2) / 2;
	// 버튼의 각종 수치 변수 설정

	m_SimulDlg.Create(IDD_DIALOG_SIMUL, this);
	m_GraphDlg.Create(IDD_DIALOG_GRAPH, &m_SimulDlg, this);
	m_SumDlg.Create(IDD_DIALOG_SUMMARIZE, &m_GraphDlg, GetDesktopWindow());

	CPoint simul_point = { (width - (button.cx * 2 + gap * 4)) / 2 + gap, (height - gap * 2)/2 + gap }; // 세균 시뮬레이터 창의 중앙점
	CPoint graph_point = { width - (button.cx * 2 + gap * 3) / 2, (height - (button.cy * 2 + gap * 3)) / 2 }; // 그래프 창의 중앙점

	int simul_radius = min(simul_point.x, simul_point.y); // 세균 시뮬레이터의 반경 설정
	int graph_radius = min(width - graph_point.x, graph_point.y); // 그래프의 반경 설정 
	simul_radius -= gap; // 간격만큼 반경 빼기
	graph_radius -= gap; // ""

	int offset = (width - (simul_radius * 2 + graph_radius * 2 + gap)) / 2;
	offset -= 2;
	// 세균 시뮬레이터와 그래프를 간격 하나를 두고 붙이도록 설정

	m_SimulDlg.MoveWindow(offset, simul_point.y - simul_radius, simul_radius * 2, simul_radius * 2);
	m_GraphDlg.MoveWindow(offset + simul_radius * 2 + gap, graph_point.y - graph_radius, graph_radius * 2, graph_radius * 2);
	// 세균 시뮬레이터와 그래프 이동

	m_SimulDlg.RedrawWindow();
	m_GraphDlg.RedrawWindow();
	// 이동했으니 다시 그리기

	CRect Rect2;
	m_GraphDlg.GetWindowRect(&Rect2);
	ScreenToClient(&Rect2);
	// 그래프의 직사각형 구하기 (그 밑에다가 버튼들을 이동시킴)

	button.cx = (Rect2.Width() - gap) / 2;
	// 버튼의 가로 길이 재설정

	GetDlgItem(IDC_BUTTON_MUTATE)->MoveWindow(Rect2.left, Rect2.bottom + gap * 2, button.cx, button.cy);
	GetDlgItem(IDC_BUTTON_NOWSTATE)->MoveWindow(Rect2.left + button.cx + gap, Rect2.bottom + gap * 2, button.cx, button.cy);
	GetDlgItem(IDC_BUTTON_SPREAD_ANTIBIOTIC)->MoveWindow(Rect2.left, Rect2.bottom + button.cy + gap * 3, Rect2.Width(), button.cy);
	// 버튼들 이동시키기

	LOGFONT lf;
	GetDlgItem(IDC_BUTTON_MUTATE)->GetFont()->GetLogFont(&lf);
	lf.lfHeight = height * 0.03;
	lf.lfWeight = FW_BOLD;

	CFont font;
	font.DeleteObject();
	font.CreateFontIndirectW(&lf);

	GetDlgItem(IDC_BUTTON_MUTATE)->SetFont(&font);
	GetDlgItem(IDC_BUTTON_NOWSTATE)->SetFont(&font);
	GetDlgItem(IDC_BUTTON_SPREAD_ANTIBIOTIC)->SetFont(&font);

	CBacteria::mutation_rate = AfxGetApp()->GetProfileIntW(_T("Option"), _T("rate"), 20) / 100.0;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMainDlg::PostNcDestroy()
{
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("rate"), CBacteria::mutation_rate * 100);

	CDialogEx::PostNcDestroy();
}


void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);
	Rect.right = Rect.left + cx;
	Rect.bottom = Rect.top + cy;

	int height = Rect.Height();
	int width = Rect.Width();
	int gap = 20;
	// 각종 수치 변수 설정

	CSize button = { 70,65 };
	button.cx = max(70, (width - height - gap * 2) / 2);
	if (nType == SIZE_MAXIMIZED) {
		gap = 30;
		button.cy = 100;
	} // 버튼의 각종 수치 설정

	CPoint simul_point = { (width - (button.cx * 2 + gap * 4)) / 2 + gap, (height - gap * 2) / 2 + gap }; // 중앙점
	CPoint graph_point = { width - (button.cx * 2 + gap * 3) / 2, (height - (button.cy * 2 + gap * 3)) / 2 }; // 중앙점

	int simul_radius = min(simul_point.x, simul_point.y); // 세균 시뮬레이터의 반경 설정
	int graph_radius = min(width - graph_point.x, graph_point.y); // 그래프의 반경 설정 
	simul_radius -= gap; // 간격만큼 반경 빼기
	graph_radius -= gap; // ""	
		
	if (m_SimulDlg.GetSafeHwnd() != NULL) { // OnSize()는 InitDialog() 전에 호출됨
		int offset = (width - (simul_radius * 2 + graph_radius * 2 + gap)) / 2;
		offset -= 2;
		// 세균 시뮬레이터와 그래프를 간격 하나를 두고 붙이도록 설정

		m_SimulDlg.MoveWindow(offset, simul_point.y - simul_radius, simul_radius * 2, simul_radius * 2);
		m_GraphDlg.MoveWindow(offset + simul_radius * 2 + gap, graph_point.y - graph_radius, graph_radius * 2, graph_radius * 2);
		// 세균 시뮬레이터와 그래프 이동

		m_SimulDlg.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		m_GraphDlg.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		// 이동했으니 다시 그리기

		CRect Rect2;
		m_GraphDlg.GetWindowRect(&Rect2);
		ScreenToClient(&Rect2);
		// 그래프의 직사각형 구하기 (그 밑에다가 버튼들을 이동시킴)

		button.cx = (Rect2.Width() - gap) / 2;
		// 버튼의 가로 길이 재설정

		GetDlgItem(IDC_BUTTON_MUTATE)->MoveWindow(Rect2.left, Rect2.bottom + gap * 2, button.cx, button.cy);
		GetDlgItem(IDC_BUTTON_NOWSTATE)->MoveWindow(Rect2.left + button.cx + gap, Rect2.bottom + gap * 2, button.cx, button.cy);
		GetDlgItem(IDC_BUTTON_SPREAD_ANTIBIOTIC)->MoveWindow(Rect2.left, Rect2.bottom + button.cy + gap * 3, Rect2.Width(), button.cy);
		// 버튼들 이동시키기

		LOGFONT lf;
		GetDlgItem(IDC_BUTTON_MUTATE)->GetFont()->GetLogFont(&lf);
		lf.lfHeight = height * 0.03;
		lf.lfWeight = FW_BOLD;
		// 폰트 크기 조절 (안됨...)

		CFont font;
		font.DeleteObject();
		font.CreateFontIndirectW(&lf);

		GetDlgItem(IDC_BUTTON_MUTATE)->SetFont(&font);
		GetDlgItem(IDC_BUTTON_NOWSTATE)->SetFont(&font);
		GetDlgItem(IDC_BUTTON_SPREAD_ANTIBIOTIC)->SetFont(&font);
	}
	
}
void CMainDlg::OnSizing(UINT fwSide, LPRECT pRect) // 이 메세지 핸들러 함수는 유저가 윈도우의 창 크기를 조절할 때 호출되며, 창의 비율 같이 창의 크기를 미리 조정할 수 있게 함
{
	CDialogEx::OnSizing(fwSide, pRect);
	int height = pRect->bottom - pRect->top;
	int width = pRect->right - pRect->left;
	int offset_x = (height * ratio - width) / 2;
	int offset_y = (width / ratio - height) / 2;
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
void CMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1146;
	lpMMI->ptMinTrackSize.y = 707;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
} // 창의 최대/최소 크기를 운영체제가 구할 때 호출하는 함수, 다만 그 크기가 정확하지 않아 따로 설정함.

void CMainDlg::OnBnClickedButtonSpreadAntibiotic()
{
	m_SimulDlg.PostMessageW(WM_SIMUL_SPREAD, 0, 0);
} 
// 항생제 살포 버튼 클릭 시 세균 시뮬레이터에 항생제를 살포하라는 메시지를 올림.


void CMainDlg::OnBnClickedButtonMutate()
{
	if (m_bMutateFlag == TRUE) {
		GetDlgItem(IDC_BUTTON_MUTATE)->SetWindowTextW(_T("변이 시작"));
	}
	else {
		GetDlgItem(IDC_BUTTON_MUTATE)->SetWindowTextW(_T("변이 중지"));
	}

	m_bMutateFlag = !m_bMutateFlag;
	m_SimulDlg.PostMessageW(WM_SIMUL_MUTATE, m_bMutateFlag, 0);
} 
// 변이 버튼 클릭 시 세균 시뮬레이터에 변이를 시작/중단하라는 메세지를 올림


void CMainDlg::OnBnClickedButtonNowstate()
{
	CRect ParentRect, ChildRect;
	GetWindowRect(&ParentRect);

	m_SumDlg.GetWindowRect(&ChildRect);

	CPoint ParentPoint = { ParentRect.left + ParentRect.Width() / 2, ParentRect.top + ParentRect.Height()/2 };
	ChildRect = CRect(ParentPoint.x - ChildRect.Width() / 2, ParentPoint.y - ChildRect.Height() / 2, ParentPoint.x + ChildRect.Width() / 2, ParentPoint.y + ChildRect.Height() / 2);

	m_SumDlg.SetWindowPos(NULL, ChildRect.left, ChildRect.top, ChildRect.Width(), ChildRect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
}
// 현재 현황 버튼 클릭 시 현재 현황 요약 창을 보여줌

void CMainDlg::OnMenuOption()
{
	static COptionDlg OptionDlg;

	if (OptionDlg.GetSafeHwnd() == NULL) {
		OptionDlg.Create(IDD_DIALOG_OPTION, this, this);
	}

	CRect ParentRect, ChildRect;
	GetWindowRect(&ParentRect);

	OptionDlg.GetWindowRect(&ChildRect);

	CPoint ParentPoint = { ParentRect.left + ParentRect.Width() / 2, ParentRect.top + ParentRect.Height() / 2 };
	ChildRect = CRect(ParentPoint.x - ChildRect.Width() / 2, ParentPoint.y - ChildRect.Height() / 2, ParentPoint.x + ChildRect.Width() / 2, ParentPoint.y + ChildRect.Height() / 2);

	OptionDlg.MoveWindow(ChildRect);
	OptionDlg.ShowWindow(SW_SHOW);
}
// 메뉴의 옵션 클릭시 옵션창을 띄움

void CMainDlg::OnMenuHelp()
{
	static CDialogEx HelpDlg;

	if (HelpDlg.GetSafeHwnd() == NULL) {
		HelpDlg.Create(IDD_DIALOG_HELP, this);
		HelpDlg.SetIcon(m_hIcon, TRUE);
		HelpDlg.SetIcon(m_hIcon, FALSE);
	}

	CRect ParentRect, ChildRect;
	GetWindowRect(&ParentRect);

	HelpDlg.GetWindowRect(&ChildRect);

	CPoint ParentPoint = { ParentRect.left + ParentRect.Width() / 2, ParentRect.top + ParentRect.Height() / 2 };
	ChildRect = CRect(ParentPoint.x - ChildRect.Width() / 2, ParentPoint.y - ChildRect.Height() / 2, ParentPoint.x + ChildRect.Width() / 2, ParentPoint.y + ChildRect.Height() / 2);

	HelpDlg.MoveWindow(ChildRect);
	HelpDlg.ShowWindow(SW_SHOW);
}
// 메뉴의 도움말 클릭 시 도움말창을 띄움

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonSpreadAntibiotic();
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
// 엔터/ESC키 무효화