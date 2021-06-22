#pragma once


// CKoppenOptionDlg 대화 상자

class CKoppenOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKoppenOptionDlg)

public:
	CKoppenOptionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CKoppenOptionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	const static int inline left_side = 1;
	const static int inline right_side = 3;

	int getCenterLine() {
		CRect Rect;
		GetClientRect(&Rect);
		return Rect.Width() * left_side / right_side;
	}

private:
	CListBox m_list_options;

	CDialog* m_OptionDlgs[3];
	int m_now_index;

	CButton m_button_reset;
	CButton m_button_apply;
	CButton m_button_ok;
	CButton m_button_cancel;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeListOptions();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnUpdateClimateInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonReset();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
