// COptionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Antibiotic Immune Simulator.h"
#include "COptionDlg.h"
#include "afxdialogex.h"


// COptionDlg 대화 상자

IMPLEMENT_DYNAMIC(COptionDlg, CDialogEx)

COptionDlg::COptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPTION, pParent)
	, m_strGridNum(_T(""))
	, m_strMutateRatio(_T(""))
	, m_strAntibSpeed(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMainDlg = nullptr;
}

COptionDlg::~COptionDlg()
{
}

BOOL COptionDlg::Create(UINT nIDTemplate, CMainDlg* pMainDlg, CWnd* pParentWnd)
{
	m_pMainDlg = pMainDlg;
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

BOOL COptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	return TRUE;
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GRIDNUM, m_strGridNum);
	DDX_Text(pDX, IDC_EDIT_MUTATE_RATIO, m_strMutateRatio);
	DDX_Text(pDX, IDC_EDIT_ANTIB_SPEED, m_strAntibSpeed);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ALLRESTART, &COptionDlg::OnBnClickedButtonAllrestart)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &COptionDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, &COptionDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// COptionDlg 메시지 처리기


void COptionDlg::OnBnClickedButtonAllrestart()
{
	m_pMainDlg->Restart();

	FormatDouble(&m_strGridNum, m_pMainDlg->m_SimulDlg.GetGridNumber(), 0);
	FormatDouble(&m_strMutateRatio, CBacteria::mutation_rate * 100, 2);
	FormatDouble(&m_strAntibSpeed, m_pMainDlg->m_SimulDlg.GetSpreadSpeed() / 2, 2);

	m_strMutateRatio += '%';
	UpdateData(FALSE);
}
// 전체 초기화 버튼 클릭 시 모두 초기화시키고 옵션 수치를 다시 업데이트하는 함수

void COptionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE) {
		FormatDouble(&m_strGridNum, m_pMainDlg->m_SimulDlg.GetGridNumber(), 0);
		FormatDouble(&m_strMutateRatio, CBacteria::mutation_rate * 100, 2);
		FormatDouble(&m_strAntibSpeed, m_pMainDlg->m_SimulDlg.GetSpreadSpeed() / 2 , 2);

		m_strMutateRatio += '%';
		UpdateData(FALSE);
	}
}
// ShowWindow() 함수를 통해 윈도우가 뜨면 옵션 수치를 업데이트하는 함수

void COptionDlg::OnBnClickedButtonApply() {
	UpdateData(TRUE);
	m_strMutateRatio.TrimRight(_T("%"));

	int tmp = 0;
	if (CheckAndAtoi(m_strGridNum, &tmp, TRUE)) {
		m_pMainDlg->m_SimulDlg.SetGridNumber(tmp);
		m_pMainDlg->m_SimulDlg.GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	}

	double dTemp = 0;
	if (CheckAndAtof(m_strMutateRatio, &dTemp, TRUE)) {
		CBacteria::mutation_rate = dTemp / 100;
	}
	if (CheckAndAtof(m_strAntibSpeed, &dTemp, TRUE)) {
		m_pMainDlg->m_SimulDlg.SetSpreadSpeed(dTemp * 2);
	}
}
// 적용 버튼 클릭 시 각종 옵션 수치를 m_pMainDlg로 옮기는 함수

void COptionDlg::OnBnClickedCancel()
{
	OnCancel();
}
// 취소 버튼 클릭 시 나가는 함수

BOOL COptionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonApply();
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
// Enter/Esc키 잡아먹는 함수