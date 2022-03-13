// Tab2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CSVmerger.h"
#include "CSVmergerDlg.h"
#include "Tab2.h"
#include "afxdialogex.h"
#include "BringInformation.h"


// Tab2 ��ȭ �����Դϴ�.

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


// Tab2 �޽��� ó�����Դϴ�.



//����������ι�ư
void Tab2::OnBnClickedButton5()
{
	m_filelist.ResetContent();
	CString strInitPath = ".\\";
	// ���� ���� ���̾�α�
	CFolderPickerDialog Picker(strInitPath, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString strPath = Picker.GetPathName();
		m_savepath.SetWindowTextA(strPath);
		GetFileList(strPath);
	}
	ShowFileNum();
}

//��� ����
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

		//Nasca�� ������ �ִ� ������ �ִٸ� �ߴ�
		if (tmp.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			AfxMessageBox(Nascafilenum + "��° ������ NASCA�� �ɷȽ��ϴ�. Nasca�� ������ �� ������ �ּ���");
			return;
		}

		//FileName�� ���� Merge�� ���� �з�
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
		AutoMergePath = makefolder(WriteFilePath, arrfilename[j],"Auto_Merge");//���� ���� �����
		AutoMergePath += "\\" + arrfilename[j];
		FileWrite.Open(AutoMergePath, CFile::modeCreate | CFile::modeWrite );
		FileWrite.WriteString(str);
		FileWrite.Close();

		//���Ϻ��� ���� ��
		CString strdivision = "";
		m_division.GetWindowTextA(strdivision);
		if (strdivision != "")
		{
			double division = 0;
			division = atof(strdivision);
			FileDivision(AutoMergePath, division);
		}

		str = "";


		//Overall �����
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
				/*TSP ������ �ɸ��� ���̵𶧹��� ���� ����*/
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
			/*TSP ������ �ɸ��� ���̵𶧹��� ���� ����*/
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
		OverallPath = makefolder(WriteFilePath, arrfilename[j], "Overall");//���� ���� �����
		OverallPath += "\\" + arrfilename[j];
		FileWrite2.Open(OverallPath, CFile::modeCreate | CFile::modeWrite );
		FileWrite2.WriteString(str);
		FileWrite2.Close();

		//���Ϻ��� ���� ��
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

//���� ���� ��Ÿ���� �Լ�
void Tab2::ShowFileNum()
{
	CString filenum;
	filenum.Format("%d", m_filelist.GetCount());
	m_filenum.SetWindowTextA(filenum);
}

//���� �� ���ϵ� �ҷ�����
void Tab2::GetFileList(CString strFolder) 
{
	CFileFind file;
	BOOL b = file.FindFile(strFolder + "\\*.*");  // CString strMusicFilter = ".MP3.OGG.WMA.WAV";   // ���͸� �ϰ� ������ �̷��� ���� ��
	CString strFolderItem, strFileExt, strTempString;
	///////////////////////////////////////////////////////
	////////////Information Setting ��������///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);
	//////////////////////////////////////////////////
	while (b)
	{
		b = file.FindNextFile();
		if (file.IsDirectory() && !file.IsDots())   // ���丮 �߽߰� 
		{
			strFolderItem = file.GetFilePath();
			GetFileList(strFolderItem);      // ���������� �˻��ϱ� ���� ���ȣ�� �߻�  
		}
		strFolderItem = file.GetFilePath();

		strFileExt = strFolderItem.Mid(strFolderItem.ReverseFind('.'));  // Ȯ���ڸ� �����Ѵ�. 
		if (!file.IsDots())         // ���� Ž�� ���� ���ǿ����� �ش� StringList�� �߰�
		{
			strFileExt.MakeUpper();       // strFileExt ���� Ȯ���� (.EXE ����) �� ����. ������ �빮��ȭ ��
			if (file.IsDirectory())
				continue;    // ������ ���� ���� ������ �ȵ� 
			for (int i = 0; i < Setting.ExtensionNum; i++)
			{
				CString extension = "." + Setting.FileExtension[i];
				if (strFolderItem.Find(extension) != -1)
					m_filelist.AddString(strFolderItem);
			}
		}
	}
}

//���������
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
	if (GetFileAttributes(strloadpath + strFolderName) == 0xFFFFFFFF) //�׽��ͱ⺰ AutoMerge/Overall ���� �����
		CreateDirectory(strloadpath + strFolderName, NULL);

	strFolderName += "\\" + arrfilename.Mid(arrfilename.Find(testername,10)); //Sw������ AutoMerge/Overall ���� �����
	strFolderName.Replace(".csv","");
	strFolderName.Replace(".Log", "");
	if (GetFileAttributes(strloadpath + strFolderName) == 0xFFFFFFFF) 
	{
		CreateDirectory(strloadpath + strFolderName, NULL);
		writefilepath = strloadpath + strFolderName;
	}
	else //������ �̹� �ִٸ�
	{
		writefilepath = strloadpath + strFolderName;
	}

	return writefilepath;
}
