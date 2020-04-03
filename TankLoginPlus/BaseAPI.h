#pragma once
#include <Windows.h>
#include "winternl.h"
#include <bcrypt.h>
#include <Shlwapi.h>

typedef NTSTATUS(WINAPI* NtQueryInformationProcessFake)(HANDLE, DWORD, PVOID, ULONG, PULONG);

class BaseAPI
{
public:
	/*将鼠标移动至屏幕(x,y)位置*/
	void MoveTo(int x, int y);
	void MoveTo(POINT p);

	//使用虚拟按钮的方式模拟鼠标点击count次
	void LeftClick(int count);

	//使用进程消息发送的方式进行鼠标点击，无需窗口激活
	void LeftClick(HWND hwnd, int x, int y);

	//激活某个进程窗口放置在最前
	void ActiveWindow(HWND hwnd);

	//使用虚拟按键方式进行按钮单击模拟
	void KeyPress(int);

	//使用虚拟按键方式进行组合键单击模拟
	void KeyPress(int, int);

	//获取进程窗口矩形（带标题栏）
	RECT getProcessRect();
	RECT getProcessRect(LPCTSTR className, LPCTSTR windowName);
	RECT getProcessRect(HWND hwnd);

	//获取客户区矩形（去掉标题栏）
	RECT getProcessClient();
	RECT getProcessClient(LPCTSTR className, LPCTSTR windowName);
	RECT getProcessClient(HWND hwnd);

	//获取游戏窗口句柄
	HWND getProcessHWND(LPCTSTR className, LPCTSTR windowName);
	HWND getProcessHWND();

	//设置目标进程窗口名称和类名
	void SetDefaultProcess(LPCTSTR className, LPCTSTR windowName);

	//执行控制台命令
	void CMDCommand(LPCTSTR command);

	//获取进程输入参数
	LPTSTR getProcCMD(WORD pid);

private:
	int TransformWidth(int);
	int TransformHeight(int);
	//屏幕分辨率信息
	DEVMODE screen;
	LPCTSTR className = NULL;
	LPCTSTR windowName = NULL;
public:
	BaseAPI();
};

