#include "pch.h"
#include "framework.h"
#include "BaseAPI.h"


int BaseAPI::TransformWidth(int old) {
	double k = 65535 / (double)screen.dmPelsWidth;
	return (int)(old * k);
}

int BaseAPI::TransformHeight(int old) {
	double k = 65535 / (double)screen.dmPelsHeight;
	return (int)(old * k);
}

BaseAPI::BaseAPI()
{
	EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &screen);
}

void BaseAPI::MoveTo(int x, int y) {
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, TransformWidth(x), TransformHeight(y), 0, 0);
}

void BaseAPI::MoveTo(POINT p)
{
	MoveTo(p.x, p.y);
}

void BaseAPI::LeftClick(int count) {
	POINT pos;
	GetCursorPos(&pos);
	for (int i = 0; i < count; i++) {
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, TransformWidth(pos.x), TransformHeight(pos.y), 0, 0);
		Sleep(100);
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, TransformWidth(pos.x), TransformHeight(pos.y), 0, 0);
		Sleep(200);
	}
}

void BaseAPI::LeftClick(HWND hwnd, int x, int y)
{
	POINT pos;
	GetCursorPos(&pos);
	int tx = TransformWidth(x);
	int ty = TransformHeight(y);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, tx, ty, 0, 0);
	Sleep(20);
	PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(tx, ty));
	PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(tx, ty));
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, TransformWidth(pos.x), TransformHeight(pos.y), 0, 0);

}

void BaseAPI::ActiveWindow(HWND hwnd)
{
	SetForegroundWindow(hwnd);
}

void BaseAPI::KeyPress(int keyvalue) {
	INPUT input[2];
	memset(input, 0, sizeof(input));
	input[0].ki.wVk = keyvalue; //按下 向下方向键
	input[0].type = INPUT_KEYBOARD;
	input[1].ki.wVk = keyvalue;//松开 向下方向键
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(2, input, sizeof(INPUT));  //该函数合成键盘事件和鼠标事件，用来模拟鼠标或者键盘操作。事件将被插入在鼠标或者键盘处理队列里面
}

void BaseAPI::KeyPress(int k1, int k2) {
	INPUT input[4];
	memset(input, 0, sizeof(input));
	input[0].ki.wVk = k1; //按下 向下方向键
	input[0].type = INPUT_KEYBOARD;
	input[1].ki.wVk = k2; //按下 向下方向键
	input[1].type = INPUT_KEYBOARD;
	input[2].ki.wVk = k2;
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;//松开 向下方向键
	input[3].ki.wVk = k1;
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;//松开 向下方向键
	SendInput(4, input, sizeof(INPUT));  //该函数合成键盘事件和鼠标事件，用来模拟鼠标或者键盘操作。事件将被插入在鼠标或者键盘处理队列里面
}


void BaseAPI::CMDCommand(LPCTSTR command)
{

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));

	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION pi;
	TCHAR commandline[MAX_PATH];
	lstrcpy(commandline, command);
	BOOL bRet = CreateProcess(NULL, commandline, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);//
	if (bRet)
	{
		WaitForSingleObject(pi.hProcess, 1000);// 等待程序退出
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

LPTSTR BaseAPI::getProcCMD(WORD pid)
{
	NtQueryInformationProcessFake ntQ = NULL;
	HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	LPTSTR result = NULL;
	if (INVALID_HANDLE_VALUE != hproc) {
		HANDLE hnewdup = NULL;
		PEB peb;
		RTL_USER_PROCESS_PARAMETERS upps;
		WCHAR buffer[MAX_PATH] = { NULL };
		HMODULE hm = LoadLibrary(L"Ntdll.dll");
		if (hm != NULL) {
			ntQ = (NtQueryInformationProcessFake)GetProcAddress(hm, "NtQueryInformationProcess");
			if (DuplicateHandle(GetCurrentProcess(), hproc, GetCurrentProcess(), &hnewdup, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
				PROCESS_BASIC_INFORMATION pbi;
				NTSTATUS isok = ntQ(hnewdup, 0, (PVOID)& pbi, sizeof(PROCESS_BASIC_INFORMATION), 0);
				if (BCRYPT_SUCCESS(isok)) {
					if (ReadProcessMemory(hnewdup, pbi.PebBaseAddress, &peb, sizeof(PEB), 0))
						if (ReadProcessMemory(hnewdup, peb.ProcessParameters, &upps, sizeof(RTL_USER_PROCESS_PARAMETERS), 0)) {
							WCHAR* buffer = new WCHAR[upps.CommandLine.Length + 1];
							ZeroMemory(buffer, (upps.CommandLine.Length + 1) * sizeof(WCHAR));
							ReadProcessMemory(hnewdup, upps.CommandLine.Buffer, buffer, upps.CommandLine.Length, 0);
							result = buffer;
						}
				}
				CloseHandle(hnewdup);
			}
		}
		CloseHandle(hproc);
	}
	return result;
}

RECT BaseAPI::getProcessRect()
{
	return getProcessRect(className, windowName);
}

RECT BaseAPI::getProcessRect(LPCTSTR className, LPCTSTR windowName)
{
	HWND client = getProcessHWND(className,windowName);
	return getProcessRect(client);
}

RECT BaseAPI::getProcessRect(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	return rect;
}

RECT BaseAPI::getProcessClient()
{
	return getProcessClient(className, windowName);
}

RECT BaseAPI::getProcessClient(LPCTSTR className, LPCTSTR windowName)
{
	HWND client = getProcessHWND(className,windowName);
	if (client == NULL) return RECT();
	return getProcessClient(client);
}

RECT BaseAPI::getProcessClient(HWND hwnd)
{
	RECT result, clients;
	GetWindowRect(hwnd, &result);
	GetClientRect(hwnd, &clients);
	result.top = result.bottom - clients.bottom;
	result.right = result.left + clients.right;
	return result;
}

HWND BaseAPI::getProcessHWND(LPCTSTR className, LPCTSTR windowName)
{
	HWND client = FindWindow(className, windowName);
	return client;
}

HWND BaseAPI::getProcessHWND()
{
	HWND client = FindWindow(this->className,this->windowName);
	return client;
}

void BaseAPI::SetDefaultProcess(LPCTSTR className, LPCTSTR windowName)
{
	this->className = className;
	this->windowName = windowName;
}
