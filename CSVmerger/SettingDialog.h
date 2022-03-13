#pragma once
#include "afxwin.h"

// SettingDialog 대화 상자

class SettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SettingDialog)

public:
	SettingDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SettingDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void InitialSetting(void);

	DECLARE_MESSAGE_MAP()
public:
	CMFCPropertyGridCtrl m_PropGrid1;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
