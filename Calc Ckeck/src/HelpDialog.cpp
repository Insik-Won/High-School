// HelpDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Calc Ckeck.h"
#include "HelpDialog.h"
#include "afxdialogex.h"


// HelpDialog 대화 상자

IMPLEMENT_DYNAMIC(HelpDialog, CDialogEx)

HelpDialog::HelpDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog_Help, pParent)
	, m_strCurFileDir(_T(""))
{

}

HelpDialog::HelpDialog(CMainDialog* pParent) : CDialogEx(IDD_Dialog_Help, pParent) {
	m_strCurFileDir = _T("기본 파일 저장 위치 : ") + pParent->GetFileDir();
}

HelpDialog::~HelpDialog()
{
}

void HelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Static_CurFileDir, m_strCurFileDir);
}


BEGIN_MESSAGE_MAP(HelpDialog, CDialogEx)
END_MESSAGE_MAP()

void HelpDialog::SetCurFileDir(const CString& str) {
	m_strCurFileDir = _T("기본 파일 저장 위치: ") + str;
	UpdateData(FALSE);
}
CString HelpDialog::GetCurFileDir() {
	return m_strCurFileDir;
}


// HelpDialog 메시지 처리기
