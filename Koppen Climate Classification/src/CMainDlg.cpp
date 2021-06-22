
// Koppen Climate ClassificationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Koppen Climate Classification.h"
#include "CMainDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKoppenClimateClassificationDlg 대화 상자

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_KOPPENCLIMATECLASSIFICATION_DIALOG, pParent)
	, m_KoppenCalculator (m_GeoGraphDlg.getTemperGraph()->getVector(), m_GeoGraphDlg.getPrecipGraph()->getVector())
	, m_OptionDlg(this)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

double CMainDlg::window_ratio = 0;

const int CMainDlg::left_side = 2;
const int CMainDlg::right_side = 1;

const int CMainDlg::info1_side = 2;
const int CMainDlg::info2_side = 1;
const int CMainDlg::info3_side = 2;

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_BUTTON_ONLY_TEMP, m_Button_tempOnly);
	DDX_Control(pDX, IDC_BUTTON_BOTH, m_Button_BothShow);
	DDX_Control(pDX, IDC_BUTTON_ONLY_PRECIP, m_Button_precipOnly);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_USER_UPDATE_CLIMATE_INFO, &CMainDlg::OnUpdateClimateInfo)
	ON_BN_CLICKED(IDC_BUTTON_ONLY_TEMP, &CMainDlg::OnBnClickedButtonOnlyTemp)
	ON_BN_CLICKED(IDC_BUTTON_ONLY_PRECIP, &CMainDlg::OnBnClickedButtonOnlyPrecip)
	ON_BN_CLICKED(IDC_BUTTON_BOTH, &CMainDlg::OnBnClickedButtonBoth)
	ON_COMMAND(ID_MENU_SAVE, &CMainDlg::OnMenuSave)
	ON_COMMAND(ID_MENU_LOAD, &CMainDlg::OnMenuLoad)
	ON_COMMAND(ID_MENU_OPTION, &CMainDlg::OnMenuOption)
	ON_COMMAND(ID_MENU_HELP, &CMainDlg::OnMenuHelp)
	ON_COMMAND(ID_IMAGE_SAVE2, &CMainDlg::OnImageSave2)
	ON_COMMAND(ID_IMAGE_SAVE1, &CMainDlg::OnImageSave1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int CMainDlg::getSplitLine() {
	return getWindowCX(this) * ((double)left_side / (left_side + right_side));
}

CGeoGraphDlg* CMainDlg::getGeoGraphDlg() {
	return &m_GeoGraphDlg;
}

CKoppenIconWnd* CMainDlg::getKoppenIconWnd() {
	return &m_staticInfo2;
}

// CMainDlg 메시지 처리기

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_GeoGraphDlg.Create(IDD_DIALOG_GEOGRAPH, this);
	m_GeoGraphDlg.ShowWindow(SW_SHOW);

	window_ratio = (double) GetSystemMetrics(SM_CXSIZE) / GetSystemMetrics(SM_CYSIZE);

	LOGFONT lf;
	m_Button_BothShow.GetFont()->GetLogFont(&lf);
	wsprintf(lf.lfFaceName, _T("%s"), _T("맑은 고딕"));

	m_button_font.DeleteObject();
	m_button_font.CreateFontIndirectW(&lf);
	m_info_font.DeleteObject();
	m_info_font.CreateFontIndirectW(&lf);

	m_staticInfo1.SetFont(&m_info_font);
	m_staticInfo2.Create(_T("STATIC"), _T("Icon"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, 1212);
	m_staticInfo3.Create(_T("STATIC"), WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 1213);
	m_staticInfo3.SetFont(&m_info_font);

	CreateDirectory(_T("save"), NULL);
	CreateDirectory(_T("image"), NULL);

	UpdateClimateInfos(_T("data\\tag.txt"), _T("data\\explanation.txt"));

	COLORREF buffer;
	Color color;
	COLORREF original_graph_colors[2] = {Color::Black, Color::Blue};

	buffer = AfxGetApp()->GetProfileIntW(_T("KoppenDlg"), _T("color_g_0"), Color(original_graph_colors[0]).ToCOLORREF());
	color.SetFromCOLORREF(buffer);
	m_GeoGraphDlg.getTemperGraph()->setColor(color);

	buffer = AfxGetApp()->GetProfileIntW(_T("KoppenDlg"), _T("color_g_1"), Color(original_graph_colors[1]).ToCOLORREF());
	color.SetFromCOLORREF(buffer);
	m_GeoGraphDlg.getPrecipGraph()->setColor(color);

	COLORREF original_icon_colors[5] = { Color::Red, Color::Yellow, Color::LightGreen, Color::RoyalBlue, Color::Aqua };

	for (int i = 0; i < 5; i++) {
		CString str_c;
		str_c.Format(_T("color_i_%d"), i);
		buffer = AfxGetApp()->GetProfileIntW(_T("KoppenDlg"), str_c, Color(original_icon_colors[i]).ToCOLORREF());
		color.SetFromCOLORREF(buffer);
		m_staticInfo2.setColor(color, i);
	}

	SetWindowPos(NULL, 0, 0, 1075, 670, SWP_NOZORDER | SWP_NOMOVE);
	PostMessage(WM_SIZE);

	//CRect rect;
	//GetDesktopWindow()->GetWindowRect(&rect);
	//MrFormatMessage(_T("%d, %d"), rect.Width(), rect.Height());
	// 1920, 1080

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMainDlg::PostNcDestroy()
{
	for (int i = 0; i < 5; i++) {
		CString str_c;
		str_c.Format(_T("color_i_%d"), i);
		AfxGetApp()->WriteProfileInt(_T("KoppenDlg"), str_c, m_staticInfo2.getColor(i).ToCOLORREF());
	}
	CDialog::PostNcDestroy();
}

void CMainDlg::OnClose()
{
	AfxGetApp()->WriteProfileInt(_T("KoppenDlg"), _T("color_g_0"), m_GeoGraphDlg.getTemperGraph()->getColor().ToCOLORREF());
	AfxGetApp()->WriteProfileInt(_T("KoppenDlg"), _T("color_g_1"), m_GeoGraphDlg.getPrecipGraph()->getColor().ToCOLORREF());

	CDialog::OnClose();
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
		CBufferDC dc(this);
		//CPaintDC dc(this);

		Graphics graphics(dc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

		CRect Rect;
		GetClientRect(&Rect);

		Color color = { RGB_TO_ARGB(255, GetSysColor(COLOR_BTNFACE)) };
		graphics.FillRectangle(&SolidBrush(color), Rect.left - 2, Rect.top - 2, Rect.Width() + 2, Rect.Height() + 2);

		CRect info1_rect, info3_rect;

		m_staticInfo1.GetWindowRect(&info1_rect);
		m_staticInfo3.GetWindowRect(&info3_rect);
		ScreenToClient(&info1_rect);
		ScreenToClient(&info3_rect);

		info1_rect.InflateRect(10 * (Rect.Width() / 1075.0), 10 * (Rect.Width() / 1075.0));
		info3_rect.InflateRect(10 * (Rect.Width() / 1075.0), 10 * (Rect.Width() / 1075.0));

		dc.Rectangle(&info1_rect);
		dc.Rectangle(&info3_rect);
	}
}
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);

	int width = Rect.Width();
	int height = Rect.Height();
	int gap = height*0.05;

	int button_height = 30 * (width / 1075.0);
	
	CPoint center_point = { getSplitLine(), height / 2 };

	CRect graph_rect = MarginHelper.Initiate().SetMargin(gap)
						.OnLeft(0)
						.OnUp(0)
						.OnRight(center_point.x)
						.OnDown(height - button_height)
						.toRect();

	CRect info1_rect, info2_rect, info3_rect;
	int height_without_gaps = height - gap * 4;
	double info_height_unit = height_without_gaps / (info1_side + info2_side + info3_side);

	info1_rect = MarginHelper.Initiate()
					.SetHeight(info_height_unit * info1_side)
					.SetMargin(gap)
					.OnLeft(center_point.x,0)
					.OnRight(width)
					.AttachUp(0)
					.toRect();
	info2_rect = MarginHelper.Initiate()
					.SetHeight(info_height_unit * info2_side)
					.SetWidth(info_height_unit * info2_side)
					.SetMargin(gap)
					.OnCenterLine(center_point.x, width - gap, MrCenter::Horizontal)
					.AttachUp(info1_rect.bottom)
					.toRect();
	info3_rect = MarginHelper.Initiate()
					.SetHeight(info_height_unit * info3_side)
					.SetMargin(gap)
					.OnLeft(center_point.x,0)
					.OnRight(width)
					.AttachUp(info2_rect.bottom)
					.toRect();

	info1_rect.top += 10 * (width / 1075.0);
	info3_rect.bottom -= 10 * (width / 1075.0);
	// for inflated drawing boxes;

	CRect button1_rect, button2_rect, button3_rect;
	int button_width = graph_rect.Width() / 3 - 2;

	button1_rect = MarginHelper.Initiate()
					.SetHeight(button_height)
					.SetWidth(button_width)
					.AttachLeft(graph_rect.left)
					.AttachUp(graph_rect.bottom)
					.toRect();

	button2_rect = MarginHelper.Initiate()
					.SetHeight(button_height)
					.SetWidth(button_width)
					.OnCenterLine(graph_rect.left, graph_rect.right, MrCenter::Horizontal)
					.AttachUp(graph_rect.bottom)
					.toRect();

	button3_rect = MarginHelper.Initiate()
					.SetHeight(button_height)
					.SetWidth(button_width)
					.AttachRight(graph_rect.right)
					.AttachUp(graph_rect.bottom)
					.toRect();

	graph_rect.top -= 10;
	info1_rect.top -= 10;
	info2_rect.top -= 10;
	info3_rect.top -= 10;
	// for menu inflating;

	if (m_staticInfo1.GetSafeHwnd() != NULL) {
		m_GeoGraphDlg.MoveWindow(graph_rect);

		m_staticInfo1.MoveWindow(&info1_rect);
		m_staticInfo2.MoveWindow(&info2_rect);
		m_staticInfo3.MoveWindow(&info3_rect);

		m_Button_tempOnly.MoveWindow(button1_rect);
		m_Button_BothShow.MoveWindow(button2_rect);
		m_Button_precipOnly.MoveWindow(button3_rect);
		
		setFontSize(m_button_font, button_height * 0.55);
		setFontSize(m_info_font, button_height * 0.525);

		m_Button_BothShow.SetFont(&m_button_font);
		m_Button_tempOnly.SetFont(&m_button_font);
		m_Button_precipOnly.SetFont(&m_button_font);

		m_staticInfo1.SetFont(&m_info_font);
		m_staticInfo2.SetFont(&m_info_font);
		m_staticInfo3.SetFont(&m_info_font);
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
void CMainDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	int height = pRect->bottom - pRect->top;
	int width = pRect->right - pRect->left;
	int offset_x = (height * window_ratio - width) / 2;
	int offset_y = (width / window_ratio - height) / 2;
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
	}
}
void CMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1075;
	lpMMI->ptMinTrackSize.y = 670;

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CMainDlg::OnBnClickedButtonOnlyTemp()
{
	m_GeoGraphDlg.getTemperGraph()->show();
	m_GeoGraphDlg.getPrecipGraph()->hide();
	m_GeoGraphDlg.RedrawWindow();
}


void CMainDlg::OnBnClickedButtonOnlyPrecip()
{
	m_GeoGraphDlg.getTemperGraph()->hide();
	m_GeoGraphDlg.getPrecipGraph()->show();
	m_GeoGraphDlg.RedrawWindow();
}


void CMainDlg::OnBnClickedButtonBoth()
{
	m_GeoGraphDlg.getTemperGraph()->show();
	m_GeoGraphDlg.getPrecipGraph()->show();
	m_GeoGraphDlg.RedrawWindow();
}

LRESULT CMainDlg::OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam) 
{
	UpdateInfo1();
	string sign = m_KoppenCalculator.Calculate().getKoppenSign();
	UpdateInfo2(sign);
	UpdateInfo3(sign);

	if (m_OptionDlg.GetSafeHwnd() != NULL) {
		m_OptionDlg.PostMessageW(WM_USER_UPDATE_CLIMATE_INFO);
	}
	return 0;
}

void CMainDlg::UpdateInfo1() 
{
	wstringstream wss;

	auto [hot_month, hot_temp] = m_KoppenCalculator.getHottestMonth();
	auto [cold_month, cold_temp] = m_KoppenCalculator.getColdestMonth();
	auto [wet_month, wet_precip] = m_KoppenCalculator.getWettestMonth();
	auto [dry_month, dry_precip] = m_KoppenCalculator.getDriestMonth();
	double aver_temp = m_KoppenCalculator.getAverageTemp();
	double aver_precip = m_KoppenCalculator.getAveragePrecip();
	double total_precip = m_KoppenCalculator.getTotalPrecip();
	double evaporation_rate = m_KoppenCalculator.getEvaporationRate();

	wss << std::setprecision(1) << std::fixed;

	wss << _T("지구상 위치: ");

	if (m_KoppenCalculator.isOnNorthen()) {
		wss << _T("북반구") << endl << endl;
	}
	else {
		wss << _T("남반구") << endl << endl;
	}

	wss << _T("최난월 평균 기온: ") << hot_temp << _T(" ℃ (") << hot_month << _T("월)") << endl;
	wss << _T("최한월 평균 기온: ") << cold_temp << _T(" ℃ (") << cold_month << _T("월)") << endl;
	wss << _T("최다우월 평균 강수량: ") << wet_precip << _T("mm (") << wet_month << _T("월)") << endl;
	wss << _T("최소우월 평균 강수량: ") << dry_precip << _T("mm (") << dry_month << _T("월)") << endl;
	wss << endl;
	wss << _T("연평균 기온: ") << aver_temp << _T(" ℃") << endl;
	wss << _T("연평균 강수량: ") << aver_precip << _T("mm") << endl;
	wss << _T("총 강수량: ") << total_precip << _T("mm") << endl;
	wss << _T("총 증발산량: ") << evaporation_rate << _T("mm") << endl;

	m_staticInfo1.SetWindowTextW(wss.str().c_str());
}

void CMainDlg::UpdateInfo2(const string& sign) {
	wstring wsign = { sign.begin(), sign.end() };

	m_staticInfo2.setKoppenSign(wsign.c_str());
	m_staticInfo2.RedrawWindow();
} 

void CMainDlg::UpdateInfo3(const string& sign) {
	if (m_koppen_info_map.end() == m_koppen_info_map.find(sign)) {
		MrFormatMessage(_T("\'%s\'에 대한 해설이 파일에 없습니다."), wstring(sign.begin(), sign.end()).c_str());
		m_staticInfo3.SetWindowTextW(_T("(데이터 없음)"));
		::ShellExecute(NULL, TEXT("open"), _T("data\\explanation.txt"), NULL, NULL, SW_SHOWNORMAL);
		::ShellExecute(NULL, TEXT("open"), _T("data\\tag.txt"), NULL, NULL, SW_SHOWNORMAL);
		return;
	}
	wstring_view info_v = m_koppen_info_map.at(sign);
	m_staticInfo3.SetWindowTextW(info_v.data());
}


void CMainDlg::UpdateClimateInfos(LPCTSTR filename_tag, LPCTSTR filename_info) {
	ifstream tag_file(filename_tag, std::ios::in);
	wifstream info_file(filename_info, std::ios::in);

	info_file.imbue(std::locale("ko_KR.UTF-8"));

	vector<string> tag_vector = { std::istream_iterator<string>(tag_file), std::istream_iterator<string>() };

	std::for_each(tag_vector.begin(), tag_vector.end(), [](string& str) {
		str.erase(std::remove_if(str.begin(), str.end(), [](char ch) -> bool {
			return ch == ',';
		}));
	});

	std::sort(tag_vector.begin(), tag_vector.end());
	tag_vector.erase(std::unique(tag_vector.begin(), tag_vector.end()), tag_vector.end());
	
	wstring buffer = { std::istream_iterator<wchar_t, wchar_t>(info_file), std::istream_iterator<wchar_t, wchar_t>() };

	auto pos1 = buffer.begin();
	auto end1 = buffer.begin();
	auto pos2 = buffer.begin();
	auto end2 = buffer.begin();

	while (true) {
		string tag;
		wstring info;

		pos1 = std::find(pos1, buffer.end(), _T('<'));
		end1 = std::find(pos1, buffer.end(), _T('>'));
		pos2 = std::find(pos2, buffer.end(), _T('['));
		end2 = std::find(pos2, buffer.end(), _T(']'));

		if (end1 == buffer.end() || end2 == buffer.end()) {
			break;
		}

		std::copy(pos1 + 1, end1, std::inserter(tag, tag.begin()));
		std::copy(pos2 + 1, end2, std::inserter(info, info.begin()));
		pos1 = end1;
		pos2 = end2;

		if (std::binary_search(tag_vector.begin(), tag_vector.end(), tag)) {
			m_koppen_info_map.insert(std::make_pair(tag, info));
		}
	}

	tag_file.close();
	info_file.close();
}


void CMainDlg::OnMenuSave()
{
	wstring cur_path = std::filesystem::current_path();
	cur_path += _T("\\save\\save");
	CFileDialog FileDialog(FALSE, _T("txt"), cur_path.c_str(), OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("텍스트 파일(*.txt)|*.txt||"), this);

	if (FileDialog.DoModal() == IDOK) {
		CString file_dir = FileDialog.GetPathName();
		CString file_name = FileDialog.GetFileName();
		CString file_ext = FileDialog.GetFileExt();
		CString file_ext_with_dot = _T(".") + file_ext;

		CString origin_dir = file_dir;
		int idx = origin_dir.ReverseFind(_T('.'));
		origin_dir.Delete(idx, file_ext_with_dot.GetLength());

		int i = 0;
		CFileFind finder;
		while (finder.FindFile(file_dir)) {
			i++;
			file_dir = origin_dir;
			file_dir.AppendFormat(_T("(%d).txt"), i);
		}

		wofstream wfout(file_dir);
		wfout << std::setprecision(1) << std::fixed;

		auto pvec_t = m_GeoGraphDlg.getTemperGraph()->getVector();
		auto pvec_p = m_GeoGraphDlg.getPrecipGraph()->getVector();
		auto pvec_m = m_GeoGraphDlg.getGraphMarker()->getVector();

		wfout << _T("temperature : ");
		std::copy(pvec_t->begin(), pvec_t->end(), std::ostream_iterator<double, wchar_t>(wfout, _T(" ")));
		wfout << endl;

		wfout << _T("precipitation : ");
		std::copy(pvec_p->begin(), pvec_p->end(), std::ostream_iterator<double, wchar_t>(wfout, _T(" ")));
		wfout << endl;

		auto [temp_lower, temp_upper] = m_GeoGraphDlg.getTemperGraph()->getGraphLimits();
		auto [prcp_lower, prcp_upper] = m_GeoGraphDlg.getPrecipGraph()->getGraphLimits();

		auto temp_finder = [](auto tuple) {
			auto [inOut, direction, ratio, string] = tuple;
			return inOut == MrInOut::Outer && direction == MrDirection::Left;
		};

		auto precip_finder = [](auto tuple) {
			auto [inOut, direction, ratio, string] = tuple;
			return inOut == MrInOut::Outer && direction == MrDirection::Right;
		};

		auto temp_beg = std::find_if(pvec_m->begin(), pvec_m->end(), temp_finder);
		auto temp_end = std::find_if(pvec_m->rbegin(), pvec_m->rend(), temp_finder).base();
		auto prcp_beg = std::find_if(pvec_m->begin(), pvec_m->end(), precip_finder);
		auto prcp_end = std::find_if(pvec_m->rbegin(), pvec_m->rend(), precip_finder).base();

		temp_end--, prcp_end--;

		int temp_unit = (temp_upper - temp_lower) / std::distance(temp_beg, --temp_end);
		int prcp_unit = (prcp_upper - prcp_lower) / std::distance(prcp_beg, --prcp_end);

		wfout << _T("temp_marker : ") << (int)temp_lower << _T(" ") << (int)temp_upper << _T(" ") << temp_unit << endl;
		wfout << _T("precip_marker : ") << (int)prcp_lower << _T(" ") << (int)prcp_upper << _T(" ") << prcp_unit << endl;

		if (i == 0) {
			AfxMessageBox(_T("파일을 성공적으로 만들었습니다."));
		}
		else {
			CString modified_name = file_name;
			idx = modified_name.ReverseFind(_T('.'));
			modified_name.Delete(idx, file_ext_with_dot.GetLength());
			modified_name.AppendFormat(_T("(%d).txt"), i);
			AfxMessageBox(_T("같은 이름의 파일이 있어 \"") + modified_name + _T("\" 라는 이름으로 저장하였습니다"));
		}

		wfout.close();
	}
}


void CMainDlg::OnMenuLoad()
{
	wstring cur_path = std::filesystem::current_path();
	cur_path += _T("\\save\\");
	CFileDialog FileDialog(TRUE, _T("txt"), cur_path.c_str(), OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("텍스트 파일(*.txt)|*.txt||"), this);

	if (FileDialog.DoModal() == IDOK) {
		CString file_dir = FileDialog.GetPathName();
		CString file_name = FileDialog.GetFileName();

		wifstream wfin(file_dir);

		wstring bin;
		auto pvec_t = m_GeoGraphDlg.getTemperGraph()->getVector();
		auto pvec_p = m_GeoGraphDlg.getPrecipGraph()->getVector();

		wfin >> bin >> bin;

		for (int i = 0; i < 12; i++) {
			wfin >> pvec_t->at(i);
		}

		wfin >> bin >> bin;

		for (int i = 0; i < 12; i++) {
			wfin >> pvec_p->at(i);
		}

		int temp_lower, temp_upper, temp_unit, prcp_lower, prcp_upper, prcp_unit;

		wfin >> bin >> bin >> temp_lower >> temp_upper >> temp_unit;
		wfin >> bin >> bin >> prcp_lower >> prcp_upper >> prcp_unit;

		m_GeoGraphDlg.getGraphMarker()->removeMark(MrDirection::Left, MrInOut::Outer);
		m_GeoGraphDlg.getGraphMarker()->removeMark(MrDirection::Right, MrInOut::Outer);
		m_GeoGraphDlg.getGraphMarker()->addSuccessiveMark(temp_lower, temp_upper, temp_unit, MrDirection::Left, MrInOut::Outer);
		m_GeoGraphDlg.getGraphMarker()->addSuccessiveMark(prcp_lower, prcp_upper, prcp_unit, MrDirection::Right, MrInOut::Outer);
		m_GeoGraphDlg.getGraphMarker()->arrange();

		Invalidate();

		wfin.close();

		AfxMessageBox(_T("파일을 성공적으로 불러왔습니다."));
	}
}


void CMainDlg::OnImageSave1()
{
	CRect rect;
	m_GeoGraphDlg.GetClientRect(&rect);

	CImage image;
	image.Create(rect.Height(), rect.Height(), 32);

	m_GeoGraphDlg.paintImage1(image);

	wstring cur_path = std::filesystem::current_path();
	cur_path += _T("\\image\\Image");

	CFileDialog FileDialog(FALSE, _T("jpeg"), cur_path.c_str(), OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("JPEG 파일(*.jpeg)|*.jpeg|PNG 파일(*.png)|*.png|BMP 파일(*.bmp)|*.bmp|GIF 파일(*.gif)|*.gif||"), this);
	if (FileDialog.DoModal() == IDOK) {
		CString file_dir = FileDialog.GetPathName();
		CString file_name = FileDialog.GetFileName();
		CString file_ext = FileDialog.GetFileExt();
		CString file_ext_with_dot = _T(".") + file_ext;

		CString origin_dir = file_dir;
		int idx = origin_dir.ReverseFind(_T('.'));
		origin_dir.Delete(idx, file_ext_with_dot.GetLength());

		int i = 0;
		CFileFind finder;
		while (finder.FindFile(file_dir)) {
			i++;
			file_dir = origin_dir;
			file_dir.AppendFormat(_T("(%d)") + file_ext_with_dot, i);
		}

		GUID guid;
		if (file_ext == _T("jpeg")) guid = Gdiplus::ImageFormatJPEG;
		else if (file_ext == _T("png")) guid = Gdiplus::ImageFormatPNG;
		else if (file_ext == _T("bmp")) guid = Gdiplus::ImageFormatGIF;
		else if (file_ext == _T("gif")) guid = Gdiplus::ImageFormatBMP;
		else {
			MrFormatMessage(_T("확장자 %s로는 저장할 수 없습니다."), file_ext);
			return;
		}

		image.Save(file_dir, guid);

		if (i == 0) {
			AfxMessageBox(_T("파일을 성공적으로 만들었습니다."));
		}
		else {
			CString modified_name = file_name;
			idx = modified_name.ReverseFind(_T('.'));
			modified_name.Delete(idx, file_ext_with_dot.GetLength());
			modified_name.AppendFormat(_T("(%d)") + file_ext_with_dot, i);
			AfxMessageBox(_T("같은 이름의 파일이 있어 \"") + modified_name + _T("\" 라는 이름으로 저장하였습니다"));
		}
	}
}


void CMainDlg::OnImageSave2()
{
	CRect rect;
	m_GeoGraphDlg.GetClientRect(&rect);

	CImage image;
	image.Create(rect.Height() * 0.6, rect.Height(), 32);

	m_GeoGraphDlg.paintImage2(image);

	wstring cur_path = std::filesystem::current_path();
	cur_path += _T("\\image\\Image");

	CFileDialog FileDialog(FALSE, _T("jpeg"), cur_path.c_str(), OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("JPEG 파일(*.jpeg)|*.jpeg|PNG 파일(*.png)|*.png|BMP 파일(*.bmp)|*.bmp|GIF 파일(*.gif)|*.gif||"), this);
	if (FileDialog.DoModal() == IDOK) {
		CString file_dir = FileDialog.GetPathName();
		CString file_name = FileDialog.GetFileName();
		CString file_ext = FileDialog.GetFileExt();
		CString file_ext_with_dot = _T(".") + file_ext;

		CString origin_dir = file_dir;
		int idx = origin_dir.ReverseFind(_T('.'));
		origin_dir.Delete(idx, file_ext_with_dot.GetLength());

		int i = 0;
		CFileFind finder;
		while (finder.FindFile(file_dir)) {
			i++;
			file_dir = origin_dir;
			file_dir.AppendFormat(_T("(%d)") + file_ext_with_dot, i);
		}

		GUID guid;
		if (file_ext == _T("jpeg")) guid = Gdiplus::ImageFormatJPEG;
		else if (file_ext == _T("png")) guid = Gdiplus::ImageFormatPNG;
		else if (file_ext == _T("bmp")) guid = Gdiplus::ImageFormatGIF;
		else if (file_ext == _T("gif")) guid = Gdiplus::ImageFormatBMP;
		else {
			MrFormatMessage(_T("확장자 %s로는 저장할 수 없습니다."), file_ext);
			return;
		}

		image.Save(file_dir, guid);

		if (i == 0) {
			AfxMessageBox(_T("파일을 성공적으로 만들었습니다."));
		}
		else {
			CString modified_name = file_name;
			idx = modified_name.ReverseFind(_T('.'));
			modified_name.Delete(idx, file_ext_with_dot.GetLength());
			modified_name.AppendFormat(_T("(%d)") + file_ext_with_dot, i);
			AfxMessageBox(_T("같은 이름의 파일이 있어 \"") + modified_name + _T("\" 라는 이름으로 저장하였습니다"));
		}
	}
}


void CMainDlg::OnMenuOption()
{
	if (m_OptionDlg.GetSafeHwnd() == NULL) {
		m_OptionDlg.Create(IDD_DIALOG_OPTIONS, this);
	}

	CRect rect;
	GetWindowRect(&rect);
	rect.DeflateRect(rect.Width() * 0.15, rect.Height() * 0.07);

	m_OptionDlg.MoveWindow(&rect);
	m_OptionDlg.ShowWindow(SW_SHOW);
}


void CMainDlg::OnMenuHelp()
{
	static CHelpDlg HelpDialog;

	if (HelpDialog.GetSafeHwnd() == NULL) {
		HelpDialog.Create(IDD_DIALOG_HELP, this);
	}
	HelpDialog.ShowWindow(SW_SHOW);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
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

	return CDialog::PreTranslateMessage(pMsg);
}
