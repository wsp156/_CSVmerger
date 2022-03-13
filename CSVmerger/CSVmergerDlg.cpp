
// CSVmergerDlg.cpp : 구현 파일
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


// CCSVmergerDlg 대화 상자



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


// CCSVmergerDlg 메시지 처리기

BOOL CCSVmergerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCSVmergerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCSVmergerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CCSVmergerDlg::On32773()  //메뉴바 파일 출력
{
}


void CCSVmergerDlg::On32774() //메뉴바 종료
{
	::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
}

void CCSVmergerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	/////////////////////INI 파일 가져오기////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	//////////////////////////////////////////////////////////////////////////
	/////////////////////파일 구분 가져오기///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CString version1result = "";
	CString version2result = "";
	CString version3result = "";
	CString modelresult = "";
	CString result = "";
	int count = MAXINFORMATIONNUM;

	//Tester 종류 찾기
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
		//version1 추출
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
		//version2 추출
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
		//version3 추출
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
		//Model 추출
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

//Overall 추출 길이 찾기
int FindReadingLength(CString tmp, CString arrfilename, TesterInformation * Information)
{
	//////////////////////////////////////////////////////////////////////////
	/////////////////////파일 구분 가져오기///////////////////////////////////
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

