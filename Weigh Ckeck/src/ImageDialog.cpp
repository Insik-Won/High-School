// ImageDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Weight Ckeck.h"
#include "ImageDialog.h"
#include "afxdialogex.h"


// ImageDialog 대화 상자

IMPLEMENT_DYNAMIC(ImageDialog, CDialogEx)

ImageDialog::ImageDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Dialog_Image, pParent)
{

}

ImageDialog::~ImageDialog()
{
}

void ImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageDialog, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ImageDialog 메시지 처리기


void ImageDialog::OnPaint()
{
	CPaintDC dc(this);

	Gdiplus::Graphics graphics(dc);
	graphics.DrawImage(m_bitmap, 0, 0);
}

void ImageDialog::SetImageResource(UINT ID) {
	m_bitmap.Load(ID);
	Invalidate();
}

CGdiPlusBitmapResource* ImageDialog::GetBitmap() {
	return &m_bitmap;
}


BOOL ImageDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			//return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}

		if (pMsg->wParam == VK_RETURN)
		{
			//return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
