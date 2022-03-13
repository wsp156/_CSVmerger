// SettingDialog.cpp: 구현 파일
//
#include "stdafx.h"
#include "CSVmerger.h"
#include "CSVmergerDlg.h"
#include "SettingDialog.h"
#include "afxdialogex.h"
#include "BringInformation.h"

// SettingDialog 대화 상자

IMPLEMENT_DYNAMIC(SettingDialog, CDialogEx)

SettingDialog::SettingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

SettingDialog::~SettingDialog()
{
}

void SettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_PropGrid1);
}


BEGIN_MESSAGE_MAP(SettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SettingDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL SettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitialSetting();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// SettingDialog 메시지 처리기
void SettingDialog::InitialSetting(void)
{
	//////////////////////////////////////////////////////////////////////////
	/////////////////////INI 파일 가져오기////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	TesterInformation	Information[MAXINFORMATIONNUM];
	SettingInformation Setting;
	BringINIfile(Information, MAXINFORMATIONNUM, &Setting);

	//////////////////////////////////////////////////////////////////////////
	/////////////////////Property 창으로 가져오기/////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CMFCPropertyGridProperty* pGroup[MAXINFORMATIONNUM];
	CMFCPropertyGridProperty* pSubGrid1[MAXINFORMATIONNUM];
	CMFCPropertyGridProperty* pSubGrid2[MAXINFORMATIONNUM];
	CMFCPropertyGridProperty* pSubGrid3[MAXINFORMATIONNUM];
	CMFCPropertyGridProperty* pSubGrid4[MAXINFORMATIONNUM];
	for (int i = 0; i < MAXINFORMATIONNUM; i++)
	{
		CString Group = "";
		Group.Format("Information%d", i);
		CString TesterLocation;
		CString Version1Location;
		CString Version2Location;
		CString Version3Location;
		CString ModelLocation;
		pGroup[i]= new CMFCPropertyGridProperty(Group);
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("kinds", Information[i].kinds, "The kind of tester of this Information Group"));
		//
		if (Information[i].TesterLocation == CHECKFIRSTROW)
			TesterLocation = "firstrow";
		else
			TesterLocation = "filename";
		pSubGrid1[i] = new CMFCPropertyGridProperty("TesterLocation", TesterLocation, "Select location of the Tester key-word");
		pSubGrid1[i]->AddOption("filename");
		pSubGrid1[i]->AddOption("firstrow");
		pSubGrid1[i]->AllowEdit(FALSE);
		pGroup[i]->AddSubItem(pSubGrid1[i]);
		//
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("TesterKey", Information[i].TesterKey, "The Key-word that help you recognizing what kind of the tester is"));
		//
		if (Information[i].Version1Location == CHECKFIRSTROW)
			Version1Location = "firstrow";
		else
			Version1Location = "filename";
		pSubGrid1[i] = new CMFCPropertyGridProperty("Version1Location", Version1Location, "Select location of the Version1 key-word");
		pSubGrid1[i]->AddOption("filename");
		pSubGrid1[i]->AddOption("firstrow");
		pSubGrid1[i]->AllowEdit(FALSE);
		pGroup[i]->AddSubItem(pSubGrid1[i]);
		//
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("Version1Key", Information[i].Version1Key, "The Key-word that help you recognizing what kind of the version1 is"));
		//
		if (Information[i].Version2Location == CHECKFIRSTROW)
			Version2Location = "firstrow";
		else
			Version2Location = "filename";
		pSubGrid2[i] = new CMFCPropertyGridProperty("Version2Location", Version2Location, "Select location of the Version2 key-word");
		pSubGrid2[i]->AddOption("filename");
		pSubGrid2[i]->AddOption("firstrow");
		pSubGrid2[i]->AllowEdit(FALSE);
		pGroup[i]->AddSubItem(pSubGrid2[i]);
		//
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("Version2Key", Information[i].Version2Key, "The Key-word that help you recognizing what kind of the version2 is"));
		//
		if (Information[i].Version3Location == CHECKFIRSTROW)
			Version3Location = "firstrow";
		else
			Version3Location = "filename";
		pSubGrid3[i] = new CMFCPropertyGridProperty("Version3Location", Version3Location, "Select location of the Version3 key-word");
		pSubGrid3[i]->AddOption("filename");
		pSubGrid3[i]->AddOption("firstrow");
		pSubGrid3[i]->AllowEdit(FALSE);
		pGroup[i]->AddSubItem(pSubGrid3[i]);
		//
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("Version3Key", Information[i].Version3Key, "The Key-word that help you recognizing what kind of the version3 is"));
		//
		if (Information[i].ModelLocation == CHECKFIRSTROW)
			ModelLocation = "firstrow";
		else
			ModelLocation = "filename";
		pSubGrid4[i] = new CMFCPropertyGridProperty("ModelLocation", ModelLocation, "Select location of the Model key-word");
		pSubGrid4[i]->AddOption("filename");
		pSubGrid4[i]->AddOption("firstrow");
		pSubGrid4[i]->AllowEdit(FALSE);
		pGroup[i]->AddSubItem(pSubGrid4[i]);
		//
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("ModelKey", Information[i].ModelKey, "The Key-word that help you recognizing what kind of the model is"));
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("ColumnRowKey", Information[i].ColumnRowKey, "The Key-word for looking for Column name Row"));
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("USLRowKey", Information[i].USLRowKey, "The Key-word for looking for USL Row"));
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("LSLRowKey", Information[i].LSLRowKey, "The Key-word for looking for LSL Row"));
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("OverallKey", Information[i].OverallKey, "The Last Item that you want to get when you use \"Overall\" function"));
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("FindKey", Information[i].FindKey, "The Item you want to find using \"Find/Delete Config\" function"));
		pGroup[i]->AddSubItem(new CMFCPropertyGridProperty("OverlapKey", Information[i].OverlapKey, "The Item that is the standard for removing duplicate data"));
		m_PropGrid1.AddProperty(pGroup[i]);
	}
	CMFCPropertyGridProperty* pGroupcommon;
	CMFCPropertyGridProperty* pcommonSubGrid;
	CString extensions = Setting.FileExtension[0];
	for (int i = 1; i < Setting.ExtensionNum; i++)
		extensions += "," + Setting.FileExtension[i];

	pGroupcommon = new CMFCPropertyGridProperty("ReadingMode");
	pcommonSubGrid = new CMFCPropertyGridProperty("kinds", Setting.ReadingMode, "Selecting Reading Mode. \"Character\" mode takes long time to merging than \"STRING\" mode, but it can read \"NULL\" value in the texts.");
	pcommonSubGrid->AddOption("STRING");
	pcommonSubGrid->AddOption("CHARACTER");
	pcommonSubGrid->AddOption("FAST");
	pcommonSubGrid->AllowEdit(FALSE);
	pGroupcommon->AddSubItem(pcommonSubGrid);
	pGroupcommon->AddSubItem(new CMFCPropertyGridProperty("FileExtension", extensions, "The extension of files"));
	m_PropGrid1.AddProperty(pGroupcommon);

	HDITEM hdItem;
	hdItem.mask = HDI_WIDTH; // indicating cxy is width
	hdItem.cxy = 300; // whatever you want the property name column width to be
	m_PropGrid1.GetHeaderCtrl().SetItem(0, &hdItem);

}

//Apply Button
void SettingDialog::OnBnClickedButton1()
{
	CString ProgramPath = "";
	CString iniPath = "";
	CString Value;
	COleVariant vars;

	if (GetModuleFileName(nullptr, ProgramPath.GetBuffer(_MAX_PATH + 1), MAX_PATH) != FALSE)
	{
		ProgramPath.ReleaseBuffer();
		iniPath = ProgramPath.Left(ProgramPath.ReverseFind('\\') + 1) + INIFILENAME;
	}

	for (int i=0; i<MAXINFORMATIONNUM; i++)
	{
		CString Group = "";
		Group.Format("Information%d", i);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(0)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "kinds", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(1)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "TesterLocation", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(2)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "TesterKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(3)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "Version1Location", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(4)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "Version1Key", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(5)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "Version2Location", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(6)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "Version2Key", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(7)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "Version3Location", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(8)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "Version3Key", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(9)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "ModelLocation", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(10)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "ModelKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(11)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "ColumnRowKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(12)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "USLRowKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(13)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "LSLRowKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(14)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "OverallKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(15)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "FindKey", Value, iniPath);
		vars = m_PropGrid1.GetProperty(i)->GetSubItem(16)->GetValue();
		Value = vars.bstrVal;
		WritePrivateProfileStringA(Group, "OverlapKey", Value, iniPath);
	}
	vars = m_PropGrid1.GetProperty(MAXINFORMATIONNUM)->GetSubItem(0)->GetValue();
	Value = vars.bstrVal;
	WritePrivateProfileStringA("ReadingMode", "kinds", Value, iniPath);
	vars = m_PropGrid1.GetProperty(MAXINFORMATIONNUM)->GetSubItem(1)->GetValue();
	Value = vars.bstrVal;
	WritePrivateProfileStringA("ReadingMode", "FileExtension", Value, iniPath);

	OnOK();

}
