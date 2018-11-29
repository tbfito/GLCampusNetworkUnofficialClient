
// GLCampusNetworkUnofficialClientDlg.h: 头文件
//

#pragma once

// 托盘图标自定义宏
#define WM_SHOWTRAY (WM_USER +1)
#define IDR_SHOW 11
#define IDR_OTHER 12
#define IDR_EXIT 13

// CGLCampusNetworkUnofficialClientDlg 对话框
class CGLCampusNetworkUnofficialClientDlg : public CDialogEx
{
// 构造
public:
	CGLCampusNetworkUnofficialClientDlg(CWnd* pParent = nullptr);			// 标准构造函数
	CListBox* listBox = nullptr;
	CStatic* statusBox = nullptr;
	void AddLog(CString strLog);											// 向列表框增加日志
	void SetStatus(CString strLog);											// 设置状态标签文本
	void LoadConfiguration();												// 加载设置
	void SaveConfiguration();												// 保存设置(内部会再次调用加载设置)
	static unsigned __stdcall Login(void *pThis);							// 认证上网
	CString strId, strPassword;												// 学号密码
	bool bAutoRun, bAutoAuth, bAutoHide;									// 选项设置
	void toTray();															// 最小化到托盘
	void deleteTray();														// 删除托盘图标
	void SendTrayMessage(CString & strMsg);									// 发送托盘信息
	afx_msg LRESULT OnShowTray(WPARAM wParam, LPARAM lParam);				// 图标恢复
	void OnSize(UINT nType, int cx, int cy);								// 主窗口大小以及状态发生变化
	void OnDestroy();														// 主窗口销毁

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLCAMPUSNETWORKUNOFFICIALCLIENT_DIALOG };
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
};
