
// TankLoginPlusDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TankLoginPlus.h"
#include "TankLoginPlusDlg.h"
#include "afxdialogex.h"
#include "AccountLoginDlg.h"
#include "BoxReceiver.h"
#include "ServerListManager.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CTankLoginPlusDlg 对话框



CTankLoginPlusDlg::CTankLoginPlusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TANKLOGINPLUS_DIALOG, pParent)
	, is_common(FALSE)
	, pure_btn(FALSE)
	, right_lock(TRUE)
	, damage_panel(TRUE)
	, disable_speak(TRUE)
	, night_speak(TRUE)
	, battle_evaluate(TRUE)
	, loginText(_T(""))
	, disable_chat(FALSE)
	, audio_recognize(FALSE)
	, red_point(TRUE)
	, safe_distance(TRUE)
	, viewDistance(TRUE)
	, short_chat(TRUE)
	, fast_upgrade(FALSE)
	, auto_cruise(TRUE)
	, pre_name(TRUE)
	, block_aura(FALSE)
	, for_video(FALSE)
	, viewCircle(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON5);
}

void CTankLoginPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, ServerBox);

	DDX_Check(pDX, IDC_CHECK1, pure_btn);
	DDX_Check(pDX, IDC_CHECK2, right_lock);
	DDX_Check(pDX, IDC_CHECK3, damage_panel);
	DDX_Check(pDX, IDC_CHECK4, disable_speak);
	DDX_Check(pDX, IDC_CHECK5, night_speak);
	DDX_Check(pDX, IDC_CHECK6, battle_evaluate);
	DDX_Text(pDX, IDC_LOGINTEXT, loginText);
	DDX_Check(pDX, IDC_CHECK8, disable_chat);
	DDX_Check(pDX, IDC_CHECK9, audio_recognize);
	DDX_Check(pDX, IDC_CHECK10, red_point);
	DDX_Check(pDX, IDC_CHECK11, safe_distance);
	DDX_Control(pDX, IDC_COMBO2, missile_kind);
	DDX_Check(pDX, IDC_CHECK12, viewDistance);
	DDX_Check(pDX, IDC_CHECK13, short_chat);
	DDX_Check(pDX, IDC_CHECK14, fast_upgrade);
	DDX_Control(pDX, IDC_SLIDER1, damage_slider);
	DDX_Control(pDX, IDC_COMBO3, enermy_missile);
	DDX_Check(pDX, IDC_CHECK7, auto_cruise);
	DDX_Check(pDX, IDC_CHECK15, pre_name);
	DDX_Check(pDX, IDC_CHECK16, block_aura);
	DDX_Check(pDX, IDC_CHECK17, for_video);
	DDX_Check(pDX, IDC_CHECK18, viewCircle);
}

BEGIN_MESSAGE_MAP(CTankLoginPlusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTankLoginPlusDlg::OnBnClickedStartGame)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CTankLoginPlusDlg::OnBnClickedCheck1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK2, &CTankLoginPlusDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CTankLoginPlusDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CTankLoginPlusDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CTankLoginPlusDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CTankLoginPlusDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_BUTTON1, &CTankLoginPlusDlg::OnBnClickedLoginBtn)
	ON_BN_CLICKED(IDC_BUTTON2, &CTankLoginPlusDlg::OnBnClickedForBattleResult)
	ON_BN_CLICKED(IDC_CHECK8, &CTankLoginPlusDlg::OnBnClickedDisableChat)
	ON_BN_CLICKED(IDC_CHECK9, &CTankLoginPlusDlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CTankLoginPlusDlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK11, &CTankLoginPlusDlg::OnBnClickedCheck11)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTankLoginPlusDlg::OnCbnSelchangeMissile)
	ON_BN_CLICKED(IDC_CHECK12, &CTankLoginPlusDlg::OnBnClickedCheck12)
	ON_BN_CLICKED(IDC_CHECK13, &CTankLoginPlusDlg::OnBnClickedCheck13)
	ON_EN_CHANGE(IDC_EDIT1, &CTankLoginPlusDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK14, &CTankLoginPlusDlg::OnBnClickedFastUpgrade)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CTankLoginPlusDlg::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CTankLoginPlusDlg::OnNMCustomdrawSlider1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CTankLoginPlusDlg::OnCbnSelchangeEnermyMissile)
	ON_BN_CLICKED(IDC_CHECK7, &CTankLoginPlusDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK15, &CTankLoginPlusDlg::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_CHECK16, &CTankLoginPlusDlg::OnBnClickedCheck16)
	ON_BN_CLICKED(IDC_CHECK17, &CTankLoginPlusDlg::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_CHECK18, &CTankLoginPlusDlg::OnBnClickedCheck18)
END_MESSAGE_MAP()


// CTankLoginPlusDlg 消息处理程序

BOOL CTankLoginPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	GetModuleFileName(NULL, this->current_path, MAX_PATH);//保存当前目录
	(_tcsrchr(this->current_path, _T('\\')))[1] = 0;

	this->damage_slider.SetRange(1500, 100000);

	BaseAPI api;
	api.CMDCommand(L"dll\\TankFlow.exe");
	api.CMDCommand(L"dll\\Audio.exe");
	ServerListManager::Inistance();

	std::vector<CString> servers = ServerListManager::GetServerNameList();
	for (auto& i : servers) {
		ServerBox.AddString(i);
	}

	for (int i = 0; i < 8; i++) {
		missile_kind.AddString(missiles[i]);
		enermy_missile.AddString(missiles[i]);
	}

	if (!checkDLL()) {
		exit(0);
	}

	if (this->mCheckBoxConfig()) {
		this->InitPlugin();//适用于有配置文件情况下的默认设置
	}
	//适用于没有配置文件的情况下的默认设置
	else {
		this->night_speak = FALSE;
		this->disable_chat = FALSE;
		this->audio_recognize = TRUE;
		this->auto_cruise = TRUE;
		this->pre_name = TRUE;
		this->block_aura = FALSE;
		this->for_video = FALSE;
		this->viewCircle = TRUE;
		this->damage_slider.EnableWindow(false);
		this->missile_kind.SetCurSel(3);
		this->enermy_missile.SetCurSel(5);
		UpdateData(false);
		this->savePlugin();
	}

	this->mReadUserConfig();

	
	
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
	return TRUE;
	TCHAR dll_path[MAX_PATH] = { 0 };
	TCHAR target_path[MAX_PATH] = { 0 };
	LPTSTR tankdir = getTankDir();
	
	GetModuleFileName(NULL, dll_path, MAX_PATH);
	(_tcsrchr(dll_path, _T('\\')))[1] = 0;
	
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

void CTankLoginPlusDlg::OnBnClickedStartGame()
{
	this->GetDlgItem(IDOK)->EnableWindow(FALSE);
	CString ser;
	ServerBox.GetWindowText(ser);
	this->service = ServerListManager::GetServerId(ser);

	if (service <= 0) {
		MessageBox(L"请选择区服");
	}
	else if(service==8){//选择的是冒险岛特区
		if (pure_btn) {
			//DirectGame(id, key, service);
			MessageBox(L"请直接从官方游戏启动入口启动游戏", L"纯净模式", MB_ICONINFORMATION | MB_OK);
		}
		SetDlgItemText(IDOK, L"版本检查...");
		this->update_check = check_Assembly();

		if (this->update_check) {
			SetDlgItemText(IDOK, L"等待游戏启动...");
			SetTimer(4, 1000, NULL);
			MessageBox(L"盒子已就绪，现在您可以正常登录游戏。新区启动与老区启动稍有不同，现在请从官方游戏登录入口登录游戏，盒子功能将自动启用！", L"启动成功", MB_ICONINFORMATION | MB_OK);
		}

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
				startGames(GameStartCMD(id, key, service));
				

			}
		}
	
	}
	else if (nIDEvent == 2) {
		KillTimer(2);
		AccountLoginDlg login(this);
		login.DoModal();
	}
	else if (nIDEvent == 4) {
		HWND hwnd = api.getProcessHWND(L"UnityWndClass", L"Tank Battle");
		if (hwnd != NULL) {
			KillTimer(4);
			DWORD pid;
			::SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);//使其窗口最小化
			GetWindowThreadProcessId(hwnd, &pid);//获取选择区服进程PID
			LPTSTR cmd = api.getProcCMD(pid);//获取选择区服进程命令行参数
			api.CMDCommand(L"taskkill /im Tank.exe /f");
			Sleep(500);
			startGames(cmd);
			Sleep(2000);

			SetDlgItemText(IDOK, L"启动游戏");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
		
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CTankLoginPlusDlg::startGames(LPTSTR startCMD)
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
	BOOL tank = CreateProcess(NULL, startCMD, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	Sleep(2000);
	if (inject) {
		DeleteFile(csharp_dll);
		int s=_trename(temp_dll, csharp_dll);
	}

	return TRUE;
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
	HttpHelper* http = new HttpHelper(3);
	std::string result = http->getData("www.bestxiaoxiang.top", "/Tank_Service_SSM/last_assembly", "");
	
	std::vector<std::string> m = HttpHelper::split_str(result, ",");
	std::string passwo = "";
	for (int i = 0; i < m.size(); i++) {
		std::string temp = *(m.begin() + i);
		int pos = temp.find("passwords", 0);
		if (pos > 0) {
			passwo = temp.substr(16, temp.size() - 18);
		}
	}
	if (passwo == "") {
		MessageBox(L"网络出错，无法检查更新版本，请重试！插件功能将不可用。", L"网络错误", MB_OK);
		return false;
	}

	TCHAR crypt_path[MAX_PATH] = { 0 };
	StrCpy(crypt_path, this->current_path);
	PathAppend(crypt_path, L"\\crypt");
	std::ifstream pass_file;
	pass_file.open(crypt_path, std::ios::in);
	if (!pass_file.is_open()) {
		OutputDebugString(L"Open File crypt 错误");
		MessageBox(L"文件缺失，所有插件将不可用。请您重新下载完整安装包后重新启动盒子", L"文件缺失", MB_OK);
		MessageBox(this->current_path, L"", MB_OK);
		MessageBox(crypt_path, L"", MB_OK);
		return false;
	}
	char* buf = new char[1024];
	pass_file >> buf;
	std::string passwo_(buf);
	pass_file.close();
	OutputDebugString(_T("\n读取的passwo_="));
	OutputDebugString(HttpHelper::UTF8ToUnicode(passwo_).c_str());
	OutputDebugString(_T("\n"));
	if (passwo == passwo_) return true;
	else {
		MessageBox(L"坦克大战盒子已经有新版本!请前往 www.bestxiaoxiang.top 下载最新版本，否则插件将不可用!", L"版本更新", MB_OK);
		return false;
	}

}

BOOL CTankLoginPlusDlg::DirectGame(LPTSTR ID, LPTSTR key, int serverID)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	SetCurrentDirectory(getTankDir());
	BOOL tank = CreateProcess(NULL, GameStartCMD(ID, key, serverID), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
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
BOOL CTankLoginPlusDlg::getPluginState(std::string name)
{
	std::string temp =TankPluginManager::getPluginState(name);
	if (temp == "1"||temp=="")
		return TRUE;
	else
		return FALSE;
}
void CTankLoginPlusDlg::setPluginState(std::string name, BOOL state)
{
	std::string m = "0";
	if (state) m = "1";
	TankPluginManager::setPlugin(name, m);
}

//初始化插件启用状态，适用于有配置文件情况下的默认设置
void CTankLoginPlusDlg::InitPlugin()
{
	LPTSTR path = this->getTankDir();
	PathAppend(path, L"Tank_Data\\Boxconfig"); 
	
	TankPluginManager::initPlugin(path);
	
	this->right_lock = this->getPluginState("right_lock");
	this->damage_panel = this->getPluginState("damage_panel");
	this->disable_speak = this->getPluginState("disable_speak");
	this->night_speak = this->getPluginState("night_speak");
	this->battle_evaluate = this->getPluginState("battle_evaluate");
	if (TankPluginManager::hasPlugin("disable_chat")) {
		this->disable_chat = this->getPluginState("disable_chat");
	}
	else {
		this->disable_chat = false;
		this->setPluginState("disable_chat", false);
	}

	if (TankPluginManager::hasPlugin("audio_recognize")) {
		this->audio_recognize = this->getPluginState("audio_recognize");
	}
	else {
		this->audio_recognize = true;
		this->setPluginState("audio_recognize",true);
	}

	if (TankPluginManager::hasPlugin("red_point")) {
		this->red_point = this->getPluginState("red_point");
	}
	else {
		this->red_point = true;
		this->setPluginState("red_point", true);
	}

	if (TankPluginManager::hasPlugin("safe_distance")) {
		this->safe_distance = this->getPluginState("safe_distance");
	}
	else {
		this->safe_distance = true;
		this->setPluginState("safe_distance", true);
	}

	if (TankPluginManager::hasPlugin("missile_kind")) {
		std::string text = TankPluginManager::getPluginState("missile_kind");
		OutputDebugStringA(text.c_str());
		this->missile_kind.SetCurSel(std::atoi(text.c_str()) - 1);
	}
	else {
		this->missile_kind.SetCurSel(3);
		TankPluginManager::setPlugin("missile_kind", "4");
	}

	if (TankPluginManager::hasPlugin("enermy_missile")) {
		std::string text = TankPluginManager::getPluginState("enermy_missile");
		OutputDebugStringA(text.c_str());
		this->enermy_missile.SetCurSel(std::atoi(text.c_str()) - 1);
	}
	else {
		this->enermy_missile.SetCurSel(5);
		TankPluginManager::setPlugin("enermy_missile", "6");
	}

	if (TankPluginManager::hasPlugin("viewDistance")) {
		this->viewDistance = this->getPluginState("viewDistance");
	}
	else {
		this->viewDistance = true;
		this->setPluginState("viewDistance", true);
	}

	if (TankPluginManager::hasPlugin("short_chat")) {
		this->short_chat = this->getPluginState("short_chat");
	}
	else {
		this->short_chat = true;
		this->setPluginState("short_chat", true);
	}

	if (TankPluginManager::hasPlugin("block_aura")) {
		this->block_aura = this->getPluginState("block_aura");
	}
	else {
		this->block_aura = false;
		this->setPluginState("block_aura",false);
	}

	if (TankPluginManager::hasPlugin("for_video")) {
		this->for_video = this->getPluginState("for_video");
	}
	else {
		this->for_video = false;
		this->setPluginState("for_video", false);
	}

	if (TankPluginManager::hasPlugin("fast_upgrade")) {
		std::string text = TankPluginManager::getPluginState("fast_upgrade");
		int pos = std::atoi(text.c_str());
		OutputDebugStringA("OK\n");
		OutputDebugStringA(text.c_str());
		if (pos == 0) {
			this->damage_slider.EnableWindow(false);
			GetDlgItem(ID_DAMAGE_MAX)->EnableWindow(false);
			this->damage_slider.SetPos(1500);
			this->fast_upgrade = false;
		}
		else {
			this->damage_slider.EnableWindow(true);
			GetDlgItem(ID_DAMAGE_MAX)->EnableWindow(true);
			this->damage_slider.SetPos(pos);
			this->fast_upgrade =true;
		}
	}
	else {
		
		this->damage_slider.EnableWindow(false);
		GetDlgItem(ID_DAMAGE_MAX)->EnableWindow(false);
		this->damage_slider.SetPos(1500);
		this->fast_upgrade = false;
		TankPluginManager::setPlugin("fast_upgrade", "0");
	}

	if (TankPluginManager::hasPlugin("auto_cruise")) {
		this->auto_cruise= this->getPluginState("auto_cruise");
	}
	else {
		this->auto_cruise = true;
		this->setPluginState("auto_cruise", true);
	}
	if (TankPluginManager::hasPlugin("pre_name")) {
		this->pre_name = this->getPluginState("pre_name");
	}
	else {
		this->pre_name = true;
		this->setPluginState("pre_name", true);
	}

	if (TankPluginManager::hasPlugin("viewCircle")) {
		this->viewCircle = this->getPluginState("viewCircle");
	}
	else {
		this->viewCircle = true;
		this->setPluginState("viewCircle", true);
	}

	UpdateData(false);
}


//保存插件启用配置信息
void CTankLoginPlusDlg::savePlugin()
{
	UpdateData(true);

	this->setPluginState("right_lock", this->right_lock);
	this->setPluginState("damage_panel", this->damage_panel);
	this->setPluginState("disable_speak", this->disable_speak);
	this->setPluginState("night_speak", this->night_speak);
	this->setPluginState("battle_evaluate", this->battle_evaluate);
	this->setPluginState("disable_chat",this->disable_chat);
	this->setPluginState("audio_recognize", this->audio_recognize);
	this->setPluginState("red_point", this->red_point);
	this->setPluginState("safe_distance", this->safe_distance);
	this->setPluginState("viewDistance", this->viewDistance);
	this->setPluginState("short_chat", this->short_chat);
	this->setPluginState("auto_cruise", this->auto_cruise);
	this->setPluginState("pre_name", this->pre_name);
	this->setPluginState("block_aura", this->block_aura);
	this->setPluginState("for_video", this->for_video);
	this->setPluginState("viewCircle", this->viewCircle);

	TankPluginManager::setPlugin("missile_kind", std::to_string(this->missile_kind.GetCurSel()+1)); 
	TankPluginManager::setPlugin("enermy_missile", std::to_string(this->enermy_missile.GetCurSel() + 1));

	if (this->fast_upgrade) {
		TankPluginManager::setPlugin("fast_upgrade", std::to_string(this->damage_slider.GetPos()));
	}
	else {
		this->setPluginState("fast_upgrade", false);
	}

}

BOOL CTankLoginPlusDlg::mCheckBoxConfig()
{
	LPTSTR path = this->getTankDir();
	PathAppend(path, L"Tank_Data\\Boxconfig");
	TankPluginManager::mSetConfigPath(path);
	return this->Is_exist(path);
}

void CTankLoginPlusDlg::mReadUserConfig()
{
	LPTSTR path = this->getTankDir();
	PathAppend(path, L"Tank_Data\\userConfig");
	if (!this->Is_exist(path)) return;

	std::ifstream pass_file;
	pass_file.open(path, std::ios::in);
	if (!pass_file.is_open()) {
		OutputDebugString(L"Open File userConfig 错误");
		return;
	}
	std::string id;
	std::string name;
	std::string email;
	try {
		pass_file >> id;
		pass_file >> name;
		pass_file >> email;
		if (id == ""||email=="") {
			SetTimer(2, 1000, NULL);
			return;
		}
		this->setAccount(id, name,email);
		this->setLoginState(TRUE);
	}
	catch (std::string ) {
	}
	pass_file.close();
}

void CTankLoginPlusDlg::mSaveAccount(std::string id, std::string name,std::string email)
{
	LPTSTR path = this->getTankDir();
	PathAppend(path, L"Tank_Data\\userConfig");

	std::fstream _file;
	_file.open(path, std::ios::out);
	_file << id;
	_file << " ";
	_file << name;
	_file << " ";
	_file << email;
	_file.close();
	
}

void CTankLoginPlusDlg::setAccount(std::string id, std::string name,std::string email)
{
	this->accountId = id;
	this->accountName = name;
	this->accountEmail = email;
	GetDlgItem(IDC_LOGINTEXT)->EnableWindow(true);
	std::wstring m = HttpHelper::UTF8ToUnicode(name);
	this->loginText = CStringW(m.c_str()) + L"  (" + CStringW(email.c_str()) + L")   已登录";
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	UpdateData(false);


}

void CTankLoginPlusDlg::setLoginState(BOOL is_login)
{
	this->isLogin = is_login;
	std::stringstream ss;
	ss << "&3 ";
	if (isLogin) {
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(L"注销");
		GetDlgItem(IDC_LOGINTEXT)->EnableWindow(true);
		ss << this->accountId.c_str();
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->SetWindowText(L"查看今日战绩详情");
	}
	else {
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(L"登录");
		GetDlgItem(IDC_LOGINTEXT)->EnableWindow(false);
		this->loginText = L"未登录";
		ss << "-1";
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->SetWindowText(L"今日战绩(登录后可查看)");
	}

	BoxReceiver tankflow = BoxReceiver();
	tankflow.ConnectServer();
	tankflow.SendData(ss.str());
	tankflow.DisConnect();
	/*
	std::wstring m = HttpHelper::UTF8ToUnicode(ss.str());
	HWND hwnd = findWindow();
	COPYDATASTRUCT cds;
	cds.dwData = 0;
	cds.cbData = (lstrlen(m.c_str())) * sizeof(WCHAR);
	cds.lpData = _malloca(cds.cbData);
	if (cds.lpData != 0) {
		lstrcpy((LPTSTR)cds.lpData, m.c_str());
		int result = ::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)& cds);
	}
	*/
}

void CTankLoginPlusDlg::OnBnClickedCheck1()
{
	UpdateData(true);
}


void CTankLoginPlusDlg::OnClose()
{
	BaseAPI api;
	api.CMDCommand(L"taskkill /F /im TankFlow.exe");
	api.CMDCommand(L"taskkill /F /im TankFlow.exe");
	api.CMDCommand(L"taskkill /F /im Audio.exe");
	api.CMDCommand(L"taskkill /F /im Audio.exe");
	CDialogEx::OnClose();
}


void CTankLoginPlusDlg::OnBnClickedCheck2()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功",MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck3()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck4()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck5()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck6()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedLoginBtn()
{
	if (this->isLogin) {
		this->setLoginState(false);
		this->UpdateData(false);
		this->mSaveAccount("", "","");
	}
	else {
		AccountLoginDlg login(this);
		login.DoModal();
	}
	
}


void CTankLoginPlusDlg::OnBnClickedForBattleResult()
{
	if (!isLogin) {
		AccountLoginDlg login(this);
		login.DoModal();
	}
	else {
		std::stringstream ss;
		ss << "https://www.bestxiaoxiang.top/TankBox/tank_data/battle_result.html?param=";
		ss << this->accountId.c_str();
		ShellExecute(NULL, _T("open"), HttpHelper::UTF8ToUnicode(ss.str()).c_str(), NULL,NULL, SW_SHOW);
	}
	
}


void CTankLoginPlusDlg::OnBnClickedDisableChat()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck9()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！该功能为高级账号功能，若您不是高级账号用户，则该功能不会生效，请知悉。", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck10()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck11()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnCbnSelchangeMissile()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck12()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck13()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CTankLoginPlusDlg::OnBnClickedFastUpgrade()
{
	
	this->savePlugin();
	if (this->fast_upgrade) {
		this->damage_slider.EnableWindow(true);
		GetDlgItem(ID_DAMAGE_MAX)->EnableWindow(true);
	}
	else {
		this->damage_slider.EnableWindow(false);
		GetDlgItem(ID_DAMAGE_MAX)->EnableWindow(false);
	}
}


void CTankLoginPlusDlg::OnNMReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！该功能仅限高级账号使用。", L"设置成功", MB_OK);
	*pResult = 0;
}



void CTankLoginPlusDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	std::wstringstream ss;
	ss << this->damage_slider.GetPos();

	GetDlgItem(ID_DAMAGE_MAX)->SetWindowText(ss.str().c_str());
	*pResult = 0;
}


void CTankLoginPlusDlg::OnCbnSelchangeEnermyMissile()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck7()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck15()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck16()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！请注意，该选项会屏蔽所有技能效果，能有效提升游戏FPS的稳定性。", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck17()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}


void CTankLoginPlusDlg::OnBnClickedCheck18()
{
	this->savePlugin();
	MessageBox(L"设置已保存，最迟下一场战斗生效！", L"设置成功", MB_OK);
}
