// WindowsTest_000002.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <tchar.h>

#define COMMUNICATION_OBJECT_NAME TEXT("_FILE_MAPPING_")
#define SYNCHRONZING_MUTEX_NAME TEXT("_TEST_MUTEX_")
typedef struct _tagCOMMUNICATIONOBJECT
{
	HWND hWndClient;
	BOOL bExitLoop;
	LONG lSleepTimeout;
}COMMUNICATIONOBJECT, *PCOMMUNICATONOBJECT;
int _tmain(int argc,_TCHAR * argv[])
{
	COMMUNICATIONOBJECT P;
	HBRUSH hBrush = NULL;
	if (_tcscmp(TEXT("blue"),argv[0])==0)
	{
		hBrush = CreateSolidBrush(RGB(0,0,255));
	}
	else
	{
		hBrush = CreateSolidBrush(RGB(255,0,0));
	}
	HWND hWnd = NULL;
	HDC hDC = NULL;
	RECT rectClient = { 0 };
	LONG lWaitTimeout = 0;
	HANDLE hMapping = NULL;
	PCOMMUNICATONOBJECT pCommObject = NULL;
	BOOL bContinueLoop = TRUE;

	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, SYNCHRONZING_MUTEX_NAME);//OpenMutex函数为现有的一个已命名互斥体对象创建一个新句柄。
	
	//函数功能：
	/*函数原型： HANDLE OpenMutex(
		DWORD dwDesiredAccess, // access
		BOOL bInheritHandle, // inheritance option
		LPCTSTR lpName // object name
	);
	参数：
		dwDesiredAccess：
		MUTEX_ALL_ACCESS 请求对互斥体的完全访问
		MUTEX_MODIFY_STATE 允许使用 ReleaseMutex 函数
		SYNCHRONIZE 允许互斥体对象同步使用
		bInheritHandle : 如希望子进程能够继承句柄，则为TRUE
		lpName ：要打开对象的名字
		返回值：如执行成功，返回对象的句柄；零表示失败。若想获得更多错误信息，请调用GetLastError函数。
		备注：一旦不再需要，注意一定要用 CloseHandle 关闭互斥体句柄。如对象的所有句柄都已关闭，那么对象也会删除
	*/
	hMapping = OpenFileMapping(FILE_MAP_READ, FALSE, COMMUNICATION_OBJECT_NAME);//OpenFileMapping是打开一个现成的文件映射对象的函数。
	if (hMapping)
	{
		while (bContinueLoop)
		{
			WaitForSingleObject(hMutex, INFINITE);/*WaitForSingleObject是一种Windows API函数，当等待仍在挂起状态时，
												  句柄被关闭，那么函数行为是未定义的。该句柄必须具有 SYNCHRONIZE 访问权限。
													WaitForSingleObject函数用来检测hHandle事件的信号状态，
													在某一线程中调用该函数时，线程暂时挂起，如果在挂起的dwMilliseconds毫秒内
													，线程所等待的对象变为有信号状态，则该函数立即返回；
													如果超时时间已经到达dwMilliseconds毫秒，但hHandle所指向的对象还没有变成有信号状态，函数照样返回。*/
			pCommObject = (PCOMMUNICATONOBJECT)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, sizeof(COMMUNICATIONOBJECT));
			if (pCommObject)
			{
				bContinueLoop = !pCommObject->bExitLoop;
				hWnd = pCommObject->hWndClient;
				lWaitTimeout = pCommObject->lSleepTimeout;
				UnmapViewOfFile(pCommObject);
				hDC = GetDC(hWnd);
				if (GetClientRect(hWnd,&rectClient))
				{
					FillRect(hDC, &rectClient,hBrush);
				}
				ReleaseDC(hWnd, hDC);
				Sleep(lWaitTimeout);

			}
			ReleaseMutex(hMutex);
		}
	}
	CloseHandle(hMapping);
	CloseHandle(hMutex);
	DeleteObject(hBrush);
	return 0;
}//60页
//aaaaaaaa阿斯达所大所大所多aa

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
