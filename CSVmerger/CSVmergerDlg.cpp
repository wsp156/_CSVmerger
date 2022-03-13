
// CSVmergerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "CSVmerger.h"
#include "CSVmergerDlg.h"
#include "afxdialogex.h"
#include "afxdisp.h"
#include "Tab1.h"
#include "Tab2.h"
#include "Tab3.h"
#include "SettingDialog.h"
#include "BringInformation.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


// CCSVmergerDlg ��ȭ ����



CCSVmergerDlg::CCSVmergerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CSVMERGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCSVmergerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressbar);
}

BEGIN_MESSAGE_MAP(CCSVmergerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_32773, &CCSVmergerDlg::On32773)
	ON_COMMAND(ID_32774, &CCSVmergerDlg::On32774)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CCSVmergerDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CCSVmergerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCSVmergerDlg �޽��� ó����

BOOL CCSVmergerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetWindowTextA("CSVmerger_v4.8");
	m_Tab.InsertItem(0, "Select Merge");
	m_Tab.InsertItem(1, "Smart Merge");
	m_Tab.InsertItem(2, "Edit File ");
	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	pDlg1 = new Tab1;
	pDlg1->Create(IDD_DIALOG1, &m_Tab);
	pDlg1->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg1->ShowWindow(SW_SHOW);

	pDlg2 = new Tab2;
	pDlg2->Create(IDD_DIALOG2, &m_Tab);
	pDlg2->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg2->ShowWindow(SW_HIDE);

	pDlg3 = new Tab3;
	pDlg3->Create(IDD_DIALOG3, &m_Tab);
	pDlg3->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg3->ShowWindow(SW_HIDE);

	m_progressbar.SetRange(0,100);
	m_progressbar.SetPos(0);
	m_progressbar.SetBarColor(RGB(0,255,0));
	m_progressbar.SetBkColor(RGB(255, 255, 255));
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CCSVmergerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CCSVmergerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CCSVmergerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CCSVmergerDlg::On32773()  //�޴��� ���� ���
{
}


void CCSVmergerDlg::On32774() //�޴��� ����
{
	::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
}

void CCSVmergerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
		case 0:
			pDlg1->ShowWindow(SW_SHOW);
			pDlg2->ShowWindow(SW_HIDE);
			pDlg3->ShowWindow(SW_HIDE);
			break;
		
		case 1:
			pDlg1->ShowWindow(SW_HIDE);
			pDlg2->ShowWindow(SW_SHOW);
			pDlg3->ShowWindow(SW_HIDE);
			break;

		case 2:
			pDlg1->ShowWindow(SW_HIDE);
			pDlg2->ShowWindow(SW_HIDE);
			pDlg3->ShowWindow(SW_SHOW);
			break;
	}

	*pResult = 0;
}


CString summaryfilename(CString Filename, CString version)
{
	//////////////////////////////////////////////////////////////////////////
	/////////////////////INI ���� ��������////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	//////////////////////////////////////////////////////////////////////////
	/////////////////////���� ���� ��������///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CString version1result = "";
	CString version2result = "";
	CString version3result = "";
	CString modelresult = "";
	CString result = "";
	int count = MAXINFORMATIONNUM;

	//Tester ���� ã��
	for (int i = 0; i < MAXINFORMATIONNUM; i++)
	{
		if (Information[i].TesterKey == "NA")
			break;
		else{
			switch (Information[i].TesterLocation)
			{
			case CHECKFILENAME:
				if(Filename.Find(Information[i].TesterKey) != -1 || Filename.Find(Information[i].kinds) != -1)
					count = i;
				break;
			case CHECKFIRSTROW:
				if (version.Find(Information[i].TesterKey) != -1 || Filename.Find(Information[i].kinds) != -1)
					count = i;
				break;
			}
			if (count != MAXINFORMATIONNUM)
				break;
		}
	}

	if (count == MAXINFORMATIONNUM)
		result = "NA";
	else {
		//version1 ����
		if (Information[count].Version1Key != "NA")
		{
			switch (Information[count].Version1Location)
			{
			case CHECKFILENAME:
				if (Filename.Find(Information[count].Version1Key) != -1)
					version1result = Filename.Mid(Filename.Find(Information[count].Version1Key));
				else
					version1result = "";
				if (version1result.Find("_") != -1)
					version1result = version1result.Left(version1result.Find("_"));
				else if (version1result.Find(" ") != -1)
					version1result = version1result.Left(version1result.Find(" "));
				else if (version1result.Find("(") != -1)
					version1result = version1result.Left(version1result.Find("("));
				else
				{
					for (int i = 0; i<Setting.ExtensionNum; i++)
					{
						CString extension = "." + Setting.FileExtension[i];
						if (version1result.Find(extension) != -1)
							version1result = version1result.Left(version1result.Find(extension));
					}
				}
				/*
				else if (version1result.Find(".txt") != -1)
					version1result = version1result.Left(version1result.Find(".txt"));
				else if (version1result.Find(".Log") != -1)
					version1result = version1result.Left(version1result.Find(".Log"));
				else
					version1result = version1result.Left(version1result.Find(".csv"));
				*/
				break;
			case CHECKFIRSTROW:
				if (version.Find(Information[count].Version1Key) != -1)
					version1result = version.Mid(version.Find(Information[count].Version1Key));
				else
					version1result = "";
				if (version1result.Find(",") != -1)
					version1result = version1result.Left(version1result.Find(","));
				else
					version1result = version1result;
				break;
			}
			version1result = "_" + version1result;
		}
		else
			version1result = "";
		//version2 ����
		if (Information[count].Version2Key != "NA")
		{
			switch (Information[count].Version2Location)
			{
			case CHECKFILENAME:
				if (Filename.Find(Information[count].Version2Key) != -1)
					version2result = Filename.Mid(Filename.Find(Information[count].Version2Key));
				else
					version2result = "";
				if (version2result.Find("_") != -1)
					version2result = version2result.Left(version2result.Find("_"));
				else if (version2result.Find(" ") != -1)
					version2result = version2result.Left(version2result.Find(" "));
				else if (version2result.Find("(") != -1)
					version2result = version2result.Left(version2result.Find("("));
				else
				{
					for (int i = 0; i<Setting.ExtensionNum; i++)
					{
						CString extension = "." + Setting.FileExtension[i];
						if (version2result.Find(extension) != -1)
							version2result = version2result.Left(version2result.Find(extension));
					}
				}
				/*
				else if (version2result.Find(".txt") != -1)
				version2result = version2result.Left(version2result.Find(".txt"));
				else if (version2result.Find(".Log") != -1)
				version2result = version2result.Left(version2result.Find(".Log"));
				else
				version2result = version2result.Left(version2result.Find(".csv"));
				*/
				break;
			case CHECKFIRSTROW:
				if (version.Find(Information[count].Version2Key) != -1)
					version2result = version.Mid(version.Find(Information[count].Version2Key));
				else
					version2result = "";
				if (version2result.Find(",") != -1)
					version2result = version2result.Left(version2result.Find(","));
				else
					version2result = version2result;
				break;
			}
			version2result = "_" + version2result;
		}
		else
			version2result = "";
		//version3 ����
		if (Information[count].Version3Key != "NA")
		{
			switch (Information[count].Version3Location)
			{
			case CHECKFILENAME:
				if (Filename.Find(Information[count].Version3Key) != -1)
					version3result = Filename.Mid(Filename.Find(Information[count].Version3Key));
				else
					version3result = "";
				if (version3result.Find("_") != -1)
					version3result = version3result.Left(version3result.Find("_"));
				else if (version3result.Find(" ") != -1)
					version3result = version3result.Left(version3result.Find(" "));
				else if (version3result.Find("(") != -1)
					version3result = version3result.Left(version3result.Find("("));
				else
				{
					for (int i = 0; i<Setting.ExtensionNum; i++)
					{
						CString extension = "." + Setting.FileExtension[i];
						if (version3result.Find(extension) != -1)
							version3result = version3result.Left(version3result.Find(extension));
					}
				}
				/*
				else if (version3result.Find(".txt") != -1)
				version3result = version3result.Left(version3result.Find(".txt"));
				else if (version3result.Find(".Log") != -1)
				version3result = version3result.Left(version3result.Find(".Log"));
				else
				version3result = version3result.Left(version3result.Find(".csv"));
				*/
				break;
			case CHECKFIRSTROW:
				if (version.Find(Information[count].Version3Key) != -1)
					version3result = version.Mid(version.Find(Information[count].Version3Key));
				else
					version3result = "";
				if (version3result.Find(",") != -1)
					version3result = version3result.Left(version3result.Find(","));
				else
					version3result = version3result;
				break;
			}
			version3result = "_" + version3result;
		}
		else
			version3result = "";
		//Model ����
		if (Information[count].ModelKey != "NA")
		{
			switch (Information[count].ModelLocation)
			{
			case CHECKFILENAME:
				if (Filename.Find(Information[count].ModelKey) != -1)
					modelresult = Filename.Mid(Filename.Find(Information[count].ModelKey));
				else
					modelresult = "";
				if (modelresult.Find("_") != -1)
					modelresult = modelresult.Left(modelresult.Find("_"));
				else if (modelresult.Find("(") != -1)
					modelresult = modelresult.Left(modelresult.Find("("));
				else if (modelresult.Find(" ") != -1)
					modelresult = modelresult.Left(modelresult.Find(" "));
				else
				{
					for (int i = 0; i<Setting.ExtensionNum; i++)
					{
						CString extension = "." + Setting.FileExtension[i];
						if (modelresult.Find(extension) != -1)
							modelresult = modelresult.Left(modelresult.Find(extension));
					}
				}
				/*
				else if (version1result.Find(".txt") != -1)
				modelresult = modelresult.Left(modelresult.Find(".txt"));
				else if (version1result.Find(".Log") != -1)
				modelresult = modelresult.Left(modelresult.Find(".Log"));
				else
				modelresult = modelresult.Left(modelresult.Find(".csv"));
				*/
				break;
			case CHECKFIRSTROW:
				if (version.Find(Information[count].ModelKey) != -1)
					modelresult = version.Mid(version.Find(Information[count].ModelKey));
				else
					modelresult = "";
				if (modelresult.Find(",") != -1)
					modelresult = modelresult.Left(modelresult.Find(","));
				else
					modelresult = modelresult;
				break;
			}
			modelresult = "_"+ modelresult;
		}
		else
			modelresult = "";
		version1result.Replace(":","");
		version2result.Replace(":", "");
		version3result.Replace(":", "");
		modelresult.Replace(":", "");
		result = Information[count].kinds + modelresult + version1result + version2result + version3result + ".csv";
	}
	return result;
}

//Overall ���� ���� ã��
int FindReadingLength(CString tmp, CString arrfilename, TesterInformation * Information)
{
	//////////////////////////////////////////////////////////////////////////
	/////////////////////���� ���� ��������///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	int count = MAXINFORMATIONNUM;
	for (int i = 0; i < MAXINFORMATIONNUM; i++)
	{
		if (Information[i].TesterKey == "NA")
			break;
		else if (arrfilename.Find(Information[i].kinds) != -1)
			count = i;
	}

	int result = 0;
	CString tmp1 = "";

	if (count == MAXINFORMATIONNUM)
		result = 0;
	else
	{
		tmp1 = tmp.Left(tmp.Find(Information[count].OverallKey));
		result = tmp1.Remove(',') + 1;
	}

	return result;
}

void FileDivision(CString FilePath, double divisionsize)
{
	WIN32_FIND_DATA fi;
	FindFirstFile(FilePath, &fi);
	DWORD LocalFilesize = fi.nFileSizeHigh + fi.nFileSizeLow;
	divisionsize = divisionsize * 1000000;

	if (divisionsize > LocalFilesize)
		return;

	CString str[MAXDIVISIONFILENUM] = { "" };
	CString tmp = "";
	CString header = "";
	int rowindex = 0;
	int fileindex = 1;
	CStdioFile FileRead;
	CStdioFile FileWrite;
	CString WriteFilePath = "";
	double stringlength = 0;
	CString strfileindex = "";

	FileRead.Open(FilePath, CFile::modeRead);

	while (FileRead.ReadString(tmp))
	{
		strfileindex.Format("%i", fileindex);
		if (rowindex < 4)
		{
			header += tmp + "\n";
			rowindex++;
		}
		else
		{
			str[fileindex] += tmp + "\n";
			rowindex++;
		}

		/*
		WriteFilePath = FilePath;
		WriteFilePath.Replace(".csv", "_division_" + strfileindex + ".csv");
		FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(header + str[fileindex]);
		FileWrite.Close();

		FindFirstFile(WriteFilePath, &fi);
		LocalFilesize = fi.nFileSizeHigh + fi.nFileSizeLow;

		if (LocalFilesize > divisionsize)
		{
			fileindex++;
		}
		*/

		stringlength += tmp.GetLength();
		if (stringlength > divisionsize)
		{
			WriteFilePath = FilePath;
			WriteFilePath.Replace(".csv", "_division_" + strfileindex + ".csv");
			FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
			FileWrite.WriteString(header + str[fileindex]);
			FileWrite.Close();
			stringlength = 0;
			fileindex++;
		}
	}

	WriteFilePath = FilePath;
	WriteFilePath.Replace(".csv", "_division_" + strfileindex + ".csv");
	FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
	FileWrite.WriteString(header + str[fileindex]);
	FileWrite.Close();

	FileRead.Close();

}

void CCSVmergerDlg::OnBnClickedButton1()
{
	SettingDialog	dlg;
	dlg.DoModal();
}

void ProcessWindowMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}

