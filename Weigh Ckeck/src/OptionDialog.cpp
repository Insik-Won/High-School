// OptionDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Weight Ckeck.h"
#include "OptionDialog.h"
#include "afxdialogex.h"


// OptionDialog 대화 상자

IMPLEMENT_DYNAMIC(OptionDialog, CDialogEx)

OptionDialog::OptionDialog(CMainDialog* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog_Option, pParent)
{
	m_pParent = pParent;
}

BOOL OptionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int n = m_pParent->GetDigit();
	m_DigitCombo.SetCurSel(n);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


OptionDialog::~OptionDialog()
{
}

void OptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Combo_digit, m_DigitCombo);
}


BEGIN_MESSAGE_MAP(OptionDialog, CDialogEx)
	ON_BN_CLICKED(IDC_Button_Reset, &OptionDialog::OnBnClickedButtonReset)
	ON_CBN_SELCHANGE(IDC_Combo_digit, &OptionDialog::OnCbnSelchangeCombodigit)
END_MESSAGE_MAP()


// OptionDialog 메시지 처리기

void OptionDialog::OnCbnSelchangeCombodigit()
{
	UpdateData(TRUE);
	int n = m_DigitCombo.GetCurSel();
	m_pParent->SetDigit(n);
}


void OptionDialog::OnBnClickedButtonReset()
{
	m_DigitCombo.SetCurSel(1);
	m_pParent->SetDigit(1);
}

BOOL OptionDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			//return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			//return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
