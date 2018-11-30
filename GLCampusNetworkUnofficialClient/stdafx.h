
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <iostream>
#include <string>
#include <regex>

// 调试开关 注释掉即为发布版
// #define __IEDON_DEBUG__  


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


extern "C" {
	#define CURL_STATICLIB
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "wldap32.lib")
	#pragma comment(lib, "crypt32.lib")
	#ifdef WIN32 // Windows 平台
		#include "libcurl/curl.h"
	#else // Unix Like or other platforms
		#include <curl/curl.h>
		#include <iconv.h>
		#include <unistd.h>
	#endif
	#ifdef _WIN64 // Windows amd64(x86-64) 平台
		#pragma comment(lib, "libcurl/zlib_a_x64.lib")
		#pragma comment(lib, "libcurl/libcurl_x64.lib")
	#else
		#ifdef _WIN32 // Windows x86 平台
			#pragma comment(lib, "libcurl/zlib_a_x86.lib")
			#pragma comment(lib, "libcurl/libcurl_x86.lib")
		#endif
	#endif
}
