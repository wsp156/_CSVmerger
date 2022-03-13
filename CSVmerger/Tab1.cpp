// Tab1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CSVmerger.h"
#include "CSVmergerDlg.h"
#include "Tab1.h"
#include "afxdialogex.h"
#include "CApplication.h"
#include "CRange.h"
#include "CFont0.h"
#include "CRanges.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "BringInformation.h"

// Tab1 대화 상자입니다.

IMPLEMENT_DYNAMIC(Tab1, CDialogEx)

Tab1::Tab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Tab1::~Tab1()
{
}

void Tab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Control(pDX, IDC_BUTTON1, m_addfile);
	DDX_Control(pDX, IDC_BUTTON2, m_deletefile);
	DDX_Control(pDX, IDC_BUTTON3, m_mergefile);
	DDX_Control(pDX, IDC_BUTTON4, m_addfolder);
	DDX_Control(pDX, IDC_BUTTON5, m_savefile);
	DDX_Control(pDX, IDC_BUTTON6, m_deleteall);
	DDX_Control(pDX, IDC_EDIT2, m_savepath);
	DDX_Control(pDX, IDC_EDIT1, m_filenum);
	DDX_Control(pDX, IDC_CHECK1, m_Checktime);
	DDX_Control(pDX, IDC_BUTTON10, m_mergeoverall);
	DDX_Control(pDX, IDC_EDIT3, m_division);
}


BEGIN_MESSAGE_MAP(Tab1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &Tab1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Tab1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &Tab1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Tab1::OnBnClickedButton5)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON6, &Tab1::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON10, &Tab1::OnBnClickedButton10)
END_MESSAGE_MAP()


// Tab1 메시지 처리기입니다.

//Merge버튼
void Tab1::OnBnClickedButton3()
{
	AfxGetApp()->DoWaitCursor(1);
	
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd(); 
	pMainDlg->m_progressbar.SetPos(0);

	//시간 정의
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString time_current = "";
	BOOL chk = m_Checktime.GetCheck();
	if (chk == TRUE)
		time_current.Format("%02d%02d%02d%02d%02d%02d_", st.wYear - 2000, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	else
		time_current = "";

	///////////////////////////////////////////////////////
	////////////Information Setting 가져오기///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	////////////File Version List 추출하기/////////////////
	///////////////////////////////////////////////////////
	int index = 0;
	int filenum = 0;
	ArrayVersionList arrfilename[200];
	int versioncount = 0;
	filenum = m_filelist.GetCount();

	if (Setting.ReadingMode == "FAST")
	{
		//전체 Version 리스트 array에 넣으면서 내용 저장
		while (index != filenum)
		{
			CStdioFile FileRead;
			CString ReadFilePath = "";
			CString ReadFileName = "";
			CString firstrow = "";
			CString currentversion = "";
			int currentversioncount = 0;
			bool different = false;
			int informationcount = 0;
			//첫줄 읽기
			m_filelist.GetText(index, ReadFilePath);
			FileRead.Open(ReadFilePath, CFile::modeRead);
			ReadFileName = FileRead.GetFileName();
			FileRead.ReadString(firstrow);
			FileRead.Close();

			//Nasca가 씌워져 있다면 실행 종료
			if (firstrow.Find("NASCA") != -1)
			{
				CString Nascafilenum = "";
				Nascafilenum.Format("%d", index + 1);
				NASCAMESSAGE;
				return;
			}

			//FileName에 따라 Version 분류
			currentversion = summaryfilename(ReadFileName, firstrow);
			informationcount = InformationCount(currentversion, Information);

			if (currentversion == "NA")
			{
			}
			else
			{
				//Version 중복 여부 확인
				while (currentversioncount <= versioncount)
				{
					if (arrfilename[currentversioncount].Version != currentversion)
					{
						different = true;
					}
					else
					{
						different = false;
						break;
					}
					currentversioncount++;
				}

				//새로운 Version 일 경우
				if (different == true)
				{
					CString tmp = "";
					CString str = "";
					FileRead.Open(ReadFilePath, CFile::modeRead);
					ReadFileName = FileRead.GetFileName();
					while (FileRead.ReadString(tmp))
					{
						str += tmp + "\n";
						ProcessWindowMessage();
					}
					FileRead.Close();

					arrfilename[versioncount].Version = currentversion;
					arrfilename[versioncount].Informationcount = informationcount;
					arrfilename[versioncount].data = str;
					versioncount++;
				}

				//이미 있는 version일 경우
				else if (different == false)
				{
					CString tmp = "";
					CString str = "";
					bool First = true;
					FileRead.Open(ReadFilePath, CFile::modeRead);
					ReadFileName = FileRead.GetFileName();
					while (FileRead.ReadString(tmp))
					{
						if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
							tmp = tmp;
						else if (tmp.Find(Information[informationcount].ColumnRowKey) != -1)
							tmp = tmp;
						else if (tmp.Find(Information[informationcount].USLRowKey) != -1)
							tmp = tmp;
						else if (tmp.Find(Information[informationcount].LSLRowKey) != -1)
							tmp = tmp;
						else
							str += tmp + "\n";
						First = false;
						ProcessWindowMessage();
					}
					FileRead.Close();
					arrfilename[currentversioncount].data += str;
				}
			}
			pMainDlg->m_progressbar.SetPos(100 / (float) filenum * (float) index);
			ProcessWindowMessage();
			index++;
		}

		//파일 쓰기
		for (int versionindex = 0; versionindex < versioncount; versionindex++)
		{
			CStdioFile FileWrite;
			CString WriteFilePath = "";

			//시간을 파일명에 추가
			SYSTEMTIME st;
			GetLocalTime(&st);
			CString time_current = "";
			BOOL chk = m_Checktime.GetCheck();
			if (chk == TRUE)
				time_current.Format("%02d%02d%02d%02d%02d%02d_", st.wYear - 2000, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			else
				time_current = "";

			m_savepath.GetWindowTextA(WriteFilePath);
			WriteFilePath += "\\" + time_current + "Mergefile_" + arrfilename[versionindex].Version;
			FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
			FileWrite.WriteString(arrfilename[versionindex].data);
			FileWrite.Close();

			//파일분할 있을 시
			CString strdivision = "";
			m_division.GetWindowTextA(strdivision);
			if (strdivision != "")
			{
				double division = 0;
				division = atof(strdivision);
				FileDivision(WriteFilePath, division);
			}
			ProcessWindowMessage();
		}
	}
	else
	{
		while (index != filenum)
		{
			CStdioFile FileRead;
			CString ReadFilePath = "";
			CString ReadFileName = "";
			CString firstrow = "";
			CString currentversion = "";
			int currentversioncount = 0;
			bool different = false;
			int informationcount = 0;
			//첫줄 읽기
			m_filelist.GetText(index, ReadFilePath);
			FileRead.Open(ReadFilePath, CFile::modeRead);
			ReadFileName = FileRead.GetFileName();
			FileRead.ReadString(firstrow);
			FileRead.Close();

			//Nasca가 씌워져 있다면 실행 종료
			if (firstrow.Find("NASCA") != -1)
			{
				CString Nascafilenum = "";
				Nascafilenum.Format("%d", index + 1);
				NASCAMESSAGE;
				return;
			}

			//FileName에 따라 Version 분류
			currentversion = summaryfilename(ReadFileName, firstrow);
			informationcount = InformationCount(currentversion, Information);
			if (currentversion == "NA")
			{
			}
			else
			{
				//Version 중복 여부 확인
				while (currentversioncount <= versioncount)
				{
					if (arrfilename[currentversioncount].Version != currentversion)
					{
						different = true;
					}
					else
					{
						different = false;
						break;
					}
					currentversioncount++;
				}

				//새로운 Version 일 경우
				if (different == true)
				{
					//array에 등록
					arrfilename[versioncount].Version = currentversion;
					arrfilename[versioncount].Informationcount = informationcount;

					//파일 읽기
					CString tmp = "";
					CString str = "";
					if (Setting.ReadingMode == "CHARACTER")
					{
						CFile cfile;
						cfile.Open(ReadFilePath, CFile::modeRead);
						ULONGLONG length = (ULONGLONG)cfile.GetLength();
						char *ch_temp1 = new char[length + 1];
						ZeroMemory(ch_temp1, length + 1);
						cfile.Read(ch_temp1, length);
						for (int i = 0; i < length; i++)
						{
							if (ch_temp1[i] == '\0')
								ch_temp1[i] = ' ';
							else if (ch_temp1[i] == '\r')
								ch_temp1[i] = ' ';
							ProcessWindowMessage();
						}
						str = ch_temp1;
						delete[] ch_temp1;
						cfile.Close();
					}
					else
					{
						FileRead.Open(ReadFilePath, CFile::modeRead);
						ReadFileName = FileRead.GetFileName();
						while (FileRead.ReadString(tmp))
						{
							str += tmp + "\n";
							ProcessWindowMessage();
						}
						FileRead.Close();
					}
					//파일 쓰기
					CStdioFile FileWrite;
					CString WriteFilePath = "";
					m_savepath.GetWindowTextA(WriteFilePath);
					WriteFilePath += "\\" + time_current + "Mergefile_" + arrfilename[versioncount].Version;
					FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
					FileWrite.WriteString(str);
					FileWrite.Close();
					versioncount++;
					ProcessWindowMessage();
				}

				//이미 있는 version일 경우
				else if (different == false)
				{
					CString str = "";

					if (Setting.ReadingMode == "CHARACTER")
					{
						//파일 null값 정화
						CString tmp1 = "";
						CString tmp2 = "";
						bool First = true;
						CFile cfile;
						cfile.Open(ReadFilePath, CFile::modeRead);
						ULONGLONG length = (ULONGLONG)cfile.GetLength();
						char *ch_temp1 = new char[length + 1];
						ZeroMemory(ch_temp1, length + 1);
						cfile.Read(ch_temp1, length);
						for (int i = 0; i < length; i++)
						{
							if (ch_temp1[i] == '\0')
								ch_temp1[i] = ' ';
							else if (ch_temp1[i] == '\r')
								ch_temp1[i] = ' ';
							ProcessWindowMessage();
						}
						tmp1 = ch_temp1;
						delete[] ch_temp1;
						cfile.Close();

						//파일 읽기
						while (tmp1.Find("\n") != -1)
						{
							tmp2 = tmp1.Left(tmp1.Find("\n"));
							if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].USLRowKey) != -1)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].LSLRowKey) != -1)
								tmp2 = "";
							else
								str += tmp2 + "\n";
							tmp1 = tmp1.Mid(tmp1.Find("\n") + 1);
							First = false;
							ProcessWindowMessage();
						}
					}
					else
					{
						//파일 읽기
						CString tmp = "";
						bool First = true;
						FileRead.Open(ReadFilePath, CFile::modeRead);
						ReadFileName = FileRead.GetFileName();
						while (FileRead.ReadString(tmp))
						{
							if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
								tmp = tmp;
							else if (tmp.Find(Information[informationcount].ColumnRowKey) != -1)
								tmp = tmp;
							else if (tmp.Find(Information[informationcount].USLRowKey) != -1)
								tmp = tmp;
							else if (tmp.Find(Information[informationcount].LSLRowKey) != -1)
								tmp = tmp;
							else
								str += tmp + "\n";
							First = false;
							ProcessWindowMessage();
						}
						FileRead.Close();
					}

					//파일 쓰기
					CStdioFile FileWrite;
					CString WriteFilePath = "";
					m_savepath.GetWindowTextA(WriteFilePath);
					WriteFilePath += "\\" + time_current + "Mergefile_" + currentversion;
					FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
					{
						FileWrite.SeekToEnd();
						FileWrite.WriteString(str);
					}
					FileWrite.Close();
				}
			}
			index++;
			pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
			ProcessWindowMessage();
		}
		for (int j = 0; j < versioncount; j++)
		{
			//파일분할 있을 시
			CString strdivision = "";
			m_division.GetWindowTextA(strdivision);
			if (strdivision != "")
			{
				double division = 0;
				division = atof(strdivision);
				CString WriteFilePath = "";
				m_savepath.GetWindowTextA(WriteFilePath);
				WriteFilePath += "\\" + time_current + arrfilename[j].Version;
				FileDivision(WriteFilePath, division);
			}
		}
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pMainDlg->m_progressbar.SetPos(100);
	AfxGetApp()->DoWaitCursor(-1);

	AfxMessageBox("Completed!");
}

//파일추가버튼
void Tab1::OnBnClickedButton1()
{
	CString PathRead;
	CStdioFile FileRead;
	CString filename;
	CString strfilelist;
	const int MaxFiles = 1000;
	const int BuffSize = (MaxFiles * (MAX_PATH + 1)) + 1;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, "CSV Files(*.csv)|*.csv|LOG Files(*.Log)|*.Log|All Files(*.*)|*.*|");

	dlg.GetOFN().lpstrFile = strfilelist.GetBuffer(BuffSize);
	dlg.GetOFN().nMaxFile = BuffSize;

	if (dlg.DoModal() == IDOK)
	{
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
		{
			PathRead = dlg.GetNextPathName(pos);
			m_filelist.AddString(PathRead);
		}
	}
	SetSavePath(PathRead);
	CreateHorizontalScroll();
	ShowFileNum();
}


//파일삭제버튼
void Tab1::OnBnClickedButton2()
{
	int count = m_filelist.GetCount();
	for (int i = count - 1; i >= 0; i--)
	{
		if (m_filelist.GetSel(i))
			m_filelist.DeleteString(i);
	}
	ShowFileNum();
}

//폴더추가버튼
void Tab1::OnBnClickedButton4()
{
	CString strInitPath = ".\\";
	CString strFolderPath;

	// 폴더 선택 다이얼로그
	CFolderPickerDialog Picker(strInitPath, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		strFolderPath = Picker.GetPathName();
		GetFileList(strFolderPath);
	}
	SetSavePath(strFolderPath);
	CreateHorizontalScroll();
	ShowFileNum();
}

//저장파일 경로 지정 버튼
void Tab1::OnBnClickedButton5()
{
	CString strInitPath = ".\\";

	// 폴더 선택 다이얼로그
	CFolderPickerDialog Picker(strInitPath, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString strPath = Picker.GetPathName();
		m_savepath.SetWindowTextA(strPath);
	}
}

//파일 갯수 나타내는 함수
void Tab1::ShowFileNum()
{
	CString filenum;
	filenum.Format("%d", m_filelist.GetCount());
	m_filenum.SetWindowTextA(filenum);
}


//가로스크롤 만드는 함수
void Tab1::CreateHorizontalScroll()
{
	CString str;
	CSize sz;
	int dx = 0;
	CDC *pDC = m_filelist.GetDC();

	for (int i = 0; i < m_filelist.GetCount(); i++)
	{
		m_filelist.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_filelist.ReleaseDC(pDC);

	if (m_filelist.GetHorizontalExtent() < dx)
	{
		m_filelist.SetHorizontalExtent(dx);
		ASSERT(m_filelist.GetHorizontalExtent() == dx);
	}
}

//파일 혹은 폴더 드래그드롭 기능
void Tab1::OnDropFiles(HDROP hDropInfo)
{
	UINT i = 0;    //Drag&Drop된 파일,폴더의 갯수저장
				   //드롭된 파일의 갯수
	UINT uCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	char buffer[256];  //Drag&Drop된 파일,폴더의 절대경로 저장

	for (i = 0; i < uCount; i++)
	{
		// 파일의 경로 얻어옴
		DragQueryFile(hDropInfo, i, buffer, 255);

		if (PathIsDirectory(buffer))
			GetFileList(buffer);
		else
			m_filelist.AddString(buffer);
	}
	::DragFinish(hDropInfo);
	SetSavePath(buffer);
	CreateHorizontalScroll();
	ShowFileNum();
}

void Tab1::GetFileList(CString strFolder) //폴더 내 파일들 불러오기
{
	CFileFind file;
	BOOL b = file.FindFile(strFolder + "\\*.*");  // CString strMusicFilter = ".MP3.OGG.WMA.WAV";   // 필터링 하고 싶으면 이렇게 쓰면 됨
	CString strFolderItem, strFileExt, strTempString;
	///////////////////////////////////////////////////////
	////////////Information Setting 가져오기///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);
	//////////////////////////////////////////////////
	while (b)
	{
		b = file.FindNextFile();
		if (file.IsDirectory() && !file.IsDots())   // 디렉토리 발견시 
		{
			strFolderItem = file.GetFilePath();
			GetFileList(strFolderItem);      // 하위폴더를 검색하기 위해 재귀호출 발생  
		}
		strFolderItem = file.GetFilePath();

		strFileExt = strFolderItem.Mid(strFolderItem.ReverseFind('.'));  // 확장자만 추출한다. 
		if (!file.IsDots())         // 파일 탐색 필터 정의에따라 해당 StringList에 추가
		{
			strFileExt.MakeUpper();       // strFileExt 에는 확장자 (.EXE 형태) 가 들어옴. 비교위해 대문자화 함
			if (file.IsDirectory())
				continue;    // 폴더만 남는 경우는 넣으면 안됨 
			for (int i = 0; i < Setting.ExtensionNum; i++)
			{
				CString extension = "." + Setting.FileExtension[i];
				if (strFolderItem.Find(extension) != -1)
					m_filelist.AddString(strFolderItem);
			}
		}
	}
}

//저장파일 경로 자동 지정 기능
void Tab1::SetSavePath(CString strsavePath)
{
	DWORD ret = GetFileAttributes(strsavePath);
	if (ret != INVALID_FILE_ATTRIBUTES)
	{
			strsavePath = strsavePath.Left(strsavePath.ReverseFind('\\'));
			m_savepath.SetWindowTextA(strsavePath);
	}
}


//전체 파일리스트 삭제 버튼
void Tab1::OnBnClickedButton6()
{
	m_filelist.ResetContent();
	ShowFileNum();
}

//Overall 파일 출력 버튼
void Tab1::OnBnClickedButton10()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd(); 
	pMainDlg->m_progressbar.SetPos(0);
	//시간을 파일명에 추가
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString time_current = "";
	BOOL chk = m_Checktime.GetCheck();
	if (chk == TRUE)
		time_current.Format("%02d%02d%02d%02d%02d%02d_", st.wYear - 2000, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	else
		time_current = "";
	///////////////////////////////////////////////////////
	////////////Information Setting 가져오기///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////
	////////////File Version List 추출하기/////////////////
	///////////////////////////////////////////////////////
	int index = 0;
	int filenum = 0;
	ArrayVersionList arrfilename[200];
	int versioncount = 0;
	filenum = m_filelist.GetCount();

	if (Setting.ReadingMode == "FAST")
	{
		//전체 Version 리스트 array에 넣으면서 내용 저장
		while (index != filenum)
		{
			CStdioFile FileRead;
			CString ReadFilePath = "";
			CString ReadFileName = "";
			CString firstrow = "";
			CString currentversion = "";
			int currentversioncount = 0;
			bool different = false;
			int informationcount = 0;
			//첫줄 읽기
			m_filelist.GetText(index, ReadFilePath);
			FileRead.Open(ReadFilePath, CFile::modeRead);
			ReadFileName = FileRead.GetFileName();
			FileRead.ReadString(firstrow);
			FileRead.Close();

			//Nasca가 씌워져 있다면 실행 종료
			if (firstrow.Find("NASCA") != -1)
			{
				CString Nascafilenum = "";
				Nascafilenum.Format("%d", index + 1);
				NASCAMESSAGE;
				return;
			}

			//FileName에 따라 Version 분류
			currentversion = summaryfilename(ReadFileName, firstrow);
			informationcount = InformationCount(currentversion, Information);
			if (currentversion == "NA")
			{
			}
			else
			{
				//Version 중복 여부 확인
				while (currentversioncount <= versioncount)
				{
					if (arrfilename[currentversioncount].Version != currentversion)
					{
						different = true;
					}
					else
					{
						different = false;
						break;
					}
					currentversioncount++;
				}

				//새로운 Version 일 경우
				if (different == true)
				{
					CString tmp1 = "";
					CString tmp2 = "";
					CString tmp3 = "";
					CString str = "";
					bool First = true;
					FileRead.Open(ReadFilePath, CFile::modeRead);
					ReadFileName = FileRead.GetFileName();
					while (FileRead.ReadString(tmp1))
					{
						tmp2 = tmp1;
						tmp3 = tmp2;
						int		stringcount = 0;
						if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
							tmp2 = tmp2;
						else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
						{
							arrfilename[versioncount].OverallCommaCount = FindReadingLength(tmp2, currentversion, Information);
							for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
							{
								stringcount += tmp3.Find(",");
								tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
							}
							stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
							tmp2 = tmp2.Left(stringcount);
						}
						else
						{
							for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
							{
								stringcount += tmp3.Find(",");
								tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
							}
							stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
							tmp2 = tmp2.Left(stringcount);
						}
						str += tmp2 + "\n";
						First = false;
						ProcessWindowMessage();
					}
					FileRead.Close();

					arrfilename[versioncount].Version = currentversion;
					arrfilename[versioncount].Informationcount = informationcount;
					arrfilename[versioncount].data = str;
					versioncount++;
				}

				//이미 있는 version일 경우
				else if (different == false)
				{
					CString tmp1 = "";
					CString tmp2 = "";
					CString tmp3 = "";
					CString str = "";
					bool First = true;
					FileRead.Open(ReadFilePath, CFile::modeRead);
					ReadFileName = FileRead.GetFileName();
					while (FileRead.ReadString(tmp1))
					{
						int stringcount = 0;
						tmp2 = tmp1;
						tmp3 = tmp2;
						if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
							tmp2 = "";
						else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
							arrfilename[versioncount].OverallCommaCount = FindReadingLength(tmp2, currentversion, Information);
						else if (tmp2.Find(Information[informationcount].USLRowKey) != -1)
							tmp2 = "";
						else if (tmp2.Find(Information[informationcount].LSLRowKey) != -1)
							tmp2 = "";
						else
						{
							for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
							{
								stringcount += tmp3.Find(",");
								tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
							}
							stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
							tmp2 = tmp2.Left(stringcount);
							str += tmp2 + "\n";
						}
						First = false;
						ProcessWindowMessage();
					}
					FileRead.Close();
					arrfilename[currentversioncount].data += str;
				}
			}
			pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
			ProcessWindowMessage();
			index++;
		}

		//파일 쓰기
		for (int versionindex = 0; versionindex < versioncount; versionindex++)
		{
			CStdioFile FileWrite;
			CString WriteFilePath = "";

			//시간을 파일명에 추가
			SYSTEMTIME st;
			GetLocalTime(&st);
			CString time_current = "";
			BOOL chk = m_Checktime.GetCheck();
			if (chk == TRUE)
				time_current.Format("%02d%02d%02d%02d%02d%02d_", st.wYear - 2000, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			else
				time_current = "";

			m_savepath.GetWindowTextA(WriteFilePath);
			WriteFilePath += "\\" + time_current + "Overallfile_" + arrfilename[versionindex].Version;
			FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
			FileWrite.WriteString(arrfilename[versionindex].data);
			FileWrite.Close();
			//파일분할 있을 시
			CString strdivision = "";
			m_division.GetWindowTextA(strdivision);
			if (strdivision != "")
			{
				double division = 0;
				division = atof(strdivision);
				FileDivision(WriteFilePath, division);
			}
			ProcessWindowMessage();
		}
	}
	else {
		while (index != filenum)
		{
			ProcessWindowMessage();
			CStdioFile FileRead;
			CString ReadFilePath = "";
			CString ReadFileName = "";
			CString firstrow = "";
			CString currentversion = "";
			int informationcount = 0;
			int	currentversioncount = 0;
			bool different = false;

			//첫줄 읽기
			m_filelist.GetText(index, ReadFilePath);
			FileRead.Open(ReadFilePath, CFile::modeRead);
			ReadFileName = FileRead.GetFileName();
			FileRead.ReadString(firstrow);
			FileRead.Close();

			//Nasca가 씌워져 있다면 실행 종료
			if (firstrow.Find("NASCA") != -1)
			{
				CString Nascafilenum = "";
				Nascafilenum.Format("%d", index + 1);
				NASCAMESSAGE;
				return;
			}

			//FileName에 따라 Version 분류
			currentversion = summaryfilename(ReadFileName, firstrow);
			informationcount = InformationCount(currentversion, Information);

			if (currentversion == "NA")
			{
			}
			else
			{
				//Version 중복 여부 확인
				for (currentversioncount = 0; currentversioncount <= versioncount; currentversioncount++)
				{
					if (arrfilename[currentversioncount].Version != currentversion)
					{
						different = true;
					}
					else
					{
						different = false;
						break;
					}
				}

				//새로운 Version 일 경우
				//FindReadingLength 함수 사용해서 comma 갯수 가져오기
				if (different == true)
				{
					//array에 등록
					arrfilename[versioncount].Version = currentversion;
					arrfilename[versioncount].Informationcount = informationcount;

					//파일 읽기
					CString tmp1 = "";
					CString tmp2 = "";
					CString tmp3 = "";
					CString str = "";
					if (Setting.ReadingMode == "CHARACTER")
					{
						bool First = true;
						CFile cfile;
						cfile.Open(ReadFilePath, CFile::modeRead);
						ULONGLONG length = (ULONGLONG)cfile.GetLength();
						char *ch_temp1 = new char[length + 1];
						ZeroMemory(ch_temp1, length + 1);
						cfile.Read(ch_temp1, length);
						for (int i = 0; i < length; i++)
						{
							if (ch_temp1[i] == '\0')
								ch_temp1[i] = ' ';
							else if (ch_temp1[i] == '\r')
								ch_temp1[i] = ' ';
						}
						tmp1 = ch_temp1;
						delete[] ch_temp1;
						cfile.Close();

						while (tmp1.Find("\n") != -1)
						{
							tmp2 = tmp1.Left(tmp1.Find("\n"));
							tmp3 = tmp2;
							int		stringcount = 0;
							if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
								tmp2 = tmp2;
							else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
							{
								arrfilename[versioncount].OverallCommaCount = FindReadingLength(tmp2, currentversion, Information);
								for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
								{
									stringcount += tmp3.Find(",");
									tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
								}
								stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
								tmp2 = tmp2.Left(stringcount);
							}
							else
							{
								for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
								{
									stringcount += tmp3.Find(",");
									tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
								}
								stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
								tmp2 = tmp2.Left(stringcount);
							}
							str += tmp2 + "\n";
							tmp1 = tmp1.Mid(tmp1.Find("\n") + 1);
							First = false;
							ProcessWindowMessage();
						}
					}
					else
					{
						bool First = true;
						FileRead.Open(ReadFilePath, CFile::modeRead);
						ReadFileName = FileRead.GetFileName();
						while (FileRead.ReadString(tmp1))
						{
							tmp2 = tmp1;
							tmp3 = tmp2;
							int		stringcount = 0;
							if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
								tmp2 = tmp2;
							else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
							{
								arrfilename[versioncount].OverallCommaCount = FindReadingLength(tmp2, arrfilename[versioncount].Version, Information);
								for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
								{
									stringcount += tmp3.Find(",");
									tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
								}
								stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
								tmp2 = tmp2.Left(stringcount);
							}
							else
							{
								for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
								{
									stringcount += tmp3.Find(",");
									tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
								}
								stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
								tmp2 = tmp2.Left(stringcount);
							}
							str += tmp2 + "\n";
							First = false;
							ProcessWindowMessage();
						}
						FileRead.Close();
					}

					//파일 쓰기
					CStdioFile FileWrite;
					CString WriteFilePath = "";
					m_savepath.GetWindowTextA(WriteFilePath);
					WriteFilePath += "\\" + time_current + "Overallfile_" + arrfilename[versioncount].Version;
					FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeWrite);
					FileWrite.WriteString(str);
					FileWrite.Close();
					versioncount++;
				}

				//이미 있는 version일 경우
				else if (different == false)
				{
					CString tmp1 = "";
					CString tmp2 = "";
					CString tmp3 = "";
					CString str = "";
					if (Setting.ReadingMode == "CHARACTER")
					{
						//파일 null값 정화
						bool First = true;
						CFile cfile;
						cfile.Open(ReadFilePath, CFile::modeRead);
						ULONGLONG length = (ULONGLONG)cfile.GetLength();
						char *ch_temp1 = new char[length + 1];
						ZeroMemory(ch_temp1, length + 1);
						cfile.Read(ch_temp1, length);
						for (int i = 0; i < length; i++)
						{
							if (ch_temp1[i] == '\0')
								ch_temp1[i] = ' ';
							else if (ch_temp1[i] == '\r')
								ch_temp1[i] = ' ';
						}
						tmp1 = ch_temp1;
						delete[] ch_temp1;
						cfile.Close();

						//파일 읽기
						while (tmp1.Find("\n") != -1)
						{
							int stringcount = 0;
							tmp2 = tmp1.Left(tmp1.Find("\n"));
							tmp3 = tmp2;
							if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
								arrfilename[versioncount].OverallCommaCount = FindReadingLength(tmp2, currentversion, Information);
							else if (tmp2.Find(Information[informationcount].USLRowKey) != -1)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].LSLRowKey) != -1)
								tmp2 = "";
							else
							{
								for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
								{
									stringcount += tmp3.Find(",");
									tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
								}
								stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
								tmp2 = tmp2.Left(stringcount);
								str += tmp2 + "\n";
							}
							tmp1 = tmp1.Mid(tmp1.Find("\n") + 1);
							First = false;
							ProcessWindowMessage();
						}
					}
					else
					{
						bool First = true;
						FileRead.Open(ReadFilePath, CFile::modeRead);
						ReadFileName = FileRead.GetFileName();
						while (FileRead.ReadString(tmp1))
						{
							int stringcount = 0;
							tmp2 = tmp1;
							tmp3 = tmp2;
							if (tmp2.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].ColumnRowKey) != -1)
								arrfilename[versioncount].OverallCommaCount = FindReadingLength(tmp2, currentversion, Information);
							else if (tmp2.Find(Information[informationcount].USLRowKey) != -1)
								tmp2 = "";
							else if (tmp2.Find(Information[informationcount].LSLRowKey) != -1)
								tmp2 = "";
							else
							{
								for (int comma = 0; comma < arrfilename[versioncount].OverallCommaCount; comma++)
								{
									stringcount += tmp3.Find(",");
									tmp3 = tmp3.Mid(tmp3.Find(",") + 1);
								}
								stringcount = stringcount + arrfilename[versioncount].OverallCommaCount - 1;
								tmp2 = tmp2.Left(stringcount);
								str += tmp2 + "\n";
							}
							First = false;
							ProcessWindowMessage();
						}
						FileRead.Close();
					}

					//파일 쓰기
					CStdioFile FileWrite;
					CString WriteFilePath = "";
					m_savepath.GetWindowTextA(WriteFilePath);
					WriteFilePath += "\\" + time_current + "Overallfile_" + currentversion;
					FileWrite.Open(WriteFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
					{
						FileWrite.SeekToEnd();
						FileWrite.WriteString(str);
					}
					FileWrite.Close();
				}
			}
			pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
			ProcessWindowMessage();
			index++;
		}
		for (int j = 0; j < versioncount; j++)
		{
			//파일분할 있을 시
			CString strdivision = "";
			m_division.GetWindowTextA(strdivision);
			if (strdivision != "")
			{
				double division = 0;
				division = atof(strdivision);
				CString WriteFilePath = "";
				m_savepath.GetWindowTextA(WriteFilePath);
				WriteFilePath += "\\" + time_current + arrfilename[j].Version;
				FileDivision(WriteFilePath, division);
			}
		}
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pMainDlg->m_progressbar.SetPos(100);
	AfxGetApp()->DoWaitCursor(-1);
	AfxMessageBox("Completed!");
}
