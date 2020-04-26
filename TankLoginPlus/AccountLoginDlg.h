#pragma once
#include <afxdialogex.h>
#include "TankLoginPlusDlg.h"
class AccountLoginDlg :
	public CDialogEx
{
	// 构造
public:
	AccountLoginDlg(CTankLoginPlusDlg* mainDlg,CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogin();

private:
	CTankLoginPlusDlg* mainDlg;
	CString inputID;
	CString inputPassword;
public:
	afx_msg void OnBnClickedRegister();
private:
	BOOL mRememberAccount;
public:
	afx_msg void OnCbnSelchangeMissile();
};

