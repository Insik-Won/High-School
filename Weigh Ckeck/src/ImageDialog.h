#pragma once
#include "CGdiPlusBitmap.h"


// ImageDialog 대화 상자

class ImageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ImageDialog)

public:
	ImageDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ImageDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dialog_Image };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private: 
	CGdiPlusBitmapResource m_bitmap;

public:
	afx_msg void OnPaint();
	void SetImageResource(UINT ID);
	CGdiPlusBitmapResource* GetBitmap();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
