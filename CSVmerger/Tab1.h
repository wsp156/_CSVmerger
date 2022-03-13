#pragma once
#include "afxwin.h"


// Tab1 대화 상자입니다.

class Tab1 : public CDialogEx
{
	DECLARE_DYNAMIC(Tab1)

public:
	Tab1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Tab1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_filelist;
	CButton m_addfile;
	CButton m_deletefile;
	CButton m_mergefile;
	CButton m_addfolder;
	CButton m_savefile;
	CButton m_deleteall;
	CEdit m_savepath;
	CEdit m_filenum;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	void GetFileList(CString strFolder);
	afx_msg void OnBnClickedButton5();
	void SetSavePath(CString strsavePath);
	void ShowFileNum();
	void CreateHorizontalScroll();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton10();
	CButton m_Checktime;
	CButton m_mergeoverall;
	CEdit m_division;
};
