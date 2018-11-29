// GLCampusNetworkUnofficialClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "GLCampusNetworkUnofficialClient.h"
#include "GLCampusNetworkUnofficialClientDlg.h"
#include "afxdialogex.h"
#include "common.h"
#include "CCurlTask.h"
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// 关于对话框消息注册
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGLCampusNetworkUnofficialClientDlg 对话框构造
CGLCampusNetworkUnofficialClientDlg::CGLCampusNetworkUnofficialClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLCAMPUSNETWORKUNOFFICIALCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGLCampusNetworkUnofficialClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// 主对话框消息注册
BEGIN_MESSAGE_MAP(CGLCampusNetworkUnofficialClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOWTRAY, OnShowTray) // 托盘消息映射
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CGLCampusNetworkUnofficialClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGLCampusNetworkUnofficialClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &CGLCampusNetworkUnofficialClientDlg::OnIdcEdit1Click)
	ON_BN_CLICKED(IDC_CHECK2, &CGLCampusNetworkUnofficialClientDlg::OnIdcEdit2Click)
	ON_BN_CLICKED(IDC_CHECK3, &CGLCampusNetworkUnofficialClientDlg::OnIdcEdit3Click)
END_MESSAGE_MAP()

// CGLCampusNetworkUnofficialClientDlg 消息处理程序
BOOL CGLCampusNetworkUnofficialClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//////////////////////////////////////////////////////////////// 在此添加额外的初始化代码

	// 增加托盘图标
	this->toTray();

	// 设置对象实例
	listBox = (CListBox*)GetDlgItem(IDC_LIST1);
	statusBox = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);

	// 加载配置
	this->LoadConfiguration();

	// 显示版本信息
	CStringA appInfo;
	appInfo.Format("广陵校园网客户端(非官方), %s", __DATE__);
	this->AddLog(CA2W(appInfo.GetString()).m_psz);

	// 运行后自动认证开关为真的话，就立即运行认证
	if (this->bAutoAuth) {
		this->OnBnClickedOk();
	}

	/////////////////////////////////////////////////////////////////

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGLCampusNetworkUnofficialClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。
void CGLCampusNetworkUnofficialClientDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CGLCampusNetworkUnofficialClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 加载配置
void CGLCampusNetworkUnofficialClientDlg::LoadConfiguration() {

	this->bAutoRun = GetPrivateProfileInt(L"Config", L"bAutoRun", 0, L".\\Config.ini");
	this->bAutoAuth = GetPrivateProfileInt(L"Config", L"bAutoAuth", 0, L".\\Config.ini");
	this->bAutoHide = GetPrivateProfileInt(L"Config", L"bAutoHide", 0, L".\\Config.ini");
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(bAutoRun);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(bAutoAuth);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(bAutoHide);

	wchar_t *wBuffer = new wchar_t[128];
	GetPrivateProfileString(L"Config", L"strId", L"160047199", wBuffer, 128, L".\\Config.ini");
	this->strId = wBuffer;

	GetPrivateProfileString(L"Config", L"strPassword", L"MTIzNDU", wBuffer, 128, L".\\Config.ini");

	char *szBuffer = new char[128];
	strcpy_s(szBuffer, 128, CW2A(wBuffer).m_psz);
	char *szDecodedPassword = decodePassword(szBuffer);
	delete[]wBuffer;

	this->strPassword = CA2W(szDecodedPassword);
	delete[]szBuffer;

	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(this->strId.GetString());
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(this->strPassword.GetString());
}

// 保存配置(里面还会再调用加载配置)
void CGLCampusNetworkUnofficialClientDlg::SaveConfiguration() {

	this->bAutoRun = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	this->bAutoAuth = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	this->bAutoHide = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();

	WritePrivateProfileString(L"Config", L"bAutoRun", this->bAutoRun ? L"1" : L"0", L".\\Config.ini");
	WritePrivateProfileString(L"Config", L"bAutoAuth", this->bAutoAuth ? L"1" : L"0", L".\\Config.ini");
	WritePrivateProfileString(L"Config", L"bAutoHide", this->bAutoHide ? L"1" : L"0", L".\\Config.ini");

	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(this->strId);
	((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(this->strPassword);

	WritePrivateProfileString(L"Config", L"strId", this->strId, L".\\Config.ini");

	char *szBuffer = new char[128];
	char *szBufferEncoded = new char[512];
	strcpy_s(szBuffer, 128, CW2A(this->strPassword).m_psz);
	encodePassword((const unsigned char *)szBuffer, strlen(szBuffer), szBufferEncoded);
	WritePrivateProfileString(L"Config", L"strPassword", CA2W(szBufferEncoded).m_psz, L".\\Config.ini");
	
	delete[]szBuffer;
	delete[]szBufferEncoded;

	// 重加载配置，虽然可以省略，但当如果配置保存失败时可以只观看出来
	this->LoadConfiguration();
}

// 认证按钮被单击事件
void CGLCampusNetworkUnofficialClientDlg::OnBnClickedOk()
{
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(this->strId);
	((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(this->strPassword);
	if (this->strId.GetLength() == 0 || this->strPassword.GetLength() == 0) {
		MessageBox(L"请输入用户名和密码！", L"错误", 16);
		return;
	}

	this->SaveConfiguration();

	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	this->SetStatus(CString(L"开始认证..."));
	this->AddLog(CString(L"开始认证..."));

	unsigned int tid = 0;
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, CGLCampusNetworkUnofficialClientDlg::Login, this, 0, &tid);
	// 关闭线程句柄以防泄露，因为下面不需要再操作这个线程了
	::CloseHandle(hThread);
	
	// CDialogEx::OnOK();
}

// 认证上网
unsigned __stdcall CGLCampusNetworkUnofficialClientDlg::Login(void *pThis)
{
	CGLCampusNetworkUnofficialClientDlg *that = static_cast<CGLCampusNetworkUnofficialClientDlg *>(pThis);
	
	// 偷个懒
	#define ENABLE_BUTTON ((CButton*)that->GetDlgItem(IDOK))->EnableWindow(TRUE)

	that->AddLog(L"正在探测 Portal 服务器...");
	that->SetStatus(L"正在探测 Portal 服务器...");

	CCurlTask curl;
	curl.Exec("http://connect.rom.miui.com/generate_204"); // 只要是能生成204响应的服务都可以，这里用的小米，当然也可以自己搭建一个
	CString strHeader(curl.GetHeaderString());
	CString strMsg(curl.GetResultString());

	if (strMsg.Find(L"网络错误") != -1) { ENABLE_BUTTON; return 0; }
	if (strHeader.Find(L"204 No Content") != -1) {
		that->AddLog(L"网络可达, 无须认证");
		that->SetStatus(L"已连接上");
		ENABLE_BUTTON;
		that->SendTrayMessage(CString(L"网络可达, 无须认证"));
		if (that->bAutoHide) { that->ShowWindow(SW_HIDE); }
		return 0;
	}

	std::smatch matches;
	// 匹配 Location
	std::string header(CW2A(strHeader.GetString()));
	bool bValid = std::regex_search(header, matches, std::regex("[L|l]ocation:(.*)"));
	//for (size_t i = 0; i < matches.size(); i++) { MessageBoxA(that->m_hWnd, matches[i].str().c_str(), "Info", 64); }
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法探测 Portal 服务器!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	CStringA strPortalURL(matches[1].str().c_str());
	strPortalURL.Trim();
	that->AddLog(L"探测到 Portal 地址: " + CString(strPortalURL));
	if (strPortalURL.Find("://1.1.1.3") != -1) { that->AddLog(L"学校认证机器抽风, 需要等一会(约5-10分钟)..."); that->SetStatus(L"需要等待重试"); ENABLE_BUTTON; return 0; }
	
	that->SetStatus(L"匹配认证参数...");
	that->AddLog(L"匹配认证参数...");
	std::string FUCK("http://10.0.98.1/a79.htm?wlanuserip=10.2.67.115&wlanacname=ME60&wlanparameter=d0-57-7b-f1-1e-54");
	CStringA strUserIP(_GET(FUCK.c_str()/*matches[1].str()*/, "wlanuserip").c_str());
	CStringA strAcName(_GET(FUCK.c_str()/*matches[1].str()*/, "wlanacname").c_str());
	CStringA strUserMac(_GET(FUCK.c_str()/*matches[1].str()*/, "wlanparameter").c_str());

	if (strUserIP.IsEmpty() || strUserMac.IsEmpty()) { that->AddLog(L"无法获取用户网络信息!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	that->AddLog(L">> 用户IP: " + CString(strUserIP));
	that->AddLog(L">> 用户MAC: " + CString(strUserMac));
	that->AddLog(L">> 认证服务器名: " + CString(strAcName));

	CCurlTask getOptionsReq;
	getOptionsReq.Exec(strPortalURL);
	// 匹配 v4serip 也就是认证服务端IP
	std::string getOptionsRep("<script type='text/javascript'>sv=0;sv1=0;v6='http://[::]:9002/v6';myv6ip='';v4serip='10.0.98.1';m46=0;v46ip='10.201.229.2';vid=0;mip=010201229002;Gno=0000;vlanid='0';AC='';ipm='0a006201';ss1='0010f3644146';s");
	//std::string getOptionsRep(getOptionsReq.GetResultString().GetString());
	bValid = std::regex_search(getOptionsRep, matches, std::regex("v4serip='(.*?)'"));
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取认证服务器IP!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }
	
	CStringA strAuthIP(matches[1].str().c_str());
	that->AddLog(L">> 认证服务器IP: " + CString(strAuthIP));

	bValid = std::regex_search(getOptionsRep, matches, std::regex("vlanid='(.*?)'"));
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取VLAN ID!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	CStringA strVlanId(matches[1].str().c_str());
	that->AddLog(L">> 认证VLAN: " + CString(strVlanId));

	ENABLE_BUTTON;
	that->SendTrayMessage(CString(L"网络已经连接成功"));
	return 0;
}

// 加入日志到列表框
void CGLCampusNetworkUnofficialClientDlg::AddLog(CString strLog) {
	CTime tm;
	tm = CTime::GetCurrentTime();
	strLog.Insert(0, tm.Format("[%X]\x20"));
	this->listBox->InsertString(-1, strLog.GetString());
	int iCount = listBox->GetCount();
	if (iCount > 0) listBox->SetCurSel(iCount - 1);
}

// 设置状态信息
void CGLCampusNetworkUnofficialClientDlg::SetStatus(CString strLog) {
	strLog.Insert(0, L"状态: ");
	this->statusBox->SetWindowText(strLog);
}

// 退出按钮被单击
void CGLCampusNetworkUnofficialClientDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel(); // 其实只需要退出了
}

// 关于对话框中确定按钮被单击
void CAboutDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// 开机自动运行
void CGLCampusNetworkUnofficialClientDlg::OnIdcEdit1Click()
{
	// 选中开机时自动运行
	this->SaveConfiguration();
}

// 运行自动认证
void CGLCampusNetworkUnofficialClientDlg::OnIdcEdit2Click()
{
	this->SaveConfiguration();
}

// 认证自动隐藏
void CGLCampusNetworkUnofficialClientDlg::OnIdcEdit3Click()
{
	this->SaveConfiguration();
}

// 主窗口最大化最小化以及大小改变事件
void CGLCampusNetworkUnofficialClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED) {
		ShowWindow(SW_HIDE);
	} else if (nType == SIZE_RESTORED) {
	}
}

// 主窗口销毁时事件
void CGLCampusNetworkUnofficialClientDlg::OnDestroy()
{
	this->deleteTray();
	CDialog::OnDestroy();
}

// 增加托盘
void CGLCampusNetworkUnofficialClientDlg::toTray()
{
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(NOTIFYICONDATA));
	nid.cbSize = NOTIFYICONDATA_V2_SIZE; // FOR XP+ compatibility
	// nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); // FOR VISTA+
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.dwInfoFlags = NIIF_INFO;
	nid.uCallbackMessage = WM_SHOWTRAY;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, 128, L"广陵校园网客户端");
	Shell_NotifyIcon(NIM_ADD, &nid);
}

// 托盘事件
LRESULT CGLCampusNetworkUnofficialClientDlg::OnShowTray(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME) return 1;
	switch (lParam)
	{
		case WM_LBUTTONUP: // 左击
		{
			this->ShowWindow(SW_RESTORE);
			SetForegroundWindow();
		}
		break;
		case WM_RBUTTONUP: // 右击
		{
		}
		break;
		case WM_LBUTTONDBLCLK: // 双击
		{
			
		}
		break;
	}
	return 0;
}

// 销毁托盘
void CGLCampusNetworkUnofficialClientDlg::deleteTray()
{
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(NOTIFYICONDATA));
	nid.cbSize = NOTIFYICONDATA_V2_SIZE; // FOR XP+ compatibility
	// nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); // FOR VISTA+
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTRAY;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, 128, L"广陵校园网客户端");
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

// 发送托盘信息
void CGLCampusNetworkUnofficialClientDlg::SendTrayMessage(CString & strMsg)
{
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(NOTIFYICONDATA));
	nid.cbSize = NOTIFYICONDATA_V2_SIZE; // FOR XP+ compatibility
	// nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); // FOR VISTA+
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_INFO | NIF_MESSAGE | NIF_TIP;
	nid.dwInfoFlags = NIIF_INFO;
	nid.uCallbackMessage = WM_SHOWTRAY;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, 128, L"广陵校园网客户端");
	wcscpy_s(nid.szInfoTitle, 64, L"提示");				// 气泡标题
	wcscpy_s(nid.szInfo, 256, strMsg);					// 气泡内容
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}
