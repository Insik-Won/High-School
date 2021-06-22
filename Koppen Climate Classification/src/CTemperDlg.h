#pragma once

// CTemperDlg 대화 상자

class CTemperDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTemperDlg)

public:
	CTemperDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTemperDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GRAPH_TMP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CPointer m_Pointer;
	CGraph m_Temper_Graph;

	int m_pointer_offset;
	int m_dragging_index;

private:
	void WriteTitleAsBool(bool flag);

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
