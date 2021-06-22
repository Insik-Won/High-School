#pragma once
#include "CBasicClasses.h"


// CFigureSegm 대화 상자

class CFigureSegm : public CBasicSegment
{
	DECLARE_DYNAMIC(CFigureSegm)

public:
	CFigureSegm(CBasicSgmMngDlg* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFigureSegm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Segm_Figure };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	double m_dResult;
	CString m_strNum;
	BOOL m_bIsExVisible;
protected:
	CString m_szaDefault[4];

public:
	void SetEditVisible(int index, BOOL bFlag);
	//BOOL isEditVisible(int index);
	void SetVisibleEx(BOOL bFlag = TRUE);
	BOOL IsVisibleEx();
	int HowManyEdits();

	void SetDefaults(LPCTSTR* szaInput);
	CString* GetDefaults();

	double GetResult();
	void SetResult(const double dnum);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
public:
	LRESULT afx_msg OnCalculate(WPARAM wParam = NULL, LPARAM lParam = NULL);
	afx_msg void OnEnChangeEdit();
	afx_msg void OnEnKillFocusEdit1();
	afx_msg void OnEnKillFocusEdit2();
	afx_msg void OnEnKillFocusEdit3();
	afx_msg void OnEnKillFocusEdit4();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
