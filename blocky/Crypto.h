#pragma once
#include <string>
#include <openssl\ec.h>
#include <openssl\sha.h>
#include <openssl\bn.h>
#include "Util.h"

namespace Crypto {
	EC_KEY *genKey();
	void setPubKey(EC_KEY *eckey, std::string key);
}
