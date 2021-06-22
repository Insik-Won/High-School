#include "pch.h"
#include "CMrStatic.h"

IMPLEMENT_DYNAMIC(CMrStatic, CStatic)


BEGIN_MESSAGE_MAP(CMrStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


HBRUSH CMrStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}


void CMrStatic::SetWindowTextW(LPCTSTR str)
{
	CStatic::SetWindowTextW(str);

	DWORD style = GetStyle();
	if (style & WS_CHILD) {
		CRect rect;
		GetWindowRect(&rect);
		GetParent()->ScreenToClient(&rect);
		GetParent()->InvalidateRect(&rect);
	}
}

BOOL CMrStatic::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return false; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			return false;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}
