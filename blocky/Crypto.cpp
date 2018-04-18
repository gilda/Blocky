#include "Crypto.h"

namespace Crypto {
	EC_KEY *genKey() {
		EC_KEY *ec_key = EC_KEY_new();
		EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		EC_KEY_set_group(ec_key, ec_group);
		EC_KEY_generate_key(ec_key);
		return ec_key;
	}

	void setPubKey(EC_KEY *eckey, std::string key) {
		EC_POINT *g = EC_POINT_new(EC_KEY_get0_group(eckey));

	}
}