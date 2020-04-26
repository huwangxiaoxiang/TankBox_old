// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define PAUSE WM_USER+4
#define EVENT 30000
HHOOK hook=NULL;
HWND hwnd=NULL;
HINSTANCE   g_hInstance = NULL;
int m = 12;
HANDLE sending = NULL;
volatile bool sending_exit = false;
//LPWSTR* messes=new LPWSTR[100];
LPWSTR messes[100];
int flags[100] = { 0 };
HANDLE have_source;//已经存在多少资源
HANDLE remain_source;//剩余多少资源
volatile int in_index=0;
volatile int out_index=0;
HANDLE  mutex_in;
HANDLE mutex_out;
CRITICAL_SECTION sec;



void log(std::string s) {
	std::ofstream out("E:\\test.txt", std::ios::app);
	out << s << std::endl;
	out.close();
}

LRESULT CALLBACK KeyBoardProcess(int nCode, WPARAM wParam, LPARAM lParam)
{
	//std::ostringstream out;
	switch (nCode)
	{
	case HC_ACTION:
	{
		KBDLLHOOKSTRUCT* kHook = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYUP&&hwnd!=NULL)
		{
			//out << "nCode:" << nCode << " wparam:" << wParam << " lparam:" << kHook->vkCode;
			//log(out.str());
			if (kHook->vkCode == VK_F2)
			{
				PostMessage(hwnd, PAUSE, NULL, NULL);
			}
		}
	}

	break;
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

extern "C" _declspec(dllexport)
BOOL  Start(HWND hwnds) {
	//log("start");
	hwnd = hwnds;
	hook = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyBoardProcess,g_hInstance,NULL);
	if (hook == NULL) {
		return FALSE;
	}
	return TRUE;
}

extern "C" _declspec(dllexport)
void Exit_() {
	//log("exit");
	UnhookWindowsHookEx(hook);
}

extern "C" _declspec(dllexport)
void ReportMessage(LPWSTR message) {
	DWORD dw=WaitForSingleObject(remain_source, 300);
	if (dw != WAIT_OBJECT_0)
		return;

	EnterCriticalSection(&sec);
	LPWSTR temp = new WCHAR[lstrlen(message)];
	lstrcpy(temp, message);
	messes[in_index ] = temp;
	in_index = (in_index + 1) % 100;
	LeaveCriticalSection(&sec);

	ReleaseSemaphore(have_source, 1, NULL);
}

extern "C" _declspec(dllexport)
void ReportMessageF(LPWSTR message,int flag) {
	DWORD dw = WaitForSingleObject(remain_source, 300);
	if (dw != WAIT_OBJECT_0)
		return;

	EnterCriticalSection(&sec);
	LPWSTR temp = new WCHAR[lstrlen(message)];
	lstrcpy(temp, message);
	messes[in_index] = temp;
	flags[in_index] = flag;
	in_index = (in_index + 1) % 100;
	LeaveCriticalSection(&sec);

	ReleaseSemaphore(have_source, 1, NULL);
}

HWND findWindow() {
	HWND window = FindWindow(L"WindowsForms10.Window.8.app.0.141b42a_r6_ad1", L"TankFlow");
	if (!window) {
		window = FindWindow(L"WindowsForms10.Window.8.app.0.141b42a_r8_ad1", L"TankFlow");
		if (!window) {
			window = FindWindow(L"WindowsForms10.Window.8.app.0.2bf8098_r25_ad1", L"TankFlow");
			if (!window)
				return NULL;
		}
		
	}
	return window;
}

extern "C" _declspec(dllexport)
void NotifyMessage(int flag) {
	HWND window = findWindow();
	PostMessage(window, EVENT, 0, flag);
}



DWORD WINAPI sendThread(PVOID pvParam) {
	DWORD dwResult = 0;
	HWND window;
	while (sending_exit == false) {
		WaitForSingleObject(have_source, INFINITE);
		window = findWindow();
		COPYDATASTRUCT cds;
		cds.dwData = 0;
		cds.cbData = (lstrlen(messes[out_index])) * sizeof(WCHAR);
		cds.lpData = _malloca(cds.cbData);
		if (cds.lpData != 0) {
			lstrcpy((LPTSTR)cds.lpData, messes[out_index]);
			int result=SendMessage(window, WM_COPYDATA, flags[out_index], (LPARAM)& cds);
		}
		ReleaseSemaphore(remain_source,1,NULL);
		WaitForSingleObject(&mutex_out,INFINITE);
		out_index = (out_index + 1) % 100;
		ReleaseMutex(&mutex_out);
	}
	return (dwResult);
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	g_hInstance = (HINSTANCE)hModule;
	DWORD sendid = 0;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//log("process attach");
		have_source = CreateSemaphore(NULL, 0, 100, NULL);
		remain_source = CreateSemaphore(NULL, 100, 100, NULL);
		mutex_in = CreateMutex(NULL, TRUE, L"mutex_in");
		mutex_out = CreateMutex(NULL, FALSE, L"muex_out");
		InitializeCriticalSection(&sec);
		sending = CreateThread(NULL, 0, sendThread, NULL,0,&sendid);
		break;
    case DLL_THREAD_ATTACH:
		//log("thread attach");
		
		break;
    case DLL_THREAD_DETACH:
		//log("thread detach");
		break;
    case DLL_PROCESS_DETACH:
		sending_exit = true;
		//log("process detach");
        break;
    }
    return TRUE;
}


