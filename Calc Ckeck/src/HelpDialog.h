#pragma once
#include "CMainDialog.h"


// HelpDialog 대화 상자

class HelpDialog : public CDialogEx
{
	DECLARE_DYNAMIC(HelpDialog)

public:
	HelpDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	HelpDialog(CMainDialog* pParent);
	virtual ~HelpDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dialog_Help };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCurFileDir;
	void SetCurFileDir(const CString& str);
	CString GetCurFileDir();
};
