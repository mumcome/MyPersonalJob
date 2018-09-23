#include <Windows.h>
#include <winternl.h>
#include <iostream>
using namespace std;
typedef NTSTATUS(NTAPI *QEURYINFORMATIONPROCESS)(IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL);
#define  pp  ;
int main(int argc, wchar_t *argv[])
{
	STARTUPINFO startupinfo{ 0 };
	PROCESS_INFORMATION processInformation{ 0 };
	BOOL bSuccess = CreateProcess(TEXT("C:\\Windows\\notepad.exe"), NULL,
		NULL, NULL, FALSE, NULL, NULL, NULL, &startupinfo, &processInformation);
	if (bSuccess)
	{
		cout << "Process start" << endl << "Process ID:\t"
			<< processInformation.dwProcessId << endl;
		PROCESS_BASIC_INFORMATION pbi;
		//操作系统在调用内核态和用户态之间的子例程会用到此结构
		/*_PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PPEB PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
}*/
		ULONG uLength = 0;

		HMODULE hDLL = LoadLibrary(TEXT("c:\\Windows\\System32\\ntdll.dll"));
		if (hDLL)
		{
			QEURYINFORMATIONPROCESS QueryInformationProcess = (QEURYINFORMATIONPROCESS)GetProcAddress(hDLL, "NtQueryInformationProcess");
			if (QueryInformationProcess)
			{
				NTSTATUS ntSataus = QueryInformationProcess(processInformation.hProcess, PROCESSINFOCLASS::ProcessBasicInformation, &pbi, sizeof(pbi), &uLength);
				if (NT_SUCCESS(ntSataus))
				{
					cout << "Process ID(from PCB):\t" << pbi.UniqueProcessId << endl;
				}
				else
				{
					cout << "Cannot open PCB!" << endl << "Error code:\t" << GetLastError() << endl;
				}
			}
			else
			{
				cout << "Cannot get" << "NTQueryInformationProcess function" << endl << "ErrorCode:\t" << GetLastError() << endl;
			}
			FreeLibrary(hDLL);
		}
		else
		{
			cout << "cannot load hdll" << endl << "Error Code:\t" << GetLastError() << endl;
		}
	}
	else
	{
		cout << "cannot open process!" << endl << "Error Code:\t" << endl;
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
