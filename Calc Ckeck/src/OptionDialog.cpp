// OptionDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Calc Ckeck.h"
#include "OptionDialog.h"
#include "afxdialogex.h"


// OptionDialog 대화 상자

IMPLEMENT_DYNAMIC(OptionDialog, CDialogEx)

OptionDialog::OptionDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog_Option, pParent)
	, m_iDigit(0)
{
	m_CParentWnd = NULL;
}

OptionDialog::OptionDialog(CMainDialog* pParent) : CDialogEx(IDD_Dialog_Option, pParent) {
	m_CParentWnd = pParent;
	m_iDigit = pParent->GetDigit();
}

OptionDialog::~OptionDialog() {
}

void OptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_Combo_digit, m_iDigit);
}


BEGIN_MESSAGE_MAP(OptionDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_Combo_digit, &OptionDialog::OnCbnSelchangeCombodigit)
	ON_BN_CLICKED(IDC_Button_DirSelect, &OptionDialog::OnBnClickedButtonDirselect)
	ON_BN_CLICKED(IDC_Button_Reset, &OptionDialog::OnBnClickedButtonReset)
END_MESSAGE_MAP()


void OptionDialog::SetDigit(int Digit) { m_iDigit = Digit; };
int OptionDialog::GetDigit() { return m_iDigit; };


// OptionDialog 메시지 처리기


void OptionDialog::OnCbnSelchangeCombodigit()
{
	UpdateData(TRUE);
	if (m_CParentWnd->GetSafeHwnd() != NULL) {
		m_CParentWnd->SetDigit(m_iDigit);
	}
}


void OptionDialog::OnBnClickedButtonDirselect()
{
	BROWSEINFO bi;
	TCHAR szBuffer[MAX_PATH];
	::ZeroMemory(&bi, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, MAX_PATH);

	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("파일이 저장될 폴더를 선택해 주세요");
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&bi);
	if (::SHGetPathFromIDList(pItemIdList, szBuffer)) {
		CString strTmp = _T("");
		strTmp.Format(_T("%s"), szBuffer);
		if (m_CParentWnd->GetSafeHwnd() != NULL) {
			m_CParentWnd->SetFileDir(strTmp);
			AfxMessageBox(_T("파일 위치(") + strTmp + _T(")를 설정 완료하였습니다."));
			return;
		}
		AfxMessageBox(_T("폴더 위치 설정에 문제가 생겼습니다."));
	}
}

void OptionDialog::OnBnClickedButtonReset()
{
	SetDigit(1);
	if (m_CParentWnd->GetSafeHwnd() != NULL) {
		m_CParentWnd->SetDigit(m_iDigit);
		m_CParentWnd->SetFileDir(_T("C:\\Users\\User\\Documents"));
	}
	UpdateData(FALSE);

	AfxMessageBox(_T("초기화를 완료하였습니다. ( 폴더 위치 : \"라이브러리/문서\" )"));
}
