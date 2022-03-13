#pragma once
#include "afxwin.h"

// Tab2 대화 상자입니다.

class Tab2 : public CDialogEx
{
	DECLARE_DYNAMIC(Tab2)

public:
	Tab2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Tab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	void ShowFileNum();
	void GetFileList(CString strFolder);
	CString makefolder(CString strloadpath, CString arrfilename, CString type);
	CEdit m_savepath;
	CButton m_savefile;
	CButton m_mergefile;
	CListBox m_filelist;
	CEdit m_filenum;
	CEdit m_division;
};
