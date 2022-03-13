#pragma once
#include "afxwin.h"

#define NASCAMESSAGE	AfxMessageBox(Nascafilenum + "th file was covered by NASCA. Please remove NASCA.")
// Tab3 대화 상자입니다.

class Tab3 : public CDialogEx
{
	DECLARE_DYNAMIC(Tab3)

public:
	Tab3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Tab3();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_filenum;
	CListBox m_filelist;
	CButton m_Denasca;
	CButton m_deleteNA;
	CButton m_changedouble;
	afx_msg void OnBnClickedButton1();
	CButton m_addfile;
	CButton m_addfolder;
	CButton m_deletefile;
	CButton m_deleteall;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	void ShowFileNum();
	void CreateHorizontalScroll();
	void GetFileList(CString strFolder);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString NASCAfile(CString strLoadPath);
	CString ChangeDouble(CString strLoadPath);
	CButton m_DeletingSpec;
	afx_msg void OnBnClickedButton9();
	CEdit m_configname;
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	void CreatingResultDirectory(CString *FilePath, CString Type);
	CButton m_Check2;
	afx_msg void OnBnClickedButton12();
	CEdit m_savepath;
	afx_msg void OnBnClickedButton13();
	void SetSavePath(CString strsavePath);
	CButton m_check_cover;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton14();
};
