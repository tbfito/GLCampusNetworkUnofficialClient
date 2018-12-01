#include "stdafx.h"
#include "CCurlTask.h"

CCurlTask::CCurlTask()
{
	curl = curl_easy_init();
}

CCurlTask::~CCurlTask()
{
	if (curl != NULL)
	{
		curl_easy_cleanup(curl);
		curl = NULL;
	}
}

char * CCurlTask::GetResult()
{
	return (char *)result.GetString();
}

size_t CCurlTask::GetLength()
{
	return result.GetLength();
}

bool CCurlTask::SetReferer(CStringA & referer)
{
	return (curl_easy_setopt(curl, CURLOPT_REFERER, referer.GetString()) == CURLE_OK);
}

CStringA & CCurlTask::GetResultString()
{
	return result;
}

CStringA & CCurlTask::GetHeaderString()
{
	return header;
}

bool CCurlTask::Exec(CStringA url, bool headonly, CStringA cookie, bool isPOST, CStringA postdata)
{
	if (curl == NULL)
	{
		return false;
	}
	header.Empty();
	result.Empty();
	if (headonly)
	{
		curl_easy_setopt(curl, CURLOPT_HEADER, true);
		curl_easy_setopt(curl, CURLOPT_NOBODY, true);
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	if (isPOST)
	{
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.GetString());
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->result);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &this->header);
	if (!cookie.IsEmpty())
	{
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.GetString());
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->curl_receive);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, this->curl_header);
	curl_easy_setopt(curl, CURLOPT_URL, url.GetString());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "iEdon-GLCNUC/1.0");
	curl_easy_setopt(curl, CURLOPT_CRLF, 1L);
	struct curl_slist *list = NULL;
	list = curl_slist_append(list, "Pragma: no-cache");
	list = curl_slist_append(list, "Cache-Control: no-cache");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

	CURLcode ret = curl_easy_perform(curl);
	curl_slist_free_all(list);
	
	if (ret != CURLE_OK)
	{
		this->result.Empty();
		this->result.Format("网络错误: %s", curl_easy_strerror(ret));
		return false;
	}
	return true;
}

size_t CCurlTask::curl_receive(char *buffer, size_t size, size_t nmemb, CStringA *stringclass)
{
	size_t block_size = size * nmemb;
	stringclass->Append(buffer, block_size);
	return block_size;
}

size_t CCurlTask::curl_header(char *buffer, size_t size, size_t nitems, CStringA *stringclass)
{
	size_t block_size = size * nitems;
	stringclass->Append(buffer, block_size);
	return block_size;
}
