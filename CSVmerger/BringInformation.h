#pragma once
#define CHECKFILENAME	TRUE
#define	CHECKFIRSTROW	FALSE
#define MAXINFORMATIONNUM	10
#define INIFILENAME	"VersionInformation.ini"

typedef struct _TesterInformation
{
	BOOL	Version1Location;
	BOOL	Version2Location;
	BOOL	Version3Location;
	BOOL	ModelLocation;
	BOOL	TesterLocation;
	CString kinds;
	CString TesterKey;
	CString Version1Key;
	CString Version2Key;
	CString Version3Key;
	CString	ModelKey;
	CString ColumnRowKey;
	CString USLRowKey;
	CString LSLRowKey;
	CString OverallKey;
	CString FindKey;
	CString OverlapKey;
}TesterInformation;

typedef struct _SettingInformation
{
	CString ReadingMode;
	CString FileExtension[MAXINFORMATIONNUM];
	int ExtensionNum;
}SettingInformation;

typedef struct _ArrayVersionList
{
	CString    Version;
	int        Informationcount;
	int        OverallCommaCount;
	CString data;
}ArrayVersionList;

void BringINIfile(TesterInformation *Information, int num, SettingInformation *Setting);
int InformationCount(CString arrayfilename, TesterInformation * Information);