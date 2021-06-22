#pragma once
#include <afxwin.h>

class CMrStatic : public CStatic
{
	DECLARE_DYNAMIC(CMrStatic)
	
public:
	CMrStatic() = default;
	virtual ~CMrStatic() = default;

public:
	void SetWindowTextW(LPCTSTR str);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

