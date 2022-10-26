// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	 pDeviceList = new TStringList;
}

// ---------------------------------------------------------------------------
AnsiString TMainForm::RunDos(const AnsiString &DosApp) {
	SECURITY_ATTRIBUTES Security;
	HANDLE ReadPipe, WritePipe;
	STARTUPINFO start;
	PROCESS_INFORMATION ProcessInfo;
	char *Buffer;
	DWORD BytesRead;
	AnsiString Buf;

	Security.nLength = sizeof(Security);
	Security.bInheritHandle = true;
	Security.lpSecurityDescriptor = NULL;

	// 创建一个命名管道用来捕获console程序的输出
	if (CreatePipe(&ReadPipe, &WritePipe, &Security, 0)) {
		Buffer = new char[ReadBuffer + 1];
		memset(&start, 0, sizeof(start));
		// 设置console程序的启动属性
		start.cb = sizeof(start);
		start.lpReserved = NULL;
		start.lpDesktop = NULL;
		start.lpTitle = NULL;
		start.dwX = 0;
		start.dwY = 0;
		start.dwXSize = 0;
		start.dwYSize = 0;
		start.dwXCountChars = 0;
		start.dwYCountChars = 0;
		start.dwFillAttribute = 0;
		start.cbReserved2 = 0;
		start.lpReserved2 = NULL;
		start.hStdOutput = WritePipe; // 将输出定向到我们建立的WritePipe上
		start.hStdInput = ReadPipe; // 将输入定向到我们建立的ReadPipe上
		start.hStdError = WritePipe; // 将错误输出定向到我们建立的WritePipe上
		start.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		start.wShowWindow = SW_HIDE; // 设置窗口为hide
		try {
			// 创建一个子进程，运行console程序
			if (CreateProcess(NULL, DosApp.c_str(), &Security, &Security, true,
					NORMAL_PRIORITY_CLASS, NULL, NULL, &start, &ProcessInfo)) {
				// 等待进程运行结束
				// WaitForSingleObject(ProcessInfo.hProcess, INFINITE); //独占模式
				// 响应外部消息模式
				while (TRUE) {
					DWORD result;
					MSG msg;

					result = MsgWaitForMultipleObjects
					(1, &ProcessInfo.hProcess, FALSE, INFINITE, QS_ALLINPUT);

					if (result == (WAIT_OBJECT_0)) {
						break;
					}
					else {
						PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
						DispatchMessage(&msg);
					}
				}
				// 关闭输出
				CloseHandle(WritePipe);
				Buf = "";
				// 读取console程序的输出
				do {
					BytesRead = 0;
					ReadFile(ReadPipe, Buffer, ReadBuffer, &BytesRead, NULL);
					Buf = Buf + AnsiString(Buffer, BytesRead);
					Application->ProcessMessages();
				}
				while (BytesRead >= ReadBuffer);
			}
		}
		__finally {
			delete[]Buffer;
			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
			CloseHandle(ReadPipe);
		}
	}
	return Buf;
}

void TMainForm::getenv() {
	pDeviceList->Clear();
	pDeviceList->LoadFromFile("device.txt");
	tempbat = ExtractFilePath(Application->ExeName) + "temp.bat";
	tempresultfile = ExtractFilePath(Application->ExeName) + "result.txt";
	cmdfile = ExtractFilePath(Application->ExeName) + "exec.sh";
}

AnsiString TMainForm::getcmdbat(AnsiString destip,AnsiString appuser,AnsiString appuserpwd,AnsiString rootpasswd) {
	TStringList *plist = new TStringList;

	plist->Text = Memo2->Text;
	plist->SaveToFile(cmdfile);
	plist->Clear();
	cmd = ExtractFilePath(Application->ExeName)
	+ "tool\\plink.exe" + " -ssh -l " + appuser + " -pw " + appuserpwd + " " + destip + " -m " +
	cmdfile + ">>" + tempresultfile;
	plist->Text = cmd;
	plist->SaveToFile(tempbat);
	delete plist;
}

void __fastcall TMainForm::Button1Click(TObject *Sender) {
	AnsiString appuser,appuserpwd,rootpasswd,cmd,destip;
	TStringList *psubstring = new TStringList;
	psubstring->Delimiter = '|';
	getenv();

	DeleteFileA(tempresultfile);
	for(int i=0;i<pDeviceList->Count;i++)
		{
		psubstring->DelimitedText = pDeviceList->Strings[i];
		getcmdbat(psubstring->Strings[0],psubstring->Strings[1],psubstring->Strings[2],psubstring->Strings[3]);
		RunDos(tempbat);
		}
	if (FileExists(tempresultfile))
		Memo1->Lines->LoadFromFile(tempresultfile);
	delete psubstring;
	deletetempfile();
}

void TMainForm::deletetempfile() {
	//DeleteFileA(tempbat);
	DeleteFileA(tempresultfile);
	DeleteFileA(cmdfile);
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete pDeviceList;
}
//---------------------------------------------------------------------------

