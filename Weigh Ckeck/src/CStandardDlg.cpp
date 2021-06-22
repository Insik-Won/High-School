// CStandardDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Weight Ckeck.h"
#include "CStandardDlg.h"
#include "afxdialogex.h"


// CStandardDlg 대화 상자

IMPLEMENT_DYNAMIC(CStandardDlg, CDialogEx)

CStandardDlg::CStandardDlg(CWnd* pParent /*=nullptr*/)
	: CBasicSgmMngDlg(IDD_Dialog_Standard, pParent)
{
	m_pCalcFuncs = NULL;
	m_pcbaSegments.ClearAndSetLen(snum);
	for (int i = 0; i < snum; i++) {
		m_pcbaSegments[i] = &m_StandardSegm[i];
	}
}

CStandardDlg::~CStandardDlg()
{
}

void CStandardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStandardDlg, CDialogEx)
END_MESSAGE_MAP()

void CStandardDlg::SetDigit(int iDigit) {
	CBasicSgmMngDlg::SetDigit(iDigit);
	for (int i = 0; i < snum; i++) {
		m_StandardSegm[i].OnCalculate();
	}
}

void CStandardDlg::SetSpGravity(double dSpGravity) {
	CBasicSgmMngDlg::SetSpGravity(dSpGravity);
	for (int i = 0; i < snum; i++) {
		m_StandardSegm[i].OnCalculate();
	}
}

// CStandardDlg 메시지 처리기


BOOL CStandardDlg::OnInitDialog()
{
	CBasicSgmMngDlg::OnInitDialog();

	CRect Rect(0, 0, 750, 24);
	Rect.top += 40;
	Rect.bottom += 40;

	CString strTmp = _T("");
	for (int i = 0; i < snum; i++) {
		m_StandardSegm[i].Create(IDD_Segm_Standard, this);
		m_StandardSegm[i].SetBitmapID(sBID[i]);
		//m_StandardSegm[i].SetCalcFunc((*m_pCalcFuncs)[i]);

		strTmp.LoadStringW(IDS_SN_ANGLE1 + i);
		m_StandardSegm[i].GetDlgItem(IDC_Static_Name)->SetWindowText(strTmp);
		m_StandardSegm[i].LoadDataFromFile(strTmp, !(i==6));

		m_StandardSegm[i].SetWindowPos(NULL, Rect.left, Rect.top, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW | SWP_SHOWWINDOW);
		Rect.top += 24 + 10;
		Rect.bottom += 24 + 10;
	}

	m_StandardSegm[5].GetEdit(2)->SetWindowTextW(_T("1"));
	m_StandardSegm[5].SetFlag(TRUE);
	m_StandardSegm[5].PostMessage(WM_SCALC, 0, 0);

	m_StandardSegm[6].GetEdit(2)->SetWindowTextW(_T("1"));
	m_StandardSegm[6].SetFlag(TRUE);
	m_StandardSegm[6].PostMessage(WM_SCALC, 0, 0);

	m_StandardSegm[6].GetComboBox()->SetCurSel(1);

	return TRUE;
}


BOOL CStandardDlg::PreTranslateMessage(MSG* pMsg)
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

	return CBasicSgmMngDlg::PreTranslateMessage(pMsg);
}
