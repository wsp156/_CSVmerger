
// CSVmergerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BringInformation.h"


class Tab1;
class Tab2;
class Tab3;
class SettingDialog;
class Progressbar;
#define MAXDIVISIONFILENUM	500
#define NASCAMESSAGE	AfxMessageBox(Nascafilenum + "th file was covered by NASCA. Please remove NASCA.")

// CCSVmergerDlg 대화 상자
class CCSVmergerDlg : public CDialogEx
{
// 생성입니다.
public:
	CCSVmergerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	Tab1 *pDlg1;
	Tab2 *pDlg2;
	Tab3 *pDlg3;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSVMERGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void On32773();
	afx_msg void On32774();

	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_progressbar;
};

CString summaryfilename(CString Filename, CString version);
int FindReadingLength(CString tmp, CString arrfilename, TesterInformation *Information);

void FileDivision(CString FilePath, double divisionsize);

void ProcessWindowMessage();