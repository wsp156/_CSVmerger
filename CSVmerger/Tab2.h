#pragma once
#include "afxwin.h"

// Tab2 ��ȭ �����Դϴ�.

class Tab2 : public CDialogEx
{
	DECLARE_DYNAMIC(Tab2)

public:
	Tab2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Tab2();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
