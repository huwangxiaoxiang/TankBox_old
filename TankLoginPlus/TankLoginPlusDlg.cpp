
// TankLoginPlusDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TankLoginPlus.h"
#include "TankLoginPlusDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <fstream>
#include "HttpHelper.h"
#include <iostream>


// CTankLoginPlusDlg 对话框



CTankLoginPlusDlg::CTankLoginPlusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TANKLOGINPLUS_DIALOG, pParent)
	, is_common(FALSE)
	, pure_btn(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON5);
}

void CTankLoginPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, ServerBox);

	DDX_Check(pDX, IDC_CHECK1, pure_btn);
}

BEGIN_MESSAGE_MAP(CTankLoginPlusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTankLoginPlusDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CTankLoginPlusDlg::OnBnClickedCheck1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTankLoginPlusDlg 消息处理程序

BOOL CTankLoginPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	BaseAPI api;
	api.CMDCommand(L"dll\\TankFlow.exe");

	std::cout << "OK" << std::endl;

	for (int i = 0; i < 7; i++) {
		ServerBox.AddString(services[i]);
	}
	if (!checkDLL()) {
		exit(0);
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTankLoginPlusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTankLoginPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CTankLoginPlusDlg::checkDLL() {
	
	TCHAR dll_path[MAX_PATH] = { 0 };
	TCHAR target_path[MAX_PATH] = { 0 };
	LPTSTR tankdir = getTankDir();
	
	GetModuleFileName(NULL, dll_path, MAX_PATH);
	(_tcsrchr(dll_path, _T('\\')))[1] = 0;
	GetModuleFileName(NULL, this->current_path, MAX_PATH);//保存当前目录
	(_tcsrchr(this->current_path, _T('\\')))[1] = 0;
	PathAppend(dll_path, L"dll\\KeyBoardHOOK.dll");

	lstrcat(target_path, tankdir);
	lstrcat(target_path, L"Tank_Data\\Managed\\KeyBoardHOOK.dll");
	BOOL exist = Is_exist(dll_path);
	if (exist) {
		CopyFile(dll_path, target_path, false);
		return TRUE;
	}
	else {
		MessageBox(L"缺失重要组件，插件无法正常使用！请重新下载完整安装包", L"组件缺失", 0);
		return FALSE;
	}


}

void CTankLoginPlusDlg::OnBnClickedOk()
{
	this->GetDlgItem(IDOK)->EnableWindow(FALSE);
	CString ser;
	ServerBox.GetWindowText(ser);
	this->service = -1;
	for (int i = 0; i < 7; i++) {
		if (ser == services[i]) {
			this->service = i+1;
			break;
		}
	}
	if (service == -1) {
		MessageBox(L"请选择区服");
	}
	else {
		if (pure_btn) {
			//DirectGame(id, key, service);
			Start();
		}
		else {
			SetDlgItemText(IDOK, L"版本检查...");
			this->update_check = check_Assembly();
			if (!update_check) {//有版本更新
				MessageBox(L"坦克大战盒子已经有新版本!请前往 www.bestxiaoxiang.top 下载最新版本，否则插件将不可用!", L"版本更新", 0);
				Start();
			}
			else {
				Start();/*在游戏的直接启动器可用时，取消此注释。2019-12-23注释*/
				SetTimer(1, 1000, NULL);
			}
			SetDlgItemText(IDOK, L"开始游戏");
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		
	}
	this->GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CTankLoginPlusDlg::Start()
{
	TCHAR DataDir[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, DataDir))) {
		PathAppend(DataDir, L"Tencent\\QQMicroGameBox\\坦克大战");
	}
	int result = (int)ShellExecute(NULL, L"open", DataDir, NULL, NULL, SW_SHOW);//打开主程序
	if (result < 32)
		MessageBox(L"游戏无法启动。因为您从未使用官方启动器（微端）启动过游戏，因此您必须前往 www.tkdz.qq.com下载微端后才能正常使用盒子启动游戏。", L"启动错误", NULL);
	Sleep(3000);
}

void CTankLoginPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1) {
		HWND hwnd = api.getProcessHWND(L"#32770", L"坦克大战登陆器");
		if (hwnd != NULL) {
			KillTimer(1);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			::SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);//使其窗口最小化
			DWORD pid;
			GetWindowThreadProcessId(hwnd, &pid);//获取选择区服进程PID
			LPTSTR cmd = api.getProcCMD(pid);//获取选择区服进程命令行参数
			if (cmd != NULL) {
				::SendMessage(hwnd, WM_CLOSE, 0, 0);//关闭选择区服进程
				CString target(cmd);
				int pos1 = target.Find(L"ID=", 0);
				int pos2 = target.Find(L"Key=", 0);
				int pos3 = target.Find(L"ModelID=", 0);
				CString temp1 = target.Mid(pos1 + 3, 32);
				CString temp2 = target.Mid(pos2 + 4, 32);
				id = new TCHAR[temp1.GetLength() + 1];
				key = new TCHAR[temp2.GetLength() + 1];
				lstrcpy(id, temp1);
				lstrcpy(key, temp2);
				//api.CMDCommand(L"taskkill /im Launcher.exe /im QQMicroGameBox.exe /im  QQMicroGameBoxTray.exe /f");
				startGames(id, key, service);
			}
		}
	
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CTankLoginPlusDlg::startGames(LPTSTR ID, LPTSTR key, int serverID)
{
	TCHAR game_path[MAX_PATH] = { 0 };
	TCHAR csharp_dll[MAX_PATH] = { 0 };
	TCHAR my_dll[MAX_PATH] = { 0 };
	TCHAR my_dll2[MAX_PATH] = { 0 };
	TCHAR temp_dll[MAX_PATH] = { 0 };
	TCHAR store_dll[MAX_PATH] = { 0 };

	LPTSTR tankdir = getTankDir();
	lstrcpy(csharp_dll, tankdir);
	lstrcpy(my_dll, tankdir);
	lstrcpy(temp_dll, tankdir);
	lstrcpy(store_dll, tankdir);

	lstrcat(csharp_dll, L"Tank_Data\\Managed\\Assembly-CSharp.dll");
	lstrcat(my_dll, L"Tank_Data\\Managed\\Assembly.dll");
	
	lstrcat(temp_dll, L"Tank_Data\\Managed\\temp.dll");
	time_t now = time(0);
	_stprintf_s(store_dll, L"%sTank_Data\\Managed\\Assembly%d.dll", tankdir,(long)now);
	GetModuleFileName(NULL, my_dll2, MAX_PATH);
	(_tcsrchr(my_dll2, _T('\\')))[1] = 0;
	PathAppend(my_dll2, L"dll\\Assembly.dll");

	LPTSTR avaliable_dll;
	if (Is_exist(my_dll2))
		avaliable_dll = my_dll2;
	else if (Is_exist(my_dll))
		avaliable_dll = my_dll;
	else
		avaliable_dll = csharp_dll;

	if (avaliable_dll == csharp_dll)
		MessageBox(L"缺少必备组件，游戏将正常启动，但是功能插件将不可用！", L"组件缺失");

	BOOL inject = FALSE;
	if (this->update_check) {
		if (_trename(csharp_dll, temp_dll) == 0) {
			int m=CopyFile(avaliable_dll, csharp_dll, false);
			if (m != 0) {
				m= CopyFile(avaliable_dll, csharp_dll, false);//在失败时尝试重新复制一次
				if(m!=0)
					inject = TRUE;//文件已成功替换
			}
			if (inject == FALSE) {
				OutputDebugString(L"文件assembly替换失败!\n");
				MessageBox(L"启动过程中产生了错误，功能将不能被成功启用，请在关闭游戏后尝试重新启动盒子以解决此问题。", L"启动错误", 0);
			}
		}
		
	}

	
	Sleep(2000);
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	SetCurrentDirectory(tankdir);
	BOOL tank = CreateProcess(NULL, GameStartCMD(ID,key,serverID), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	Sleep(2000);
	if (inject) {
		DeleteFile(csharp_dll);
		int s=_trename(temp_dll, csharp_dll);
	}
	

	HWND hwnd = findWindow();
	::PostMessage(hwnd, 32770, 0, 0);
	return TRUE;
}

HWND CTankLoginPlusDlg::findWindow() {
	CWnd* window = FindWindow(L"WindowsForms10.Window.8.app.0.141b42a_r6_ad1", L"TankFlow");
	if (!window) {
		window = FindWindow(L"WindowsForms10.Window.8.app.0.141b42a_r8_ad1", L"TankFlow");
		if (!window)
			return NULL;
	}
	return window->GetSafeHwnd();
}

FILETIME CTankLoginPlusDlg::get_Filetime(LPTSTR path)
{
	FILETIME ft1;
	HANDLE hFile1 = CreateFile(path, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(hFile1, NULL,NULL, &ft1);
	CloseHandle(hFile1);
	return ft1;
}

//检测Assembly版本是否过期
BOOL CTankLoginPlusDlg::check_Assembly()
{
	HttpHelper* http = new HttpHelper();
	std::string result = http->getData("www.bestxiaoxiang.top", "/Tank_Service_SSM/last_assembly", "");
	
	//OutputDebugString(HttpHelper::UTF8ToUnicode(result).c_str());
	std::vector<std::string> m = HttpHelper::split_str(result, ",");
	std::string passwo = "";
	for (int i = 0; i < m.size(); i++) {
		std::string temp = *(m.begin() + i);
		int pos = temp.find("passwords", 0);
		if (pos > 0) {
			passwo = temp.substr(16, temp.size() - 18);
		}
	}
	//OutputDebugString(_T("分离出来的passwo="));
	//OutputDebugString(HttpHelper::UTF8ToUnicode(passwo).c_str());
	
	TCHAR crypt_path[MAX_PATH] = { 0 };
	StrCpy(crypt_path, this->current_path);
	PathAppend(crypt_path, L"\\crypt");
	std::ifstream pass_file;
	pass_file.open(crypt_path, std::ios::in);
	if (!pass_file.is_open()) {
		OutputDebugString(L"Open File crypt 错误");
		return false;
	}
	char* buf = new char[1024];
	pass_file >> buf;
	std::string passwo_(buf);
	pass_file.close();
	//OutputDebugString(_T("读取的passwo_="));
	//OutputDebugString(HttpHelper::UTF8ToUnicode(passwo_).c_str());
	if (passwo == passwo_) return true;
	else return false;

}

BOOL CTankLoginPlusDlg::DirectGame(LPTSTR ID, LPTSTR key, int serverID)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	SetCurrentDirectory(getTankDir());
	BOOL tank = CreateProcess(NULL, GameStartCMD(ID, key, serverID), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	Sleep(2000);
	return TRUE;
}

LPTSTR CTankLoginPlusDlg::GameStartCMD(LPTSTR ID, LPTSTR key, int serverID)
{
	TCHAR* DataDir = new TCHAR[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, DataDir))) {
		lstrcat(DataDir, L"\\tkdz\\Tank.exe ID:");
		lstrcat(DataDir, ID);
		lstrcat(DataDir, L",Key:");
		lstrcat(DataDir, key);
		lstrcat(DataDir, L",PID:10,serverId:");
		CString s;
		s.Format(L"%d", serverID);
		lstrcat(DataDir, s.GetBuffer());
	}
	return DataDir;
}

LPTSTR CTankLoginPlusDlg::getTankDir()
{
	TCHAR* DataDir = new TCHAR[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, DataDir))) {
		lstrcat(DataDir, L"\\tkdz\\");
	}
	return DataDir;
}

BOOL CTankLoginPlusDlg::Is_exist(LPTSTR path) {
	std::fstream _file;
	_file.open(path, std::ios::in);
	if (!_file) {
		_file.close();
		OutputDebugString(path);
		OutputDebugString(L"文件不存在!\n");
		return FALSE;
	}
	else {
		_file.close();
		OutputDebugString(path);
		OutputDebugString(L"文件存在\n");
		return TRUE;
	}
}

void CTankLoginPlusDlg::OnBnClickedCheck1()
{
	UpdateData(true);
	
}


void CTankLoginPlusDlg::OnClose()
{
	HWND hwnd = findWindow();
	::PostMessage(hwnd, WM_CLOSE, 0, 0);
	BaseAPI api;
	api.CMDCommand(L"taskkill /F /im TankFlow.exe");
	api.CMDCommand(L"taskkill /F /im TankFlow.exe");
	CDialogEx::OnClose();
}
