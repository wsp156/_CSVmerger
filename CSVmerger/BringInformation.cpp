#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "BringInformation.h"
#include <string.h>

void BringINIfile(TesterInformation *Information, int num, SettingInformation *Setting)
{
	//////////////////////////////////////////////////////////////////////////
	/////////////////////INI 파일 가져오기////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	CHAR inbuffer[20] = "";
	CString ProgramPath = "";
	CString iniPath = "";
	CString extensions = "";

	if (GetModuleFileName(nullptr, ProgramPath.GetBuffer(_MAX_PATH + 1), MAX_PATH) != FALSE)
	{
		ProgramPath.ReleaseBuffer();
		iniPath = ProgramPath.Left(ProgramPath.ReverseFind('\\') + 1) + INIFILENAME;
	}
	for (int i = 0; i < num; i++)
	{
		CString Group = "";
		Group.Format("Information%d", i);
		//
		GetPrivateProfileString(Group, "kinds", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].kinds = inbuffer;
		//
		GetPrivateProfileString(Group, "TesterLocation", "NA", inbuffer, sizeof(inbuffer), iniPath);
		if (strcmp(inbuffer, "firstrow") == 0)
			Information[i].TesterLocation = CHECKFIRSTROW;
		else
			Information[i].TesterLocation = CHECKFILENAME;
		//
		GetPrivateProfileString(Group, "TesterKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].TesterKey = inbuffer;
		//
		GetPrivateProfileString(Group, "Version1Location", "NA", inbuffer, sizeof(inbuffer), iniPath);
		if (strcmp(inbuffer, "firstrow") == 0)
			Information[i].Version1Location = CHECKFIRSTROW;
		else
			Information[i].Version1Location = CHECKFILENAME;
		//
		GetPrivateProfileString(Group, "Version1Key", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].Version1Key = inbuffer;
		//
		GetPrivateProfileString(Group, "Version2Location", "NA", inbuffer, sizeof(inbuffer), iniPath);
		if (strcmp(inbuffer, "firstrow") == 0)
			Information[i].Version2Location = CHECKFIRSTROW;
		else
			Information[i].Version2Location = CHECKFILENAME;
		//
		GetPrivateProfileString(Group, "Version2Key", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].Version2Key = inbuffer;
		//
		GetPrivateProfileString(Group, "Version3Location", "NA", inbuffer, sizeof(inbuffer), iniPath);
		if (strcmp(inbuffer, "firstrow") == 0)
			Information[i].Version3Location = CHECKFIRSTROW;
		else
			Information[i].Version3Location = CHECKFILENAME;
		//
		GetPrivateProfileString(Group, "Version3Key", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].Version3Key = inbuffer;
		//
		GetPrivateProfileString(Group, "ModelLocation", "NA", inbuffer, sizeof(inbuffer), iniPath);
		if (strcmp(inbuffer, "firstrow") == 0)
			Information[i].ModelLocation = CHECKFIRSTROW;
		else
			Information[i].ModelLocation = CHECKFILENAME;
		//
		GetPrivateProfileString(Group, "ModelKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].ModelKey = inbuffer;
		GetPrivateProfileString(Group, "ColumnRowKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].ColumnRowKey = inbuffer;
		GetPrivateProfileString(Group, "USLRowKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].USLRowKey = inbuffer;
		GetPrivateProfileString(Group, "LSLRowKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].LSLRowKey = inbuffer;
		GetPrivateProfileString(Group, "OverallKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].OverallKey = inbuffer;
		GetPrivateProfileString(Group, "FindKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].FindKey = inbuffer;
		GetPrivateProfileString(Group, "OverlapKey", "NA", inbuffer, sizeof(inbuffer), iniPath);
		Information[i].OverlapKey = inbuffer;
	}
	GetPrivateProfileString("ReadingMode", "kinds", "FAST", inbuffer, sizeof(inbuffer), iniPath);
	Setting->ReadingMode = inbuffer;
	GetPrivateProfileString("ReadingMode", "FileExtension", "csv", inbuffer, sizeof(inbuffer), iniPath);
	extensions = inbuffer;
	int extnum = 0;
	while (extnum<MAXINFORMATIONNUM)
	{
		CString tmp = "";
		if (extensions.Find(",") != -1)
		{
			tmp = extensions.Left(extensions.Find(","));
			Setting->FileExtension[extnum] = tmp;
			extensions = extensions.Mid(extensions.Find(",")+1);
			extnum++;
		}
		else
		{
			tmp = extensions;
			Setting->FileExtension[extnum] = tmp;
			extensions = extensions.Mid(extensions.Find(",") + 1);
			extnum++;
			break;
		}
	}
	Setting->ExtensionNum = extnum;
}

int InformationCount(CString arrayfilename, TesterInformation *Information)
{
	int count = MAXINFORMATIONNUM;

	for (int i = 0; i < MAXINFORMATIONNUM; i++)
	{
		if (Information[i].TesterKey == "NA")
			break;
		else if (arrayfilename.Find(Information[i].kinds) != -1 || arrayfilename.Find(Information[i].TesterKey) != -1)
			count = i;
	}
	return count;
}