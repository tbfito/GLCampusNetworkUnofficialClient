
// GLCNUCDlg.h: 头文件
//

#pragma once

// 托盘图标自定义宏
#define WM_SHOWTRAY (WM_USER + 1)
#define IDR_SHOW 1001
#define IDR_EXIT 1002

// CGLCNUCDlg 对话框
class CGLCNUCDlg : public CDialogEx
{
// 构造
public:
	CGLCNUCDlg(CWnd* pParent = nullptr);									// 标准构造函数
	CString ConfigPath;														// 配置文件路径
	CString& SetConfigPath();												// 配置文件路径设置
	CListBox* listBox = nullptr;
	CStatic* statusBox = nullptr;
	void AddLog(CString strLog);											// 向列表框增加日志
	void SetStatus(CString strLog);											// 设置状态标签文本
	void LoadConfiguration();												// 加载设置
	void SaveConfiguration();												// 保存设置(内部会再次调用加载设置)
	static unsigned __stdcall Login(void *pThis);							// 认证上网
	static unsigned __stdcall Logout(void *pThis);							// 断开上网
	static unsigned	__stdcall AccountInfo(void *pThis);						// 帐号余额
	CString strId, strPassword, strSelfSystemURL;							// 帐号密码，自助系统URL(不含末尾/)
	bool bAutoRun, bAutoAuth, bAutoHide;									// 选项设置
	void toTray();															// 最小化到托盘
	void deleteTray();														// 删除托盘图标
	void SendTrayMessage(CString & strMsg, CString strTitle = _T("提示"));		// 发送托盘信息
	afx_msg LRESULT OnShowTray(WPARAM wParam, LPARAM lParam);				// 图标恢复
	void OnSize(UINT nType, int cx, int cy);								// 主窗口大小以及状态发生变化
	void OnDestroy();														// 主窗口将被销毁
	void OnClose();															// 主窗口可否销毁
	// 以下为登录后记录的变量，为注销服务
	CString strWlanAcIp;													// AC(接入控制器)的IP，登录后再能看到
	CString strCookie;														// 会话Cookie
	CString strLogoutURL;													// 断线URL (用的时候要补一个 strWlanAcIp)
	HANDLE  hShutdownLogoutThread = NULL;									// 断线线程句柄
	CString strSessionId;													// 认证会话ID
	CString strReferer;														// 认证来路 (断线时提供此 Referer)

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLCNUC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnIdcEdit1Click();
	afx_msg void OnIdcEdit2Click();
	afx_msg void OnIdcEdit3Click();
	afx_msg void OnBnClickedLogout();
};
