#pragma once
#include <string>
#include <openssl\ec.h>
#include <openssl\sha.h>
#include <openssl\bn.h>
#include "Util.h"

namespace Crypto {
	EC_KEY *genKey();
	int setPubKey(EC_KEY *eckey, std::string key, int ybit);
	int setPrivKey(EC_KEY *eckey, std::string key);
	std::string sign(std::string message, std::string strPrivKey);
	int verify(std::string hash, std::string signature, std::string publicKey);
}
