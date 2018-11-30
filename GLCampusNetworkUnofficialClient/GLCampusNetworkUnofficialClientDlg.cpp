/************************************/
/******  iEdon GLCNUC Project  ******/
/** A suck code for Campus Network **/
/************************************/
/************************************/

#include "stdafx.h"
#include "GLCampusNetworkUnofficialClient.h"
#include "GLCampusNetworkUnofficialClientDlg.h"
#include "afxdialogex.h"
#include "common.h"
#include "CCurlTask.h"

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
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CGLCampusNetworkUnofficialClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGLCampusNetworkUnofficialClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &CGLCampusNetworkUnofficialClientDlg::OnIdcEdit1Click)
	ON_BN_CLICKED(IDC_CHECK2, &CGLCampusNetworkUnofficialClientDlg::OnIdcEdit2Click)
	ON_BN_CLICKED(IDC_CHECK3, &CGLCampusNetworkUnofficialClientDlg::OnIdcEdit3Click)
	ON_BN_CLICKED(IDLOGOUT, &CGLCampusNetworkUnofficialClientDlg::OnBnClickedLogout)
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
}

// 认证上网
unsigned __stdcall CGLCampusNetworkUnofficialClientDlg::Login(void *pThis)
{
	CGLCampusNetworkUnofficialClientDlg *that = static_cast<CGLCampusNetworkUnofficialClientDlg *>(pThis);

	// 清空列表框
	that->listBox->ResetContent();

	// 偷个懒
	#define ENABLE_BUTTON ((CButton*)that->GetDlgItem(IDOK))->EnableWindow(TRUE); ((CButton*)that->GetDlgItem(IDOK))->ShowWindow(TRUE); if (that->hShutdownLogoutThread) { ::CloseHandle(that->hShutdownLogoutThread); that->hShutdownLogoutThread = NULL; }
	#define BUTTON_ONLINE ((CButton*)that->GetDlgItem(IDOK))->EnableWindow(TRUE); ((CButton*)that->GetDlgItem(IDOK))->ShowWindow(FALSE); ((CButton*)that->GetDlgItem(IDLOGOUT))->ShowWindow(TRUE); ((CEdit*)that->GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE); ((CEdit*)that->GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE); if (that->hShutdownLogoutThread) { ::CloseHandle(that->hShutdownLogoutThread); that->hShutdownLogoutThread = NULL; }
	
	that->AddLog(L"正在探测 Portal 服务器...");
	that->SetStatus(L"正在探测 Portal 服务器...");

	CCurlTask curl;
	curl.Exec("http://connect.rom.miui.com/generate_204"); // 只要是能生成204响应的服务都可以，这里用的小米，当然也可以自己搭建一个
	CString strHeader(curl.GetHeaderString());
	CString strMsg(curl.GetResultString());

	if (strMsg.Find(L"网络错误") != -1) { that->AddLog(strMsg); that->SetStatus(L"网络错误"); ENABLE_BUTTON; return 0; }
	if (strHeader.Find(L"204 No Content") != -1) {
		that->AddLog(L"网络可达, 无须认证");
		that->SetStatus(L"已连接上");
		ENABLE_BUTTON;
		that->SendTrayMessage(CString(L"此网络已连接, 无须认证"), L"网络已连接");
		if (that->bAutoHide) { that->ShowWindow(SW_HIDE); }
		return 0;
	}

	std::smatch matches;
	// 匹配 Location
	std::string header(CW2A(strHeader.GetString()));
	bool bValid = std::regex_search(header, matches, std::regex("[L|l]ocation:(.*)"));
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法探测 Portal 服务器!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	CStringA strPortalURL(matches[1].str().c_str());
	strPortalURL.Trim();
	that->AddLog(L"探测到 Portal 地址: " + CString(strPortalURL));
	if (strPortalURL.Find("://1.1.1.3") != -1) { that->AddLog(L"学校认证机器抽风, 需要等一会(约5-10分钟)..."); that->SetStatus(L"需要等待重试"); ENABLE_BUTTON; return 0; }

	that->SetStatus(L"匹配认证参数...");
	that->AddLog(L"匹配认证参数...");

	#ifdef __IEDON_DEBUG__
		const char *FUCK1 = "http://http://10.0.98.1/a79.htm?wlanuserip=10.2.0.187&wlanacname=ME60&wlanparameter=9c-ad-97-d1-51-11";
		CStringA strUserIP(_GET(/**/FUCK1/*matches[1].str()*/, "wlanuserip").c_str());
		CStringA strAcName(_GET(/**/FUCK1/*matches[1].str()*/, "wlanacname").c_str());
		CStringA strUserMac(_GET(/**/FUCK1/*matches[1].str()*/, "wlanparameter").c_str());
	#else
		CStringA strUserIP(_GET(matches[1].str(), "wlanuserip").c_str());
		CStringA strAcName(_GET(matches[1].str(), "wlanacname").c_str());
		CStringA strUserMac(_GET(matches[1].str(), "wlanparameter").c_str());
	#endif

	if (strUserIP.IsEmpty() || strUserMac.IsEmpty()) { that->AddLog(L"无法获取用户网络信息!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	that->AddLog(L">> 用户IP: " + CString(strUserIP));
	that->AddLog(L">> 用户MAC: " + CString(strUserMac));
	that->AddLog(L">> 接入控制器: " + CString(strAcName));

	CCurlTask getOptionsReq;
	getOptionsReq.Exec(strPortalURL);

	// 匹配 v4serip 也就是认证服务端IP
	#ifdef __IEDON_DEBUG__
		std::string getOptionsRep(FUCK);
	#else
		std::string getOptionsRep(getOptionsReq.GetResultString().GetString());
	#endif

	bValid = std::regex_search(getOptionsRep, matches, std::regex("v4serip=['|\"](.*?)['|\"]"));
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取入口服务器IP!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	CStringA strPortalIP(matches[1].str().c_str());
	that->AddLog(L">> 入口服务器: " + CString(strPortalIP));

	bValid = std::regex_search(getOptionsRep, matches, std::regex("vlanid=['|\"](.*?)['|\"]"));
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取VLAN ID!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	CStringA strVlanId(matches[1].str().c_str());
	that->AddLog(L">> 认证VLAN: " + CString(strVlanId));

	bValid = std::regex_search(getOptionsRep, matches, std::regex("authloginIP=['|\"](.*?)['|\"]"));
	CStringA strAuthBackendServer, strAuthBackendIP;
	if (bValid && matches.size() == 2 && matches[1].str().length()) { strAuthBackendServer.Append(matches[1].str().c_str()); } else { strAuthBackendServer.Append(strPortalIP); }
	strAuthBackendIP.Append(strAuthBackendServer);

	bValid = std::regex_search(getOptionsRep, matches, std::regex("authloginport=(.*?);"));
	CStringA strAuthLogoutPort; // 为什么是 Logout 呢？因为 authloginport = login + logout's port 鬼畜的Dr.COM。原因见下文
	if (bValid && matches.size() == 2) {
		strAuthBackendServer.Append(":");
		strAuthBackendServer.Append(matches[1].str().c_str()); 
		strAuthLogoutPort.Append(":");
		strAuthLogoutPort.Append(matches[1].str().c_str());
	} else {
		strAuthBackendServer.Append(":80");
		strAuthLogoutPort.Append(":80");
	}
	
	that->AddLog(L">> 认证服务器: " + CString(strAuthBackendServer));

	bValid = std::regex_search(getOptionsRep, matches, std::regex("authloginpath=['|\"](.*?)['|\"]"));
	CStringA strAuthURL("http://");
	strAuthURL.Append(strAuthBackendServer);
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取验证路径"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }
	strAuthURL.Append(matches[1].str().c_str());

	bValid = std::regex_search(getOptionsRep, matches, std::regex("authloginparam=['|\"](.*?)['|\"]"));
	if (bValid && matches.size() == 2 && matches[1].str().length()) { strAuthURL.Append("&"); strAuthURL.Append(matches[1].str().c_str()); }


	bValid = std::regex_search(getOptionsRep, matches, std::regex("authuserfield=['|\"](.*?)['|\"]"));
	CStringA strAuthUserNameHtmlField;
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取用户名域"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }
	strAuthUserNameHtmlField.Append(matches[1].str().c_str());

	bValid = std::regex_search(getOptionsRep, matches, std::regex("authpassfield=['|\"](.*?)['|\"]"));
	CStringA strAuthPasswordHtmlField;
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取密码域"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }
	strAuthPasswordHtmlField.Append(matches[1].str().c_str());

	bValid = std::regex_search(getOptionsRep, matches, std::regex("\/eportal\/extern\/(.*?)\/"));
	CStringA strProgramId;
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法获取认证版本"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }
	strProgramId.Append(matches[1].str().c_str());
	that->AddLog(L">> 认证版本: " + CString(strProgramId));
	that->AddLog(L">> 认证字段: " + CString(strAuthUserNameHtmlField) + L", " + CString(strAuthPasswordHtmlField));

	///////// 下面是获取断开连接所需要的信息
	bValid = std::regex_search(getOptionsRep, matches, std::regex("authlogoutIP=['|\"](.*?)['|\"]"));
	CStringA strAuthBackendServerLogout, strAuthBackendIPLogout;
	if (bValid && matches.size() == 2 && matches[1].str().length()) { strAuthBackendServerLogout.Append(matches[1].str().c_str()); strAuthBackendIPLogout.Append(matches[1].str().c_str()); } else { strAuthBackendServerLogout.Append(strPortalIP); strAuthBackendIPLogout.Append(strPortalIP); }

	/* 虽然 DR.COM 提供了额 authlogoutport，然而却不用！！！！端口等同于 authloginport  神TMD的鬼畜逻辑
		bValid = std::regex_search(getOptionsRep, matches, std::regex("authlogoutport=(.*?);"));
		if (bValid && matches.size() == 2) { strAuthBackendServerLogout.Append(":"); strAuthBackendServerLogout.Append(matches[1].str().c_str()); } else { strAuthBackendServerLogout.Append(":80"); }
	*/
	strAuthBackendServerLogout.Append(strAuthLogoutPort); // Dr.COM 式鬼畜
	bValid = std::regex_search(getOptionsRep, matches, std::regex("authlogoutpath=['|\"](.*?)['|\"]"));
	CStringA strAuthURLLogout("http://");
	strAuthURLLogout.Append(strAuthBackendServerLogout);
	strAuthURLLogout.Append(matches[1].str().c_str());

	bValid = std::regex_search(getOptionsRep, matches, std::regex("authlogoutparam=['|\"](.*?)['|\"]"));
	if (bValid && matches.size() == 2 && matches[1].str().length()) { strAuthURLLogout.Append("&"); strAuthURLLogout.Append(matches[1].str().c_str()); }

	///////////////////////////

	// iTermType=1 代表平台为PC，可在 a41 等找到
	// protocol 协议前缀
	// hostname 认证服务器主机名
	// wlanuserip/ip 终端IP
	// wlanacip ACIP
	// wlanacname AC名
	that->AddLog(L"构建登录请求...");
	
	strAuthURL.Append("&protocol=http:&hostname=" + strAuthBackendIP + "&iTermType=1&wlanuserip=" + strUserIP + "&wlanacip=null&wlanacname=" + strAcName + "&mac=00-00-00-00-00-00&ip=" + strUserIP + "&enAdvert=0&queryACIP=0&loginMethod=1");
	CStringA strLogoutMac(strUserMac); strLogoutMac.Replace("-", "");
	that->strLogoutURL = strAuthURLLogout + "&wlanuserip=" + strUserIP + "&wlanacname=" + strAcName + "&port=&hostname=" + strAuthBackendIPLogout + "&iTermType=1&session=&queryACIP=0&mac=" + strLogoutMac + "&wlanacip=";
	
	CCurlTask loginReq;
	CStringA postData, postCookie("program=");
	postData.Append(strAuthUserNameHtmlField + "=%2C0%2C" + CStringA(that->strId) + "&" + // 用户名
		strAuthPasswordHtmlField + "=" + CStringA(that->strPassword) + // 密码
		"&R1=0&R2=0&R3=0&R6=0&para=00&0MKKey=123456&buttonClicked=&redirect_url=&err_flag=&username=&password="); // 杂七杂八
	postCookie.Append(strProgramId + "; vlan=" + strVlanId + "; ip=" + strUserIP + "; ssid=null; areaID=null; md5_login2=%2C0%2C" + CStringA(that->strId) + "%7C" + CStringA(that->strPassword));
	that->strCookie = postCookie;

	that->AddLog(L"发送用户名与密码...");
	loginReq.Exec(strAuthURL, false, postCookie, true, postData);

	std::string strLoginRep(loginReq.GetHeaderString().GetString());
	bValid = std::regex_search(strLoginRep, matches, std::regex("[L|l]ocation:(.*)"));
	if (bValid && matches.size() == 2 && matches[1].str().length()) { that->strReferer = CString(matches[1].str().c_str()); that->strReferer.Trim(); }

	CString strLoginMsg(loginReq.GetResultString());
	std::string strLoginHeader(loginReq.GetHeaderString().GetString());

	if (strLoginMsg.Find(L"网络错误") != -1) { that->AddLog(strLoginMsg); that->SetStatus(L"网络错误"); ENABLE_BUTTON; return 0; }
	bValid = std::regex_search(strLoginHeader, matches, std::regex("[L|l]ocation:(.*)"));
	if (!bValid || matches.size() != 2) { that->AddLog(L"无法得到认证结果!"); that->SetStatus(L"发生错误"); ENABLE_BUTTON; return 0; }

	CStringA strAcMessage(_GET(strLoginHeader.c_str(), "ACLogOut").c_str());
	that->strWlanAcIp = _GET(strLoginHeader.c_str(), "wlanacip").c_str();
	that->strSessionId = _COOKIE(strLoginHeader.c_str(), "PHPSESSID").c_str();
	CStringA strRadiusMessage(_GET(strLoginHeader.c_str(), "ErrorMsg").c_str());

	// 粗略“URL解码”
	strRadiusMessage.Replace("%3D", "="); strRadiusMessage.Replace("%3d", "=");
	strRadiusMessage.Replace("%2B", "+"); strRadiusMessage.Replace("%2b", "+");
	strRadiusMessage.Replace("%2F", "/"); strRadiusMessage.Replace("%2f", "/");

	if (strAcMessage.GetLength() == 0 || strAcMessage == "0" /* 二维码扫描终端认证成功 */ || strAcMessage == "1" /* 注销成功 */) {

		strAcMessage = "登录成功!";
		that->AddLog(CString(strAcMessage));
		that->SetStatus(L"已连接上");
		if (that->bAutoHide) { that->ShowWindow(SW_HIDE); }

	} else if (strAcMessage != "5") {

		if (strAcMessage == "2") { strAcMessage = "注销失败"; }
		if (strAcMessage == "3") { strAcMessage = "访问开户接口失败"; }
		if (strAcMessage == "4") { strAcMessage = "二维码扫描终端认证失败"; }
		that->AddLog(CString(strAcMessage));
		that->SetStatus(L"认证失败");
		ENABLE_BUTTON;
		that->SendTrayMessage(CString(strAcMessage), L"认证失败");
		return 0;

	} else {

		size_t bufferLength = strRadiusMessage.GetLength() + 1;
		char *szRadiusMessageBuffer = new char[bufferLength];
		memset(szRadiusMessageBuffer, 0, bufferLength);
		
		base64_decode(strRadiusMessage.GetString(), (unsigned char *)szRadiusMessageBuffer, &bufferLength);
		strRadiusMessage = szRadiusMessageBuffer;

		delete[]szRadiusMessageBuffer;
		
		// 继续偷懒
		#define AUTH_FAILED_RADIUS that->AddLog(CString(strRadiusMessage)); that->SetStatus(L"认证失败"); ENABLE_BUTTON; that->SendTrayMessage(CString(strRadiusMessage), L"认证失败"); return 0
		//////////////////////////////////////////  AC给出错误的形式一 ////////////////////////////////////////////
		
		if (strRadiusMessage == "2") { strRadiusMessage = "终端IP已经在线"; AUTH_FAILED_RADIUS; }
		if (strRadiusMessage == "3") { strRadiusMessage = "系统繁忙，请稍后再试"; AUTH_FAILED_RADIUS; }
		if (strRadiusMessage == "4") { strRadiusMessage = "发生未知错误，请稍后再试"; AUTH_FAILED_RADIUS;}
		if (strRadiusMessage == "5") { strRadiusMessage = "REQ_CHALLENGE失败，请联系AC确认"; AUTH_FAILED_RADIUS; }
		if (strRadiusMessage == "6") { strRadiusMessage = "REQ_CHALLENGE超时，请联系AC确认"; AUTH_FAILED_RADIUS; }
		if (strRadiusMessage == "7") { strRadiusMessage = "Radius认证失败"; AUTH_FAILED_RADIUS;}
		if (strRadiusMessage == "8") { strRadiusMessage = "Radius认证超时"; AUTH_FAILED_RADIUS;}
		if (strRadiusMessage == "9") { strRadiusMessage = "Radius下线失败"; AUTH_FAILED_RADIUS;}
		if (strRadiusMessage == "10") { strRadiusMessage = "Radius下线超时"; AUTH_FAILED_RADIUS;}
		if (strRadiusMessage == "11") { strRadiusMessage = "发生其他错误，请稍后再试"; AUTH_FAILED_RADIUS;}
		if (strRadiusMessage == "998") { strRadiusMessage = "Portal协议参数不全，请稍后再试"; AUTH_FAILED_RADIUS;}

		if (strRadiusMessage == "1" || strRadiusMessage == "512") { // 这两种情况，需要客户端去请求Radius，获得更详细的错误原因

			that->AddLog(L"认证失败 (Radius 审核失败)");
			that->AddLog(L">> 正在获取具体错误原因...");

			CCurlTask getErrorDescReq;
			getErrorDescReq.Exec("http://" + strPortalIP + "/errcode", false, postCookie);
			CString strErrorMsg(getErrorDescReq.GetResultString());
			if (strErrorMsg.Find(L"网络错误") != -1) { that->AddLog(strErrorMsg); that->SetStatus(L"网络错误"); ENABLE_BUTTON; return 0; }
			std::string strErrorHtml = getErrorDescReq.GetResultString().GetString();

			CStringA strErrDesc("未知的认证错误");
			bValid = std::regex_search(strErrorHtml, matches, std::regex("Rpost=2;ret='(.*?)'"));
			if (bValid && matches.size() == 2) { 
				strErrDesc.Append(matches[1].str().c_str());
				if (strErrDesc == "no errcode") { strErrDesc = "AC认证失败"; }
				if (strErrDesc == "") { strErrDesc = "SESSION已过期,请重新登录"; }
				if (strErrDesc == "Authentication Fail ErrCode=04") { strErrDesc = "上网时长/流量已到上限"; }
				if (strErrDesc == "Authentication Fail ErrCode=05") { strErrDesc = "帐号已停机(欠费/报停)"; }
			} else {
				if (strErrorHtml.find("userid error1") != std::string::npos) { strErrDesc = "帐号不存在";  }
				if (strErrorHtml.find("userid error2") != std::string::npos) { strErrDesc = "密码错误"; }
				if (strErrorHtml.find("userid error3") != std::string::npos) { strErrDesc = "密码错误"; }
				if (strErrorHtml.find("auth error") != std::string::npos) { strErrDesc = "用户验证失败"; }
				if (strErrorHtml.find("auth error4") != std::string::npos) { strErrDesc = "用户使用量超出限制"; }
				if (strErrorHtml.find("auth error5") != std::string::npos) { strErrDesc = "帐号已停机"; }
				if (strErrorHtml.find("auth error9") != std::string::npos) { strErrDesc = "时长流量超支"; }
				if (strErrorHtml.find("auth error80") != std::string::npos) { strErrDesc = "本时段禁止上网"; }
				if (strErrorHtml.find("auth error99") != std::string::npos) { strErrDesc = "用户名或密码错误"; }
				if (strErrorHtml.find("auth error198") != std::string::npos) { strErrDesc = "用户名或密码错误"; }
				if (strErrorHtml.find("auth error199") != std::string::npos) { strErrDesc = "用户名或密码错误"; }
				if (strErrorHtml.find("auth error258") != std::string::npos) { strErrDesc = "帐号只能在指定区域使用"; }
				if (strErrorHtml.find("set_onlinet error") != std::string::npos) { strErrDesc = "用户数超过限制"; }
				if (strErrorHtml.find("In use") != std::string::npos) { strErrDesc = "登录超过人数限制"; }
				if (strErrorHtml.find("port err") != std::string::npos) { strErrDesc = "上课时间不允许上网"; }
				if (strErrorHtml.find("can not use static ip") != std::string::npos) { strErrDesc = "不允许使用静态IP"; }
				if (strErrorHtml.find("set_onlinet error") != std::string::npos) { strErrDesc = "用户数超过限制"; }
				if (strErrorHtml.find("err(3)") != std::string::npos) { strErrDesc = "请在指定的IP登录"; }
				if (strErrorHtml.find("err(7)") != std::string::npos) { strErrDesc = "请在指定的登录源VLAN范围登录"; }
				if (strErrorHtml.find("err(10)") != std::string::npos) { strErrDesc = "请在指定的VLAN登录"; }
				if (strErrorHtml.find("err(11)") != std::string::npos) { strErrDesc = "请在指定的MAC登录"; }
				if (strErrorHtml.find("err(17)") != std::string::npos) { strErrDesc = "请在指定的设备端口登录"; }
			}

			that->AddLog(CString(strErrDesc));
			that->SetStatus(L"认证失败");
			ENABLE_BUTTON;
			that->SendTrayMessage(CString(strErrDesc), L"认证失败");
			return 0;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////  AC给出错误的形式二 ////////////////////////////////////////////
		
		if (strRadiusMessage == "no errcode") { strRadiusMessage = "AC认证失败"; }
		if (strRadiusMessage == "") { strRadiusMessage = "SESSION已过期,请重新登录"; }
		if (strRadiusMessage == "Authentication Fail ErrCode=04") { strRadiusMessage = "上网时长/流量已到上限"; }
		if (strRadiusMessage == "Authentication Fail ErrCode=05") { strRadiusMessage = "帐号已停机(欠费/报停)"; }
		if (strRadiusMessage == "userid error1") { strRadiusMessage = "帐号不存在"; }
		if (strRadiusMessage == "userid error2") { strRadiusMessage = "密码错误"; }
		if (strRadiusMessage == "userid error3") { strRadiusMessage = "密码错误"; }
		if (strRadiusMessage == "auth error") { strRadiusMessage = "用户验证失败"; }
		if (strRadiusMessage == "auth error4") { strRadiusMessage = "用户使用量超出限制"; }
		if (strRadiusMessage == "auth error5") { strRadiusMessage = "帐号已停机"; }
		if (strRadiusMessage == "auth error9") { strRadiusMessage = "时长流量超支"; }
		if (strRadiusMessage == "auth error80") { strRadiusMessage = "本时段禁止上网"; }
		if (strRadiusMessage == "auth error99") { strRadiusMessage = "用户名或密码错误"; }
		if (strRadiusMessage == "auth error198") { strRadiusMessage = "用户名或密码错误"; }
		if (strRadiusMessage == "auth error199") { strRadiusMessage = "用户名或密码错误"; }
		if (strRadiusMessage == "auth error258") { strRadiusMessage = "帐号只能在指定区域使用"; }
		if (strRadiusMessage == "set_onlinet error") { strRadiusMessage = "用户数超过限制"; }
		if (strRadiusMessage == "In use") { strRadiusMessage = "登录超过人数限制"; }
		if (strRadiusMessage == "port err") { strRadiusMessage = "上课时间不允许上网"; }
		if (strRadiusMessage == "can not use static ip") { strRadiusMessage = "不允许使用静态IP"; }
		if (strRadiusMessage == "set_onlinet error") { strRadiusMessage = "用户数超过限制"; }
		if (strRadiusMessage == "err(3)") { strRadiusMessage = "请在指定的IP登录"; }
		if (strRadiusMessage == "err(7)") { strRadiusMessage = "请在指定的登录源VLAN范围登录"; }
		if (strRadiusMessage == "err(10)") { strRadiusMessage = "请在指定的VLAN登录"; }
		if (strRadiusMessage == "err(11)") { strRadiusMessage = "请在指定的MAC登录"; }
		if (strRadiusMessage == "err(17)") { strRadiusMessage = "请在指定的设备端口登录"; }

		AUTH_FAILED_RADIUS;
	}

	BUTTON_ONLINE;
	that->SendTrayMessage(CString(L"当前IP: " + CString(strUserIP)), L"校园网 现在已连接");
	return 0;
}

// 断开上网
unsigned __stdcall CGLCampusNetworkUnofficialClientDlg::Logout(void *pThis)
{
	CGLCampusNetworkUnofficialClientDlg *that = static_cast<CGLCampusNetworkUnofficialClientDlg *>(pThis);
	
	// 套用登出URL
	CStringA strLogoutURL(that->strLogoutURL);
	// 补全 WLAN AC IP
	strLogoutURL.Append(CStringA(that->strWlanAcIp));

	CStringA strCookie(that->strCookie);
	CStringA strPhpSessId("PHPSESSID=");
	strPhpSessId.Append(that->strSessionId + ";\x20");
	strCookie.Insert(0, strPhpSessId);

	// 发送断线请求
	CCurlTask req;
	req.SetReferer(CStringA(that->strReferer));
	req.Exec(strLogoutURL, false, strCookie, true, "");

	int iPos = req.GetResultString().Find("Logout succeed.");
	if (iPos != -1)
	{
		that->AddLog(L"断开成功, 已经成功下线");
		that->SetStatus(L"未连接");
	} else {
		that->AddLog(L"下线失败...");
		that->SetStatus(L"未连接");
	}

	if (that->hShutdownLogoutThread) { ::CloseHandle(that->hShutdownLogoutThread); that->hShutdownLogoutThread = NULL; }
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
	CString strBuffer;
	this->statusBox->GetWindowText(strBuffer);
	if (strBuffer == L"状态: 已连接上" && this->IsWindowVisible()) {
		if (::MessageBox(this->m_hWnd, L"已经连上网络。\r\n是否仍然需要退出？退出后网络将会断开。", L"退出确认", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
		{
			CDialogEx::OnCancel(); // 关闭对话框(取消)
		}
	} else {
		CDialogEx::OnCancel(); // 关闭对话框(取消)
	}
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

// 主窗口关闭事件
void CGLCampusNetworkUnofficialClientDlg::OnClose()
{
	CString strBuffer;
	this->statusBox->GetWindowText(strBuffer);
	if (strBuffer == L"状态: 已连接上" && this->IsWindowVisible()) {
		this->ShowWindow(SW_HIDE);
		return;
	}
	if (strBuffer == L"状态: 已连接上") {
		if (::MessageBox(this->m_hWnd, L"已经连上网络。\r\n是否仍然需要退出？退出后网络将会断开。", L"退出确认", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
		{
			CDialog::OnClose();
		}
	} else {
	
		CDialog::OnClose();
	}
}

// 主窗口销毁时事件
void CGLCampusNetworkUnofficialClientDlg::OnDestroy()
{
	this->deleteTray();
	CString strBuffer;
	this->statusBox->GetWindowText(strBuffer);
	if (strBuffer == L"状态: 已连接上") {
		this->OnBnClickedLogout();
	}
	if (strBuffer == L"状态: 已连接上" || this->hShutdownLogoutThread) {
		this->ShowWindow(SW_HIDE);
		::WaitForSingleObject(this->hShutdownLogoutThread, 5000); // 退出时，先隐藏界面，然后等待断线线程执行，如果线程超时，直接退出
	}
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
		case WM_LBUTTONUP: case WM_LBUTTONDBLCLK: // 左击和双击
		{
			this->ShowWindow(SW_RESTORE);
			SetForegroundWindow();
		}
		break;
		case WM_RBUTTONUP: // 右击
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MFT_STRING, IDR_SHOW, L"显示主界面...(&S)");
			CString strAboutDialogStr;
			BOOL bNameValid = strAboutDialogStr.LoadString(IDS_ABOUTBOX);
			ASSERT(bNameValid);
			menu.AppendMenu(MFT_STRING, IDM_ABOUTBOX, strAboutDialogStr);
			menu.AppendMenu(MFT_SEPARATOR);
			menu.AppendMenu(MFT_STRING, IDR_EXIT, L"退出...(&E)");
			int iOptId = ::TrackPopupMenu(menu, TPM_RETURNCMD, lpoint->x, lpoint->y, NULL, this->m_hWnd, NULL);
			switch (iOptId) {
				case IDR_SHOW: this->ShowWindow(SW_RESTORE); this->SetForegroundWindow(); break;
				case IDM_ABOUTBOX: {
					CAboutDlg dlgAbout;
					dlgAbout.DoModal();
					break;
				}
				case IDR_EXIT: this->ShowWindow(SW_HIDE); this->SendMessage(WM_CLOSE); break;
				default: break;
			}
			HMENU hMenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
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
void CGLCampusNetworkUnofficialClientDlg::SendTrayMessage(CString & strMsg, CString strTitle)
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
	wcscpy_s(nid.szInfoTitle, 64, strTitle);			// 气泡标题
	wcscpy_s(nid.szInfo, 256, strMsg);					// 气泡内容
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// 断开按钮被单击事件
void CGLCampusNetworkUnofficialClientDlg::OnBnClickedLogout()
{
	((CEdit*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOK))->ShowWindow(TRUE);
	((CButton*)GetDlgItem(IDLOGOUT))->ShowWindow(FALSE);

	this->SetStatus(CString(L"正在断开..."));
	this->AddLog(CString(L"正在断开..."));

	unsigned int tid = 0;
	if (this->hShutdownLogoutThread) ::CloseHandle(this->hShutdownLogoutThread);
	this->hShutdownLogoutThread = (HANDLE)::_beginthreadex(NULL, 0, CGLCampusNetworkUnofficialClientDlg::Logout, this, 0, &tid);
}
