#pragma once
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>
#include <random>
#include <ctime>
#include <openssl\ssl.h>
#include <openssl\sha.h>
#include <openssl\err.h>
#include <openssl\bn.h>
#include <openssl\conf.h>
#include <openssl\engine.h>


#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

namespace Util{
	void initOpenSSL();
	void cleanupOpenSSL();
	int numZeroHash(std::string hash);
	std::string Hash256(std::string mess);
	std::string base58Encode(std::string hexnum);
	std::string base58Decode(std::string str);
	int random(int min, int max);
}