#pragma once
#include "CMainDlg.h"


// COptionDlg 대화 상자

class COptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~COptionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
	CMainDlg* m_pMainDlg;

	CString m_strGridNum;
	CString m_strMutateRatio;
	CString m_strAntibSpeed;
	// 각종 옵션 수치를 저장할 CString형 변수와 데이터를 받아올 CMainDlg의 포인터

public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create(UINT nIDTemplate, CMainDlg* pMainDlg, CWnd* pParentWnd = NULL);

	afx_msg void OnBnClickedButtonAllrestart();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
