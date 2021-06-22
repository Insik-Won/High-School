#pragma once
#include "CMainDialog.h"


// OptionDialog 대화 상자

class OptionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(OptionDialog)

public:
	OptionDialog(CMainDialog* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OptionDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dialog_Option };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CMainDialog* m_pParent;
	CComboBox m_DigitCombo;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombodigit();
	afx_msg void OnBnClickedButtonReset();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
