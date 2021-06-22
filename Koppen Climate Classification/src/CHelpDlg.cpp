// CHelpDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Koppen Climate Classification.h"
#include "CHelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 대화 상자

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HELP, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GROUP, m_statics[0]);
	DDX_Control(pDX, IDC_STATIC_bar1, m_statics[1]);
	DDX_Control(pDX, IDC_STATIC_bar2, m_statics[2]);
	DDX_Control(pDX, IDC_STATIC_bar3, m_statics[3]);
	DDX_Control(pDX, IDC_STATIC_bar4, m_statics[4]);
	DDX_Control(pDX, IDC_STATIC_bar5, m_statics[5]);
	DDX_Control(pDX, IDC_STATIC_expl1, m_statics[6]);
	DDX_Control(pDX, IDC_STATIC_expl2, m_statics[7]);
	DDX_Control(pDX, IDC_STATIC_expl3, m_statics[8]);
	DDX_Control(pDX, IDC_STATIC_expl4, m_statics[9]);
	DDX_Control(pDX, IDC_STATIC_expl5, m_statics[10]);
	DDX_Control(pDX, IDC_STATIC_dvlp1, m_statics[11]);
	DDX_Control(pDX, IDC_STATIC_dvlp2, m_statics[12]);
	DDX_Control(pDX, IDC_STATIC_dvlp3, m_statics[13]);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelpDlg 메시지 처리기


BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFontIndirectW(&lf);
	setFontName(m_font, _T("맑은 고딕"));

	for (int i = 0; i < 14; i++) {
		m_statics[i].SetFont(&m_font);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
