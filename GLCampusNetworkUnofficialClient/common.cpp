#include "stdafx.h"
#include "common.h"
/*
	BASE64 编码
*/
static const char *base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char * base64_encode(const unsigned char * bindata, char * base64, int binlength)
{
	int i, j;
	unsigned char current;

	for (i = 0, j = 0; i < binlength; i += 3)
	{
		current = (bindata[i] >> 2);
		current &= (unsigned char)0x3F;
		base64[j++] = base64char[(int)current];

		current = ((unsigned char)(bindata[i] << 4)) & ((unsigned char)0x30);
		if (i + 1 >= binlength)
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			base64[j++] = '=';
			break;
		}
		current |= ((unsigned char)(bindata[i + 1] >> 4)) & ((unsigned char)0x0F);
		base64[j++] = base64char[(int)current];

		current = ((unsigned char)(bindata[i + 1] << 2)) & ((unsigned char)0x3C);
		if (i + 2 >= binlength)
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			break;
		}
		current |= ((unsigned char)(bindata[i + 2] >> 6)) & ((unsigned char)0x03);
		base64[j++] = base64char[(int)current];

		current = ((unsigned char)bindata[i + 2]) & ((unsigned char)0x3F);
		base64[j++] = base64char[(int)current];
	}
	base64[j] = '\0';
	return base64;
}

static const unsigned char g_pMap[256] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};
bool base64_decode(const char *strIn, unsigned char *pOut, size_t *uOutLen)
{
	size_t t, x, y, z;
	unsigned char c;
	size_t g = 3;

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));
	size_t len = strlen(strIn);
	for (x = y = z = t = 0; x < len; x++)
	{
		c = g_pMap[(const unsigned char)strIn[x]];
		if (c == 255) continue;
		if (c == 254) { c = 0; g--; }

		t = (t << 6) | c;

		if (++y == 4)
		{
			if ((z + g) > *uOutLen) { return false; } // Buffer overflow
			pOut[z++] = (unsigned char)((t >> 16) & 255);
			if (g > 1) pOut[z++] = (unsigned char)((t >> 8) & 255);
			if (g > 2) pOut[z++] = (unsigned char)(t & 255);
			y = t = 0;
		}
	}

	*uOutLen = z;
	return true;
}

char * encodePassword(const unsigned char * src, int srcLen, char * out)
{
	base64_encode(src, out, srcLen);

	// 自定义BASE64
	size_t outLen = strlen(out);
	for (size_t i = 0; i < outLen; i++)
	{
		if (out[i] == '+') { out[i] = '#'; continue; }
		if (out[i] == '/') { out[i] = '$'; continue; }
	}

	// 剥离BASE64编码后的等于号
	for (size_t i = 0; *(out + i) != '\0'; i++)
	{
		if (*(out + i) == '=') {
			*(out + i) = '\0';
			// break; // 去掉注释性能更高，保持注释更加安全
		}
	}

	return out;
}

char * decodePassword(char *szPassword)
{
	size_t len = strlen(szPassword);
	if (len == 0) return nullptr;
	int mod_len = 4 - len % 4;

	size_t bufferLength = len * 4 + 1;
	char * buffer = new char[bufferLength];
	strcpy_s(buffer, bufferLength, szPassword);

	// 还原成标准 BASE64
	for (size_t i = 0; i < len; i++)
	{
		if (buffer[i] == '#') { buffer[i] = '+'; continue; }
		if (buffer[i] == '$') { buffer[i] = '/'; continue; }
	}
	for (; mod_len > 0; mod_len--)
	{
		strcat(buffer, "=");
	}

	size_t outLen = len;
	char * szOutBuffer = new char[len + 1];
	memset(szOutBuffer, 0, len + 1);
	base64_decode(buffer, (unsigned char *)szOutBuffer, &outLen);
	strcpy_s(szPassword, len, szOutBuffer);

	delete[]szOutBuffer;
	delete[]buffer;
	return szPassword;
}

// 获取 GET 中的内容
std::string _GET(const std::string & get, const char *name)
{
	return _URLFIND(get, name);
}

std::string _URLFIND(const std::string & url, const char *name)
{
	std::string var(name);
	var.append("=");
	size_t len = var.length();
	std::string ret;

	size_t pos1 = url.find(var);
	size_t pos2 = url.find("&", pos1 + len);
	if (pos1 == std::string::npos)
	{
		return ret;
	}
	if (pos2 == std::string::npos)
	{
		ret = url.substr(pos1 + len, url.length() - pos1 - len);
	}
	else
	{
		ret = url.substr(pos1 + len, pos2 - pos1 - len);
	}
	return ret;
}
