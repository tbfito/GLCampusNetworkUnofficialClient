#include "stdafx.h"
#include "GLCNUC.h"
#include "GLCNUCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGLCNUCApp
BEGIN_MESSAGE_MAP(CGLCNUCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CGLCNUCApp 构造
CGLCNUCApp::CGLCNUCApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// 在此处添加构造代码，将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CGLCNUCApp 对象
CGLCNUCApp theApp;

// CGLCNUCApp 初始化
BOOL CGLCNUCApp::InitInstance()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("__IEDON_GLCNUC_MUTEX__"));
	if ((NULL != hMutex) && (ERROR_ALREADY_EXISTS == GetLastError()))
	{
		HWND hWnd = ::FindWindow(NULL, _T("广陵校园网客户端(非官方)"));
		if (NULL != hWnd)
		{
			// 显示窗口
			::ShowWindow(hWnd, SW_RESTORE);
			::SetForegroundWindow(hWnd);
			// 窗口抖动
			const int ty = 5;
			CRect m_rect;
			CRect m_DeskRet;
			::GetWindowRect(::GetDesktopWindow(), &m_DeskRet);
			::GetWindowRect(hWnd, &m_rect);
			int iXpos = m_DeskRet.Width() / 2 - m_rect.Width() / 2;
			int iYpos = m_DeskRet.Height() / 2 - m_rect.Height() / 2;
			::SetWindowPos(hWnd, NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
			::GetWindowRect(hWnd, &m_rect);
			LONG recordy = m_rect.left;
			LONG recordx = m_rect.top;
			for (int i = 0; i < 3; i++)
			{
				m_rect.left = recordy;
				m_rect.top = recordx;
				m_rect.top = m_rect.top + ty;
				m_rect.left = m_rect.left - ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE); ::Sleep(25);
				m_rect.top = m_rect.top - ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE); ::Sleep(25);
				m_rect.top = m_rect.top - 2 * ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE); ::Sleep(25);
				m_rect.left = m_rect.left + ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE); ::Sleep(25);
				m_rect.left = m_rect.left + 2 * ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE); ::Sleep(25);
				m_rect.top = m_rect.top + ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE); ::Sleep(25);
				m_rect.top = m_rect.top + 2 * ty;
				::SetWindowPos(hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE);
				::SetWindowPos(hWnd, NULL, recordy, recordx, 0, 0, SWP_NOSIZE);
				::Sleep(25);
			}
		}
		CloseHandle(hMutex);
		return FALSE;
	}

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 创建 shell 管理器，以防对话框包含任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	CGLCNUCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，而不是启动应用程序的消息泵。
	return FALSE;
}
