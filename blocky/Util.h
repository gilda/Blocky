#pragma once
// dependencies
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl\ssl.h>
#include <openssl\sha.h>
#include <openssl\err.h>
#include <openssl\conf.h>
#include <openssl\engine.h>

#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

namespace Util {
	void initOpenSSL();
	void cleanupOpenSSL();
	int numZeroHash(std::string hash);
	std::string Hash256(std::string mess);
}