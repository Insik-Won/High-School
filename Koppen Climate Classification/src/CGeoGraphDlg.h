#pragma once


// CGeoGraphDlg 대화 상자

class CGeoGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGeoGraphDlg)

public:
	CGeoGraphDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGeoGraphDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GEOGRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CPointer m_Pointer;

	CGraph m_Graph_Temper;
	CGraph m_Graph_Precip;
	CGraphMarker m_Graph_Marker;

	CGraph* m_now_pGraph;
	int m_now_index;
	int m_pointer_offset;

public:
	CGraph* getTemperGraph();
	CGraph* getPrecipGraph();
	CGraphMarker* getGraphMarker();

	void paintImage1(CImage& image);
	void paintImage2(CImage& image);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
};
