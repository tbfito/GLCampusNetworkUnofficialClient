#pragma once
#include "stdafx.h"

#ifndef __CCURLTASK_H__

	#define __CCURLTASK_H__

	class CCurlTask {
	public:
		CCurlTask();
		~CCurlTask();
		bool Exec(CStringA url, bool headonly = false, CStringA cookie = "", bool isPOST = false, CStringA postdata = "");
		bool SetReferer(CStringA & referer);
		char *GetResult();
		CStringA & GetResultString();
		CStringA & GetHeaderString();
		size_t GetLength();
	
	private:
		static size_t curl_receive(char *buffer, size_t size, size_t nmemb, CStringA *stringclass);
		static size_t curl_header(char *buffer, size_t size, size_t nitems, CStringA *stringclass);
		CURL* curl;
		CStringA result;
		CStringA header;
	};

#endif