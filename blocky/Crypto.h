#pragma once
#include <string>
#include <openssl\ec.h>
#include <openssl\sha.h>
#include <openssl\bn.h>
#include "Util.h"

namespace Crypto {
	EC_KEY *genKey();
	std::string derivePublicFromPrivate(std::string privKey);
	std::string getPublicString(EC_KEY *key);
	std::string getPrivateString(EC_KEY *key);
	std::string sign(std::string message, std::string strPrivKey);
	bool verify(std::string message, std::string signature, std::string publicKey);
}
