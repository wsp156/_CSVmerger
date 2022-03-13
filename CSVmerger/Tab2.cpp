// Tab2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CSVmerger.h"
#include "CSVmergerDlg.h"
#include "Tab2.h"
#include "afxdialogex.h"
#include "BringInformation.h"


// Tab2 대화 상자입니다.

IMPLEMENT_DYNAMIC(Tab2, CDialogEx)

Tab2::Tab2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Tab2::~Tab2()
{
}

void Tab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	//  DDX_Control(pDX, IDC_EDIT2, m_savefile);
	DDX_Control(pDX, IDC_EDIT2, m_savepath);
	DDX_Control(pDX, IDC_BUTTON3, m_savefile);
	DDX_Control(pDX, IDC_BUTTON5, m_mergefile);
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Control(pDX, IDC_EDIT1, m_filenum);
	DDX_Control(pDX, IDC_EDIT3, m_division);
}


BEGIN_MESSAGE_MAP(Tab2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &Tab2::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &Tab2::OnBnClickedButton3)
END_MESSAGE_MAP()


// Tab2 메시지 처리기입니다.



//실행폴더경로버튼
void Tab2::OnBnClickedButton5()
{
	m_filelist.ResetContent();
	CString strInitPath = ".\\";
	// 폴더 선택 다이얼로그
	CFolderPickerDialog Picker(strInitPath, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString strPath = Picker.GetPathName();
		m_savepath.SetWindowTextA(strPath);
		GetFileList(strPath);
	}
	ShowFileNum();
}

//출력 파일
void Tab2::OnBnClickedButton3()
{
	AfxGetApp()->DoWaitCursor(1);
	TesterInformation Information[MAXINFORMATIONNUM];
	CStdioFile FileRead;
	CStdioFile FileWrite;
	CString ReadFilePath = "";
	CString WriteFilePath = "";
	CString MergingFolderPath = "";
	CString ReadFileName = "";
	CString ReadFolderName = "";
	CString ReadNowName = "";

	int index = 0;
	int filenum = 0;
	int folderpathlength = 0;

	CString tmp;
	CString str;
	CString arrfilename[200] = { "" };

	int i = 0;
	int k = 0;
	bool different = false;

	filenum = m_filelist.GetCount();
	m_savepath.GetWindowTextA(MergingFolderPath);


	while (index != filenum) {
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		ReadFileName = FileRead.GetFileName();
		FileRead.ReadString(tmp);
		FileRead.Close();

		//Nasca가 씌워져 있는 파일이 있다면 중단
		if (tmp.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			AfxMessageBox(Nascafilenum + "번째 파일이 NASCA에 걸렸습니다. Nasca를 제거한 뒤 실행해 주세요");
			return;
		}

		//FileName에 따라 Merge할 파일 분류
		folderpathlength = ReadFilePath.GetLength()-MergingFolderPath.GetLength();
		ReadFolderName = ReadFilePath.Right(folderpathlength-1);
		tmp = ReadFolderName.Left(ReadFolderName.Find('\\')) + "_Auto_Merge_"+ summaryfilename(ReadFileName, tmp);
		for (k = 0; k <= i; k++)
		{
			if (arrfilename[k] != tmp)
			{
				different = true;
			}
			else
			{
				different = false;
				break;
			}
		}
		if (different == true && tmp != "NO")
		{
			arrfilename[i] = tmp;
			i++;
		}
		index++;
		tmp = "";
	}

	for (int j = 0; j < i; j++)
	{
		index = 0;
		bool firstfile = true;
		bool startwrite = false;
		bool firstrow = true;
		while (index != filenum)
		{
			m_filelist.GetText(index, ReadFilePath);
			FileRead.Open(ReadFilePath, CFile::modeRead);
			ReadFileName = FileRead.GetFileName();

			firstrow = true;
			int skip = 0;
			while (FileRead.ReadString(tmp))
			{
				if (firstrow == true)
				{
					folderpathlength = ReadFilePath.GetLength() - MergingFolderPath.GetLength();
					ReadFolderName = ReadFilePath.Right(folderpathlength-1);
					ReadNowName = ReadFolderName.Left(ReadFolderName.Find('\\')) + "_Auto_Merge_" + summaryfilename(ReadFileName, tmp);

					if (ReadNowName != arrfilename[j])
						break;
				}

				if (ReadFileName.Find(".Log") != -1)
					firstfile = true;

				if ((firstfile == false && firstrow == true) || skip > 0)
				{
					skip += 1;
					firstrow = false;
					if (skip == 4)
					{
						skip = 0;
					}
					continue;
				}

				str += tmp + "\n";
				startwrite = true;
				firstrow = false;
			}
			FileRead.Close();

			if (startwrite == true)
				firstfile = false;

			index += 1;
		}

		m_savepath.GetWindowTextA(WriteFilePath);

		CString AutoMergePath = "";
		AutoMergePath = makefolder(WriteFilePath, arrfilename[j],"Auto_Merge");//내부 폴더 만들기
		AutoMergePath += "\\" + arrfilename[j];
		FileWrite.Open(AutoMergePath, CFile::modeCreate | CFile::modeWrite );
		FileWrite.WriteString(str);
		FileWrite.Close();

		//파일분할 있을 시
		CString strdivision = "";
		m_division.GetWindowTextA(strdivision);
		if (strdivision != "")
		{
			double division = 0;
			division = atof(strdivision);
			FileDivision(AutoMergePath, division);
		}

		str = "";


		//Overall 만들기
		CStdioFile FileRead2;
		CStdioFile FileWrite2;
		FileRead2.Open(AutoMergePath, CFile::modeRead);
		int CommaNumber= 1;
		int StartPoint = 0;
		int AdditionalCommaNumber = 0;
		while (FileRead2.ReadString(tmp))
		{
			if (tmp.Find("SerialNumber,overallResult") != -1)
			{
				CommaNumber = FindReadingLength(tmp,arrfilename[j],Information);
				/*TSP 데이터 케리어 아이디때문에 만든 구문*/
				if (tmp.Find("CARRIER_ID") != -1)
				{
					CString Carrier1 = tmp.Left(tmp.Find("CARRIER_ID"));
					AdditionalCommaNumber = Carrier1.Remove(',');
				}
				/*****************************************/
			}

			int count = 0;
			int point = 0;
			while (CommaNumber > count)
			{
				point = tmp.Find(',',point+1);
				count++;
			}
			/*TSP 데이터 케리어 아이디때문에 만든 구문*/
			CString Carrier2 = "";
			if (AdditionalCommaNumber > 0)
			{
				int count2 = 0;
				int point2 = 0;
				while (AdditionalCommaNumber > count2)
				{
					point2 = tmp.Find(',', point2 + 1);
					count2++;
				}
				Carrier2 = tmp.Mid(point2);
				Carrier2 = Carrier2.Left(Carrier2.Find(",", 1));
			}
			/*****************************************/
			tmp = tmp.Left(point);
			tmp += Carrier2;
			str += tmp + "\n";
		}
		FileRead2.Close();

		arrfilename[j].Replace("Auto_Merge", "Overall");
		CString OverallPath = "";
		OverallPath = makefolder(WriteFilePath, arrfilename[j], "Overall");//내부 폴더 만들기
		OverallPath += "\\" + arrfilename[j];
		FileWrite2.Open(OverallPath, CFile::modeCreate | CFile::modeWrite );
		FileWrite2.WriteString(str);
		FileWrite2.Close();

		//파일분할 있을 시
		m_division.GetWindowTextA(strdivision);
		if (strdivision != "")
		{
			double division = 0;
			division = atof(strdivision);
			FileDivision(OverallPath, division);
		}

		str = "";
	}

	AfxGetApp()->DoWaitCursor(-1);
	AfxMessageBox("Completed!");
}

//파일 갯수 나타내는 함수
void Tab2::ShowFileNum()
{
	CString filenum;
	filenum.Format("%d", m_filelist.GetCount());
	m_filenum.SetWindowTextA(filenum);
}

//폴더 내 파일들 불러오기
void Tab2::GetFileList(CString strFolder) 
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

//폴더만들기
CString Tab2::makefolder(CString strloadpath, CString arrfilename, CString type)
{
	CString strFolderName = "\\" + type + "_";
	CString writefilepath = strloadpath;
	CString testername = "";
	if (arrfilename.Find("ET1") != -1)
		testername = "ET1";
	else if (arrfilename.Find("ET2") != -1)
		testername = "ET2";
	else if (arrfilename.Find("TSP") != -1)
		testername = "TSP";
	else if (arrfilename.Find("T_Sensor") != -1)
		testername = "TSENSOR";
	else if (arrfilename.Find("T_Flex") != -1)
		testername = "TFLEX";
	else if (arrfilename.Find("SFT") != -1)
		testername = "SFT";
	else if (arrfilename.Find("ETY") != -1)
		testername = "ETY";

	strFolderName += testername;
	if (GetFileAttributes(strloadpath + strFolderName) == 0xFFFFFFFF) //테스터기별 AutoMerge/Overall 폴더 만들기
		CreateDirectory(strloadpath + strFolderName, NULL);

	strFolderName += "\\" + arrfilename.Mid(arrfilename.Find(testername,10)); //Sw버전별 AutoMerge/Overall 폴더 만들기
	strFolderName.Replace(".csv","");
	strFolderName.Replace(".Log", "");
	if (GetFileAttributes(strloadpath + strFolderName) == 0xFFFFFFFF) 
	{
		CreateDirectory(strloadpath + strFolderName, NULL);
		writefilepath = strloadpath + strFolderName;
	}
	else //폴더가 이미 있다면
	{
		writefilepath = strloadpath + strFolderName;
	}

	return writefilepath;
}
