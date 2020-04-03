
// TankLoginPlusDlg.h: 头文件
//

#pragma once
#include "BaseAPI.h"

// CTankLoginPlusDlg 对话框
class CTankLoginPlusDlg : public CDialogEx
{
// 构造
public:
	CTankLoginPlusDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TANKLOGINPLUS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL checkDLL();
	DECLARE_MESSAGE_MAP()
	CString services[7] = { CString("华南一区"),CString("华东一区"),CString("华东二区"),CString("华南二区"),CString("华北一区"),CString("华东三区"),CString("华南三区") };
	BaseAPI api;
	int service=-1;
	LPTSTR id;
	LPTSTR key;
public:
	CComboBox ServerBox;

	afx_msg void OnBnClickedOk();
	void Start();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL startGame(LPTSTR ID, LPTSTR key, int serverID);
	BOOL DirectGame(LPTSTR ID, LPTSTR key, int serverID);
	BOOL is_common;


private:
	LPTSTR GameStartCMD(LPTSTR ID, LPTSTR key, int serverID);
	LPTSTR getTankDir();
	BOOL Is_exist(LPTSTR path);
	BOOL startGames(LPTSTR ID, LPTSTR key, int serverID);
	HWND findWindow();
	FILETIME get_Filetime(LPTSTR path);

	BOOL check_Assembly();
	BOOL update_check = FALSE;

	TCHAR current_path[MAX_PATH] = { 0 };
	

public:
	BOOL pure_btn;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnClose();
};
