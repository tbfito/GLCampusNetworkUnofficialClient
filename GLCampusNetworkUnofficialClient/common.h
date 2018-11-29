#pragma once
char * encodePassword(const unsigned char * src, int srcLen, char * out);
char * decodePassword(char *szPassword);
std::string _GET(const std::string & get, const char *name);
std::string _URLFIND(const std::string & url, const char *name);
