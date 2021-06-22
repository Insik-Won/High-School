#pragma once
#include "CStandardSegm.h"

// CStandardDlg 대화 상자

const int snum = 7;
const int sBID[snum] = {
	IDB_STANDARD_ANGLE
	,IDB_STANDARD_ANGLE
	,IDB_STANDARD_CHANNEL
	,IDB_STANDARD_HBEAM
	,IDB_STANDARD_IBEAM
	,IDB_STANDARD_BOLT1
	,IDB_STANDARD_NUT1
};

class CStandardDlg : public CBasicSgmMngDlg
{
	DECLARE_DYNAMIC(CStandardDlg)

public:
	CStandardDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStandardDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dialog_Standard };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	CStandardSegm m_StandardSegm[snum];
	pvFunc(*m_pCalcFuncs)[snum];
public:
	virtual BOOL OnInitDialog();
	void SetDigit(int iDigit);
	void SetSpGravity(double dSpGravity);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
