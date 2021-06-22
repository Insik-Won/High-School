#pragma once


// SecureWarningDlg 대화 상자

class SecureWarningDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SecureWarningDlg)

public:
	SecureWarningDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SecureWarningDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dialog_SecureWarning };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CString m_strPassword;
	CString GetPassword();
	afx_msg void OnBnClickedOk();
};
