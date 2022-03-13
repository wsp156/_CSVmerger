// Tab3.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CSVmerger.h"
#include "CSVmergerDlg.h"
#include "Tab3.h"
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


// Tab3 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Tab3, CDialogEx)

Tab3::Tab3(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

Tab3::~Tab3()
{
}

void Tab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_filenum);
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Control(pDX, IDC_BUTTON1, m_Denasca);
	DDX_Control(pDX, IDC_BUTTON2, m_deleteNA);
	DDX_Control(pDX, IDC_BUTTON3, m_changedouble);
	DDX_Control(pDX, IDC_BUTTON6, m_addfile);
	DDX_Control(pDX, IDC_BUTTON7, m_addfolder);
	DDX_Control(pDX, IDC_BUTTON4, m_deletefile);
	DDX_Control(pDX, IDC_BUTTON8, m_deleteall);
	DDX_Control(pDX, IDC_BUTTON9, m_DeletingSpec);
	DDX_Control(pDX, IDC_EDIT3, m_configname);
	DDX_Control(pDX, IDC_EDIT4, m_savepath);
	DDX_Control(pDX, IDC_CHECK2, m_check_cover);
}


BEGIN_MESSAGE_MAP(Tab3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &Tab3::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &Tab3::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON4, &Tab3::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON8, &Tab3::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON2, &Tab3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Tab3::OnBnClickedButton3)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON9, &Tab3::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &Tab3::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &Tab3::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &Tab3::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &Tab3::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_CHECK2, &Tab3::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON14, &Tab3::OnBnClickedButton14)
END_MESSAGE_MAP()


// Tab3 �޽��� ó�����Դϴ�.

//���� �߰� ��ư
void Tab3::OnBnClickedButton6()
{
	CString PathRead;
	CStdioFile FileRead;
	CString filename;
	CString strfilelist;
	const int MaxFiles = 400;
	const int BuffSize = (MaxFiles * (MAX_PATH + 1)) + 1;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, "CSV Files(*.csv)|*.csv|All Files(*.*)|*.*|");

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

//���� �߰� ��ư
void Tab3::OnBnClickedButton7()
{
	CString strInitPath = ".\\";
	CString strFolderPath;

	// ���� ���� ���̾�α�
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

//�������� ���� ��ư
void Tab3::OnBnClickedButton4()
{
	int count = m_filelist.GetCount();
	for (int i = count - 1; i >= 0; i--)
	{
		if (m_filelist.GetSel(i))
			m_filelist.DeleteString(i);
	}
	ShowFileNum();
}

//��ü���� ���� ��ư
void Tab3::OnBnClickedButton8()
{
	m_filelist.ResetContent();
	ShowFileNum();
}

//���ϸ���Ʈ ���� �����ִ� �Լ�
void Tab3::ShowFileNum()
{
	CString filenum;
	filenum.Format("%d", m_filelist.GetCount());
	m_filenum.SetWindowTextA(filenum);
}

//���ν�ũ�� ����� �Լ�
void Tab3::CreateHorizontalScroll()
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

//���� �� ���ϵ� �ҷ�����
void Tab3::GetFileList(CString strFolder) 
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

//���� Ȥ�� ���� �巡�׵�� ���
void Tab3::OnDropFiles(HDROP hDropInfo)
{
	UINT i = 0;    //Drag&Drop�� ����,������ ��������
				   //��ӵ� ������ ����
	UINT uCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	char buffer[256];  //Drag&Drop�� ����,������ ������ ����

	for (i = 0; i < uCount; i++)
	{
		// ������ ��� ����
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

//Denasca ��ư
void Tab3::OnBnClickedButton1()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CStdioFile FileRead;
	CString ReadFilePath = "";
	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();

	if(coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_DeNASCA");
	}

	int index = 0;
	int filenum = 0;

	filenum = m_filelist.GetCount();

	while (index != filenum) {
		CString tmp;
		CString WriteFilepath = "";
		CString FileName = "";

		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		FileRead.ReadString(tmp);
		FileRead.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index++;

		if (tmp.Find("NASCA") != -1)
			tmp = NASCAfile(ReadFilePath);
		else
			continue;
		
		CStdioFile FileWrite;
		if (coverCheck == FALSE)
		{
			FileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + FileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}
		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
		FileWrite.WriteString(tmp);
		FileWrite.Close();
	}
	pMainDlg->m_progressbar.SetPos(100);
	AfxGetApp()->DoWaitCursor(-1);
	AfxMessageBox("Completed!");
}

//NA ���� ä��� ��ư
void Tab3::OnBnClickedButton2()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);
	CStdioFile FileRead;
	CStdioFile FileWrite;
	CString ReadFilePath = "";
	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_NAtoBlank");
	}
	int index = 0;
	int filenum = 0;

	filenum = m_filelist.GetCount();

	while (index != filenum)
	{
		CString tmp = "";
		CString str = "";
		CString WriteFilepath = "";
		CString FileName = "";

		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		while (FileRead.ReadString(tmp))
		{
			if (tmp.Find("NASCA") != -1)
			{
				CString Nascafilenum = "";
				Nascafilenum.Format("%d", index + 1);
				NASCAMESSAGE;
				return;
			}
			tmp.Replace(",NA", ",");
			str += tmp + "\n";
			ProcessWindowMessage();
		}
		FileRead.Close();
		if (coverCheck == FALSE)
		{
			FileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + FileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}
		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(str);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index += 1;
	}
	AfxGetApp()->DoWaitCursor(-1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!");
}

//��� �� �Ǽ� ó�� ��ư
void Tab3::OnBnClickedButton3()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CString ReadFilePath = "";
	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_ChangeDouble");
	}
	int index = 0;
	int filenum = 0;

	filenum = m_filelist.GetCount();

	while (index != filenum) {
		CStdioFile FileWrite;
		CString WriteFilepath = "";
		CString FileName = "";
		CString text = "";

		m_filelist.GetText(index, ReadFilePath);
		text = ChangeDouble(ReadFilePath);
		if (coverCheck == FALSE)
		{
			FileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + FileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}
		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(text);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index++;
	}
	AfxGetApp()->DoWaitCursor(-1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!");
}

//NASCA ����� �Լ�
CString Tab3::NASCAfile(CString strLoadPath)
{
	CString savepath = "";
	m_savepath.GetWindowTextA(savepath);
	CString FileName = "";
	FileName = strLoadPath.Mid(strLoadPath.ReverseFind('\\'));
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication oApp;
	CWorkbooks oBooks;
	CWorkbook oBook;
	CWorksheets oSheets;
	CWorksheet oSheet;
	CRange oRange;

	oApp.CreateDispatch("Excel.Application");
	oBooks = oApp.get_Workbooks();
	oBook = oBooks.Open(strLoadPath,
		covOptional, covFalse, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional);
	oSheets = oBook.get_Worksheets();
	oSheet = oSheets.get_Item(COleVariant((short)1));
	//������ �ֱ�
	long iRows;
	long iCols;
	long lowRow;
	long lowCol;
	long index[2];

	int r, c;
	CString value = "";
	CString text = "";

	oRange = oSheet.get_UsedRange();

	COleSafeArray saRet(oRange.get_Value(covOptional));
	saRet.GetUBound(1, &iRows);
	saRet.GetUBound(2, &iCols);
	saRet.GetLBound(1, &lowRow);
	saRet.GetLBound(2, &lowCol);
	VARIANT val;

	for (r = lowRow; r <= iRows; r++)
	{
		for (c = lowCol; c <= iCols; c++)
		{
			index[0] = r;
			index[1] = c;
			saRet.GetElement(index, &val);
			/*
			if (val.vt == VT_R8) {
				value.Format("%f", val.dblVal);
			}
			else if (val.vt == VT_R4) {
				value.Format("%f", val.fltVal);
			}
			else if (val.vt == VT_I2) {
				value.Format("%d", val.dblVal);
			}
			else if (val.vt == VT_I4) {
				value.Format("%d", val.dblVal);

			}
			else if (val.vt == VT_DATE) {
				COleDateTime t1 = val.date;
				value = t1.Format("%Y-%m-%d %H:%M");
			}
			else if (val.vt == VT_LPSTR) value = val.bstrVal;
			else if (val.vt == VT_BSTR) value = val.bstrVal;
			else if (val.vt == VT_EMPTY) value = "";
			else value = "";
			*/
			value = val;
			text += value + ",";
		}
		text += "\n";
	}
	oApp.Quit();
	return text;
}

//��� �� �Ǽ��� �ٲٴ� �Լ�
CString Tab3::ChangeDouble(CString strLoadPath)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication oApp;
	CWorkbooks oBooks;
	CWorkbook oBook;
	CWorksheets oSheets;
	CWorksheet oSheet;
	CRange oRange;
	CRange range;

	oApp.CreateDispatch("Excel.Application");
	oBooks = oApp.get_Workbooks();
	oBook = oBooks.Open(strLoadPath,
		covOptional, covFalse, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional);
	oSheets = oBook.get_Worksheets();
	oSheet = oSheets.get_Item(COleVariant((short)1));
	//������ �ֱ�
	long iRows;
	long iCols;
	long lowRow;
	long lowCol;
	long index[2];
	VARIANT val;
	int r, c;

	oRange = oSheet.get_UsedRange();
	COleSafeArray saRet(oRange.get_Value(covOptional));

	saRet.GetUBound(1, &iRows);
	saRet.GetUBound(2, &iCols);
	saRet.GetLBound(1, &lowRow);
	saRet.GetLBound(2, &lowCol);

	CString value = "";
	CString text = "";

	for (r = lowRow; r <= iRows; r++)
	{
		for (c = lowCol; c <= iCols; c++)
		{
			index[0] = r;
			index[1] = c;
			saRet.GetElement(index, &val);
			if (val.vt == VT_R8) {
				value.Format("%f", val.dblVal);
			}
			else if (val.vt == VT_R4) {
				value.Format("%f", val.fltVal);
			}
			else if (val.vt == VT_I2) {
				value.Format("%d", val.dblVal);
			}
			else if (val.vt == VT_I4) {
				value.Format("%d", val.dblVal);

			}
			else if (val.vt == VT_DATE) {
				COleDateTime t1 = val.date;
				value = t1.Format("%Y-%m-%d %H:%M");
			}
			else if (val.vt == VT_LPSTR) value = val.bstrVal;
			else if (val.vt == VT_BSTR) value = val.bstrVal;
			else if (val.vt == VT_EMPTY) value = "";
			text += value + ",";
		}
		text += "\n";
	}
	oApp.Quit();
	return text;	
}

//�������Ź�ư
void Tab3::OnBnClickedButton9()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CString ReadFilePath = "";
	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_DeleteSpec");
	}

	///////////////////////////////////////////////////////
	////////////Information Setting ��������///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation	Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	int index = 0;
	int filenum = 0;
	filenum = m_filelist.GetCount();

	while (index != filenum)
	{
		CStdioFile FileRead;
		CString WriteFilepath = "";
		CString ReadFileName = "";
		CString firstrow = "";
		int informationcount = 0;
		//ù�� �б�
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		ReadFileName = FileRead.GetFileName();
		FileRead.ReadString(firstrow);
		FileRead.Close();

		//Nasca�� ������ �ִٸ� ���� ����
		if (firstrow.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			NASCAMESSAGE;
			return;
		}

		//FileName�� ���� Version �з�
		informationcount = InformationCount(ReadFileName, Information);

		//���� �б�
		CString tmp = "";
		CString str = "";
		bool First = true;
		FileRead.Open(ReadFilePath, CFile::modeRead);
		while (FileRead.ReadString(tmp))
		{
			if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
			{
				First = false;
				continue;
			}
			if (tmp.Find(Information[informationcount].USLRowKey) != -1)
				continue;
			else if (tmp.Find(Information[informationcount].LSLRowKey) != -1)
				continue;
			str += tmp + "\n";
			First = false;
			ProcessWindowMessage();
		}
		FileRead.Close();

		if (coverCheck == FALSE)
		{
			ReadFileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + ReadFileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}

		CStdioFile FileWrite;

		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(str);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index += 1;
	}
	AfxGetApp()->DoWaitCursor(1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!.");
}

//Config ���� ��ư
void Tab3::OnBnClickedButton10()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_DeleteConfig");
	}

	///////////////////////////////////////////////////////
	////////////Information Setting ��������///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation	Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	//������ Config Ȯ��
	CString config = "";
	CString configarr[1000] = {};
	int configindex = 0;
	m_configname.GetWindowTextA(config);
	
	if (config.Find(",") == -1)
		configarr[configindex] = config;
	else
		configarr[configindex] = config.Left(config.Find(","));
	configindex = 1;
	while (config.Find(",") != -1)
	{
		config = config.Mid(config.Find(",")+1);
		if (config.Find(",") == -1)
			configarr[configindex] = config;
		else
			configarr[configindex] = config.Left(config.Find(","));
		configindex++;
	} 

	int index = 0;
	int filenum = 0;
	filenum = m_filelist.GetCount();
	while (index != filenum)
	{
		CStdioFile FileRead;
		CString ReadFilePath = "";
		CString WriteFilepath = "";
		CString ReadFileName = "";
		CString firstrow = "";
		int informationcount = 0;
		int commanumber = 0;

		//ù�� �б�
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		ReadFileName = FileRead.GetFileName();
		FileRead.ReadString(firstrow);
		FileRead.Close();

		//Nasca�� ������ �ִٸ� ���� ����
		if (firstrow.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			NASCAMESSAGE;
			return;
		}

		//FileName�� ���� Version �з�
		informationcount = InformationCount(ReadFileName, Information);

		CString tmp = "";
		CString tmp2 = "";
		CString str = "";
		bool First = true;
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		while (FileRead.ReadString(tmp))
		{
			if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
				tmp = tmp;
			else if (tmp.Find(Information[informationcount].USLRowKey) != -1 || tmp.Find(Information[informationcount].LSLRowKey) != -1)
				tmp = tmp;
			else if (tmp.Find(Information[informationcount].ColumnRowKey) != -1)
			{
				tmp2 = tmp.Left(tmp.Find(Information[informationcount].FindKey));
				commanumber = tmp2.Remove(',');
			}
			else
			{
				BOOL foundit = FALSE;
				tmp2 = tmp;
				for (int comma = 0; comma < commanumber; comma++)
				{
					tmp2 = tmp2.Mid(tmp2.Find(",") + 1);
				}
				tmp2 = tmp2.Left(tmp2.Find(","));
				for (int arr = 0; arr < configindex; arr++)
				{
					if (configarr[arr] == tmp2)
						foundit = TRUE;
				}
				if (foundit == TRUE)
					continue;
			}
			str += tmp + "\n";
			First = false;
			ProcessWindowMessage();
		}
		FileRead.Close();
		if (coverCheck == FALSE)
		{
			ReadFileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + ReadFileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}

		CStdioFile FileWrite;

		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(str);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index += 1;
	}
	AfxGetApp()->DoWaitCursor(1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!");
}

//Add "STRING" in 1st row ��ư
void Tab3::OnBnClickedButton11()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CString ReadFilePath = "";
	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_AddStringIn1stRow");
	}

	///////////////////////////////////////////////////////
	////////////Information Setting ��������///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation	Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);


	int index = 0;
	int filenum = 0;
	filenum = m_filelist.GetCount();

	while (index != filenum)
	{
		CStdioFile FileRead;
		CString WriteFilepath = "";
		CString ReadFileName = "";
		CString firstrow = "";
		int informationcount = 0;
		//ù�� �б�
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		ReadFileName = FileRead.GetFileName();
		FileRead.ReadString(firstrow);
		FileRead.Close();

		//Nasca�� ������ �ִٸ� ���� ����
		if (firstrow.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			NASCAMESSAGE;
			return;
		}

		//FileName�� ���� Version �з�
		informationcount = InformationCount(ReadFileName, Information);

		int commanumber = 0;
		CString tmp = "";
		CString tmp2 = "";
		CString str = "";
		bool First = true;
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		while (FileRead.ReadString(tmp))
		{
			if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
			{
				First = false;
				continue;
			}
			else if (tmp.Find(Information[informationcount].ColumnRowKey) != -1)
			{
				tmp2 = tmp;
				commanumber = tmp2.Remove(',');
				tmp2 = "STRING";
				for (int number = 0; number < commanumber; number++)
				{
					tmp2 += ",STRING";
				}
			}
			str += tmp + "\n";
			First = false;
			ProcessWindowMessage();
		}
		FileRead.Close();
		str = tmp2 + "\n" + str;
		if (coverCheck == FALSE)
		{
			ReadFileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + ReadFileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}

		CStdioFile FileWrite;

		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(str);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index += 1;
	}
	AfxGetApp()->DoWaitCursor(1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!");
}

void Tab3::CreatingResultDirectory(CString *SavePath, CString Type)
{
	CString ReadFilePath = *SavePath;
	time_t timer = time(NULL);
	tm calTime;
	localtime_s(&calTime, &timer);
	CString time_current = "";
	time_current.Format("%02d%02d%02d%02d%02d%02d", calTime.tm_year - 100, calTime.tm_mon + 1, calTime.tm_mday, calTime.tm_hour, calTime.tm_min, calTime.tm_sec);
	ReadFilePath += '\\' + time_current + "_" + Type;
	CreateDirectory(ReadFilePath, NULL);
	*SavePath = ReadFilePath;
}

//Config ã�� ��ư
void Tab3::OnBnClickedButton12()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_FindConfig");
	}

	///////////////////////////////////////////////////////
	////////////Information Setting ��������///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation	Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	//������ Config Ȯ��
	CString config = "";
	CString configarr[1000] = {};
	int configindex = 0;
	m_configname.GetWindowTextA(config);

	if (config.Find(",") == -1)
		configarr[configindex] = config;
	else
		configarr[configindex] = config.Left(config.Find(","));
	configindex = 1;
	while (config.Find(",") != -1)
	{
		config = config.Mid(config.Find(",") + 1);
		if (config.Find(",") == -1)
			configarr[configindex] = config;
		else
			configarr[configindex] = config.Left(config.Find(","));
		configindex++;
	}

	int index = 0;
	int filenum = 0;
	filenum = m_filelist.GetCount();

	while (index != filenum)
	{
		CStdioFile FileRead;
		CString ReadFilePath = "";
		CString WriteFilepath = "";
		CString ReadFileName = "";
		CString firstrow = "";
		int informationcount = 0;
		int commanumber = 0;

		//ù�� �б�
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		ReadFileName = FileRead.GetFileName();
		FileRead.ReadString(firstrow);
		FileRead.Close();

		//Nasca�� ������ �ִٸ� ���� ����
		if (firstrow.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			NASCAMESSAGE;
			return;
		}

		//FileName�� ���� Version �з�
		informationcount = InformationCount(ReadFileName, Information);


		CString tmp = "";
		CString tmp2 = "";
		CString str = "";
		bool First = true;
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		while (FileRead.ReadString(tmp))
		{
			if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
				tmp = tmp;
			else if(tmp.Find(Information[informationcount].USLRowKey) != -1 || tmp.Find(Information[informationcount].LSLRowKey) != -1)
				tmp = tmp;
			else if (tmp.Find(Information[informationcount].ColumnRowKey) != -1)
			{
				tmp2 = tmp.Left(tmp.Find(Information[informationcount].FindKey));
				commanumber = tmp2.Remove(',');
			}
			else
			{
				BOOL foundit = FALSE;
				tmp2 = tmp;
				for (int comma = 0; comma < commanumber; comma++)
				{
					tmp2 = tmp2.Mid(tmp2.Find(",") + 1);
				}
				tmp2 = tmp2.Left(tmp2.Find(","));
				for (int arr = 0; arr < configindex; arr++)
				{
					if (configarr[arr] == tmp2)
						foundit = TRUE;
				}
				if (foundit != TRUE)
					continue;
			}
			str += tmp + "\n";
			First = false;
			ProcessWindowMessage();
		}
		FileRead.Close();

		if (coverCheck == FALSE)
		{
			ReadFileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + ReadFileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}

		CStdioFile FileWrite;
		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(str);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index += 1;
	}
	AfxGetApp()->DoWaitCursor(1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!");
}

//������ ���� ���� ��ư
void Tab3::OnBnClickedButton13()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strInitPath = ".\\";

	// ���� ���� ���̾�α�
	CFolderPickerDialog Picker(strInitPath, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString strPath = Picker.GetPathName();
		m_savepath.SetWindowTextA(strPath);
	}
}

void Tab3::SetSavePath(CString strsavePath)
{
	DWORD ret = GetFileAttributes(strsavePath);
	if (ret != INVALID_FILE_ATTRIBUTES)
	{
		strsavePath = strsavePath.Left(strsavePath.ReverseFind('\\'));
		m_savepath.SetWindowTextA(strsavePath);
	}
}

void Tab3::OnBnClickedCheck2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bCheck = m_check_cover.GetCheck();

	if (bCheck == TRUE)
		GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
}

//�ߺ� ���� ��ư
void Tab3::OnBnClickedButton14()
{
	AfxGetApp()->DoWaitCursor(1);
	CCSVmergerDlg* pMainDlg = (CCSVmergerDlg*)::AfxGetMainWnd();
	pMainDlg->m_progressbar.SetPos(0);

	CString WriteFileFolder = "";
	BOOL coverCheck = m_check_cover.GetCheck();
	if (coverCheck == FALSE)
	{
		m_savepath.GetWindowTextA(WriteFileFolder);
		CreatingResultDirectory(&WriteFileFolder, "Result_Deduplication");
	}

	///////////////////////////////////////////////////////
	////////////Information Setting ��������///////////////
	///////////////////////////////////////////////////////
	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation	Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	int index = 0;
	int filenum = 0;
	filenum = m_filelist.GetCount();
	while (index != filenum)
	{
		CStdioFile FileRead;
		CString ReadFilePath = "";
		CString WriteFilepath = "";
		CString ReadFileName = "";
		CString firstrow = "";
		int informationcount = 0;
		int commanumber = 0;

		//ù�� �б�
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		ReadFileName = FileRead.GetFileName();
		FileRead.ReadString(firstrow);
		FileRead.Close();

		//Nasca�� ������ �ִٸ� ���� ����
		if (firstrow.Find("NASCA") != -1)
		{
			CString Nascafilenum = "";
			Nascafilenum.Format("%d", index + 1);
			NASCAMESSAGE;
			return;
		}

		//FileName�� ���� Version �з�
		informationcount = InformationCount(ReadFileName, Information);

		CString tmp = "";
		CString tmp2 = "";
		CString tmp3 = "";
		CString str = "";
		CString duplicatearray[100000] = { "" };
		int duplicateindex = 0;
		bool First = true;
		m_filelist.GetText(index, ReadFilePath);
		FileRead.Open(ReadFilePath, CFile::modeRead);
		while (FileRead.ReadString(tmp))
		{
			if (tmp.Find(Information[informationcount].ColumnRowKey) == -1 && First == true)
				tmp = tmp;
			else if (tmp.Find(Information[informationcount].USLRowKey) != -1 || tmp.Find(Information[informationcount].LSLRowKey) != -1)
				tmp = tmp;
			else if (tmp.Find(Information[informationcount].ColumnRowKey) != -1)
			{
				if (tmp.Find(Information[informationcount].OverlapKey) == -1 || Information[informationcount].OverlapKey == "NA")
					tmp2 = tmp2;
				else
					tmp2 = tmp.Left(tmp.Find(Information[informationcount].OverlapKey));
				commanumber = tmp2.Remove(',');
			}
			else
			{
				tmp3 = "";
				tmp2 = tmp;
				bool duplicate = false;
				if (commanumber == 0)
				{
					tmp3 = tmp2;
				}
				//�ߺ��̸� ���ʲ��, �ƴϸ� ����
				else
				{
					for (int comma = 0; comma < commanumber; comma++)
					{
						tmp3 += tmp2.Left(tmp2.Find(","));
						tmp2 = tmp2.Mid(tmp2.Find(",") + 1);
					}
					tmp3 += tmp2.Left(tmp2.Find(","));
				}
				for (int i = 0; i < duplicateindex; i++)
				{
					if (duplicatearray[i] == tmp3)
						duplicate = true;
				}
				if (duplicate == true)
				{
					ProcessWindowMessage();
					continue;
				}
				else
				{
					duplicatearray[duplicateindex] = tmp3;
					duplicateindex++;
				}
			}
			str += tmp + "\n";
			First = false;
			ProcessWindowMessage();
		}
		FileRead.Close();
		if (coverCheck == FALSE)
		{
			ReadFileName = ReadFilePath.Mid(ReadFilePath.ReverseFind('\\'));
			WriteFilepath = WriteFileFolder + '\\' + ReadFileName;
		}
		else
		{
			DeleteFile(ReadFilePath);
			WriteFilepath = ReadFilePath;
		}

		CStdioFile FileWrite;

		FileWrite.Open(WriteFilepath, CFile::modeCreate | CFile::modeWrite);
		FileWrite.WriteString(str);
		FileWrite.Close();
		pMainDlg->m_progressbar.SetPos(100 / (float)filenum * (float)index);
		ProcessWindowMessage();
		index += 1;
	}
	AfxGetApp()->DoWaitCursor(1);
	pMainDlg->m_progressbar.SetPos(100);
	AfxMessageBox("Completed!");
}
