// ImageDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Calc Ckeck.h"
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
//	ON_WM_PAINT()
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
