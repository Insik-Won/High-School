// SecureWarningDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Calc Ckeck.h"
#include "SecureWarningDlg.h"
#include "afxdialogex.h"


// SecureWarningDlg 대화 상자

IMPLEMENT_DYNAMIC(SecureWarningDlg, CDialogEx)

SecureWarningDlg::SecureWarningDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog_SecureWarning, pParent)
	, m_strPassword(_T(""))
{

}

SecureWarningDlg::~SecureWarningDlg()
{
}

void SecureWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Edit_Password, m_strPassword);
}


BEGIN_MESSAGE_MAP(SecureWarningDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SecureWarningDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SecureWarningDlg 메시지 처리기

void SecureWarningDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

CString SecureWarningDlg::GetPassword() {
	return m_strPassword;
}
