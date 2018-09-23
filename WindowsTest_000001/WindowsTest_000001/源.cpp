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
		//����ϵͳ�ڵ����ں�̬���û�̬֮��������̻��õ��˽ṹ
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

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ������ʾ: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
