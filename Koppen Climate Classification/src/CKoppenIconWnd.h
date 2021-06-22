#pragma once


// CKoppenIconWnd

class CKoppenIconWnd : public CWnd
{
	DECLARE_DYNAMIC(CKoppenIconWnd)

public:
	CKoppenIconWnd();
	virtual ~CKoppenIconWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CString m_koppen_sign;

	Color m_icon_colors[5];

public:
	void	setKoppenSign(LPCTSTR sign);
	LPCTSTR getKoppenSign();

	Color	getColor(int idx);
	void	setColor(Color color, int idx);

	void DrawIcon(Graphics& graphics, const CRect& rect, LPCTSTR sign);

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


