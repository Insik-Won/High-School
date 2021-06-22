// CSimulDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Antibiotic Immune Simulator.h"
#include "CSimulDlg.h"
#include "CMainDlg.h"
#include "afxdialogex.h"

// CSimulDlg 대화 상자

IMPLEMENT_DYNAMIC(CSimulDlg, CDialogEx)

CSimulDlg::CSimulDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SIMUL, pParent)
{
	srand(time(NULL));
	num = 7;
	m_BlueColor = MrColor::Ambient_Blue;
	m_bMutateFlag = false;
	m_bSpreadFlag = false;
	m_Spread_Radius = 0;
	m_Spread_Speed = 1.2;

} // 각종 초기화

CSimulDlg::~CSimulDlg()
{
	for (int i = 0; i < num; i++) {
		delete[] m_bacterias[i];
	}

	delete[] m_bacterias;
} // CBacteria 2차원 동적 배열 해제

BOOL CSimulDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	int height = rect.Height();
	int width = rect.Width();

	LOGFONT lf;
	GetDlgItem(IDC_BUTTON_RESTART)->GetFont()->GetLogFont(&lf);
	lf.lfHeight = height * 0.01;
	lf.lfWeight = FW_BOLD;

	CFont font;
	font.DeleteObject();
	font.CreateFontIndirectW(&lf);

	GetDlgItem(IDC_BUTTON_RESTART)->SetFont(&font);

	num = AfxGetApp()->GetProfileIntW(_T("Option"), _T("num"), 7);
	m_Spread_Speed = AfxGetApp()->GetProfileIntW(_T("Option"), _T("speed"), 120) / 100.0;

	m_bacterias = new CBacteria * [num];
	for (int i = 0; i < num; i++) {
		m_bacterias[i] = new CBacteria[num];
	}

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			m_bacterias[i][j].fatality(1);
			m_bacterias[i][j].initiate_Health();
			m_bacterias[i][j].SetMutate(m_bMutateFlag);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSimulDlg::PostNcDestroy()
{
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("num"), num);
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("speed"), m_Spread_Speed * 100);
	CDialogEx::PostNcDestroy();
}


void CSimulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSimulDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SIMUL_SPREAD, &CSimulDlg::OnSpreadAntibiotic)
	ON_MESSAGE(WM_SIMUL_MUTATE, &CSimulDlg::OnMutateChange)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CSimulDlg::OnBnClickedButtonRestart)
END_MESSAGE_MAP()

// CSimulDlg 멤버 함수

void CSimulDlg::DrawBacteria(Graphics& graphic, REAL x, REAL y, REAL radius) {
	SolidBrush BlueBrush(m_BlueColor);
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	Pen	BluePen(m_BlueColor, radius * 0.1);

	REAL small_radius = radius / 7.0;
	REAL big_radius = 5.0 * small_radius;
	REAL bias_radius = radius * sqrt(2) / 2;

	PointF points[8];
	points[0] = { x, y - radius };
	points[1] = { x + bias_radius,y - bias_radius };
	points[2] = { x + radius, y };
	points[3] = { x + bias_radius, y + bias_radius };
	points[4] = { x, y + radius };
	points[5] = { x - bias_radius, y + bias_radius };
	points[6] = { x - radius, y };
	points[7] = { x - bias_radius, y - bias_radius };

	for (int i = 0; i < 4; i++) graphic.DrawLine(&BluePen, points[i], points[i + 4]);
	for (int i = 0; i < 8; i++) graphic.FillEllipse(&BlueBrush, points[i].X - small_radius, points[i].Y - small_radius, small_radius * 2, small_radius * 2);

	BluePen.SetWidth(radius*0.15);

	graphic.FillEllipse(&WhiteBrush, x - big_radius, y - big_radius, big_radius * 2, big_radius * 2);
	graphic.DrawEllipse(&BluePen, x - big_radius, y - big_radius, big_radius * 2, big_radius * 2);
} // 박테리아를 그리는 함수

void CSimulDlg::ReproduceBacteria(CBacteria** bacterias, int x, int y) {
	if (m_bacterias[x][y].isDead()) return;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			if ((x + i < 0 || x + i >= num) || (y + j < 0 || y + j >= num)) {
				continue;
			}
			if (bacterias[x+i][y+j].isDead()) {
				if (rand() < RAND_MAX/8)
					m_bacterias[x][y].reproduce(bacterias[x + i][y + j]);
			}
		}
	}
} // 박테리아를 번식시키는 함수, 임시 버퍼가 없으면 번식한 세균이 또 번식하는 문제가 생김

double CSimulDlg::average_fatality() {
	double sum_f = 0, sum_d = 0;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			if (m_bacterias[i][j].isAlive()) {
				sum_f += m_bacterias[i][j].fatality();
				sum_d += 1;
			}
		}
	}

	if (sum_f == 0 || sum_d == 0) return 0;
	return sum_f / sum_d;
} // 치사율의 평균을 구하는 함수

MrColor CSimulDlg::GetBlueColor() {
	return m_BlueColor;
}

void CSimulDlg::Restart() {
	num = 7;
	m_Spread_Speed = 1.2;
	SetGridNumber(num);
}

void CSimulDlg::SetGridNumber(int inum) {
	for (int i = 0; i < num; i++) {
		delete[] m_bacterias[i];
	}
	delete[] m_bacterias;

	num = inum;

	m_bacterias = new CBacteria * [num];
	for (int i = 0; i < num; i++) {
		m_bacterias[i] = new CBacteria[num];
	}

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			m_bacterias[i][j].SetMutate(m_bMutateFlag);
			if (m_bMutateFlag == true) {
				double num = (double)rand() / RAND_MAX;
				m_bacterias[i][j].fatality(1 - num / 2);
			}
			else {
				m_bacterias[i][j].fatality(1);
			}
			m_bacterias[i][j].initiate_Health();
		}
	}
}

int CSimulDlg::GetGridNumber() {
	return num;
}

void CSimulDlg::SetSpreadSpeed(double inum) {
	m_Spread_Speed = inum;
}
REAL CSimulDlg::GetSpreadSpeed() {
	return m_Spread_Speed;
}

// CSimulDlg 메시지 처리기

void CSimulDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	int height = rect.Height();
	int width = rect.Width();

	CSize button = { int(width * 0.13), int(height * 0.07) };

	if (GetDlgItem(IDC_BUTTON_RESTART)->GetSafeHwnd() != NULL) {
		GetDlgItem(IDC_BUTTON_RESTART)->MoveWindow(width / 2 - button.cx / 2, height / 2 - button.cy / 2, button.cx, button.cy);
	}
}

void CSimulDlg::OnPaint()
{
	//CPaintDC dc(this); 
	CBufferDC dc(this);

	Graphics graphics(dc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(rect);

	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	SolidBrush BlackBrush(Color::Black);
	Pen BlackPen(Color(220, 0, 0, 0));
	Pen GreenPen(Color(255, 0, 200, 0), 2.0F);
	REAL Dashes[2] = {2.0F, 3.0F};
	BlackPen.SetDashPattern(Dashes, 2);
	// 필요한 각종 GDI+ 도구 선언

	REAL interval = rect.Width() / (float)num;
	// 한 칸의 간격 계산

	CImage Image;
	Image.Create(interval -1, interval-1, 24);
	Graphics bmp_graphics(Image.GetDC());
	bmp_graphics.SetSmoothingMode(SmoothingModeHighQuality);

	bmp_graphics.FillRectangle(&WhiteBrush, -30, -30, Image.GetWidth() + 50, Image.GetHeight()+50);
	DrawBacteria(bmp_graphics, interval / 2, interval / 2, (interval * 0.70)/2);

	Image.ReleaseDC();
	// CImage 객체를 만들고 그 속에 박테리아를 그려넣음

	Gdiplus::Font font(_T("Helvetica"), interval/9, FontStyleBold, UnitPixel);
	StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	// 세균의 치사율을 적을 폰트 선언

	CString data;

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			if (m_bacterias[i][j].isAlive()) {
				Image.BitBlt(dc.m_hDC, interval * i + 1, interval * j + 1);
				data.Format(_T("%0.1f%%"), m_bacterias[i][j].fatality() * 100);
				//data.Format(_T("%0.1f"), m_bacterias[i][j].health());
				graphics.DrawString(data, -1, &font, RectF(interval*i+2, interval*j, interval, interval), &format, &BlackBrush);
				graphics.DrawLine(&GreenPen, interval*i+1+(interval / 2 - interval * 0.18 * m_bacterias[i][j].health() / 100), interval * j + 1 + (interval / 2 + interval / 12), interval * i + 1 + (interval / 2 + interval * 0.18 * m_bacterias[i][j].health() / 100), interval * j + 1 + (interval / 2 + interval / 12));
			}
		}
	} // CImage 객체 속에 있는 박테리아의 이미지를 각 칸마다 복사한 후, 치사율을 적고 그 밑에 초록색 체력 게이지를 표시함. 

	for (int i = 0; i < num; i++) {
		graphics.DrawLine(&BlackPen, interval * i, 0.0, interval * i, static_cast<REAL>(rect.Width()));
		graphics.DrawLine(&BlackPen, 0.0, interval * i, static_cast<REAL>(rect.Width()), interval * i);
	} // 점선으로 가로세로 격자칸 그리기

	BlackPen.SetDashStyle(DashStyleSolid);
	graphics.DrawRectangle(&BlackPen, 0, 0, rect.Width() - 1, rect.Height() - 1);
	// 검정색 외곽 테두리 그리기

	if (m_bSpreadFlag == true) { // 항생제 살포 시
		int radius = rect.Width() * m_Spread_Radius / 100.0;
		Pen RedPen(MrColor::Laser_Red, 10.0F);
		graphics.DrawEllipse(&RedPen, -radius, -radius, radius * 2, radius * 2);
		// 항생제가 살포되는 부채꼴 그리기

		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				for (int k = -5; k <= 5; k++) {
						COLORREF color = dc.GetPixel(interval * i + interval / 2 + k, interval * j + interval / 2);
						if (GetRValue(color) > 200 && GetGValue(color) < 100 && GetBValue(color) < 100) {
							m_bacterias[i][j].try_Kill();
							goto GETOUT;
						}
				}
				GETOUT:;
			}
		} // 각 칸의 중앙점의 색상을 구해서 그 색상이 빨간색류이면 그 칸에 속한 세균 살균 시도
	}
}


LRESULT CSimulDlg::OnSpreadAntibiotic(WPARAM wParam, LPARAM lParam) {
	m_bSpreadFlag = true;
	return 0;
} // CMainDlg의 항생제 살포 버튼 클릭 시


// 타이머 설정 부분

int CSimulDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (SetTimer(TIMER_SM_SPREAD, 20, NULL) != TIMER_SM_SPREAD) {
		AfxMessageBox(_T("에러: 항생제 시뮬레이터의 항생제 살포용 타이머 설정에 실패하였습니다."));
	}	

	if (SetTimer(TIMER_SM_LIVING, 500, NULL) != TIMER_SM_LIVING) {
		AfxMessageBox(_T("에러: 항생제 시뮬레이터의 세균의 생리작용 타이머 설정에 실패하였습니다."));
	}
	// 각종 타이머 설정

	return 0;
}


void CSimulDlg::OnClose()
{
	KillTimer(TIMER_SM_SPREAD);
	KillTimer(TIMER_SM_LIVING);

	CDialogEx::OnClose();
} // 타이머 해제


void CSimulDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_SM_SPREAD) { // 항생제 살포 타이머 작동 시
		if (m_bSpreadFlag == true) {
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			if (m_Spread_Radius > 100 * sqrt(2)) {
				m_bSpreadFlag = false;
				m_Spread_Radius = 0;
			}
			m_Spread_Radius += m_Spread_Speed;
		} // 윈도우 창을 다시 그리고 항생제가 살포되는 부채꼴의 반경 확대
	} 
	else if (nIDEvent == TIMER_SM_LIVING) { // 세균의 생리작용 타이머 작동 시
		if (m_bSpreadFlag == false) {
			CBacteria** temp_bacterias = new CBacteria* [num];
			// 임시 버퍼 생성, 안할 시 번식한 세균이 또 번식하는 불상사가 일어남

			for (int i = 0; i < num; i++) {
				temp_bacterias[i] = new CBacteria[num];
				for (int j = 0; j < num; j++) {
					temp_bacterias[i][j] = m_bacterias[i][j];
				}
			} // 임시 버퍼의 2차 생성 및 초기화

			for (int i = 0; i < num; i++) {
				for (int j = 0; j < num; j++) {
					ReproduceBacteria(temp_bacterias, i, j);
				}
			} // 번식 알고리즘 작동

			for (int i = 0; i < num; i++) {
				for (int j = 0; j < num; j++) {
					m_bacterias[i][j] = temp_bacterias[i][j];
				}
				delete[] temp_bacterias[i];
			}// 임시 버퍼를 다시 본 버퍼로 옮기고 해제

			delete[] temp_bacterias;
			// 임시 버퍼의 2차 해제

			for (int i = 0; i < num; i++) {
				for (int j = 0; j < num; j++) {
					m_bacterias[i][j].living(-1);
					if (m_bacterias[i][j].health() < 0) {
						m_bacterias[i][j].Kill();
					}
				}
			} // 세균의 수명을 깎고 다 깎으면 죽임

			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			// 윈도우 다시 그리기

			bool b_AllDiedFlag = true;

			for (int i = 0; i < num; i++) {
				for (int j = 0; j < num; j++) {
					if (m_bacterias[i][j].isAlive()) {
						b_AllDiedFlag = false;
						goto GETOUT;
					}
				}
			}
			GETOUT:

			if (b_AllDiedFlag == true) {
				GetDlgItem(IDC_BUTTON_RESTART)->ShowWindow(SW_SHOW);
			}

			// 다 죽으면 재시작 버튼을 띄움
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CSimulDlg::OnMutateChange(WPARAM wParam, LPARAM lParam) {
	m_bMutateFlag = wParam;
	m_BlueColor = (m_bMutateFlag) ? MrColor::Purple_Blue : MrColor::Ambient_Blue;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			m_bacterias[i][j].SetMutate(m_bMutateFlag);
		}
	}

	if (m_bMutateFlag == true) {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				double num = (double)rand() / RAND_MAX;
				m_bacterias[i][j].fatality(1-num/2);
				m_bacterias[i][j].initiate_Health();
			}
		}
	}
	return 0;
}
// 변이를 시작/중지시키는 함수


void CSimulDlg::OnBnClickedButtonRestart()
{
	CMainDlg* pMainDlg = reinterpret_cast<CMainDlg*>(GetParent());
	pMainDlg->Restart();
	GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
}
// 초기화 함수


BOOL CSimulDlg::PreTranslateMessage(MSG* pMsg)
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
// Enter/Esc키 무효화 함수