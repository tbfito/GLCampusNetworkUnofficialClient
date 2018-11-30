#pragma once
char * encodePassword(const unsigned char * src, int srcLen, char * out);
char * decodePassword(char *szPassword);
bool base64_decode(const char *strIn, unsigned char *pOut, size_t *uOutLen);
std::string _GET(const std::string & get, const char *name);
std::string _URLFIND(const std::string & url, const char *name);
std::string _COOKIE(const std::string & cookie, const char *name);
#ifdef __IEDON_DEBUG__
	extern const char *FUCK;
#endif