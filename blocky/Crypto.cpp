#include "Crypto.h"

namespace Crypto {
	
	// generate private public key pair
	EC_KEY *genKey() {
		EC_KEY *ec_key = EC_KEY_new();
		EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		EC_KEY_set_group(ec_key, ec_group);
		EC_KEY_generate_key(ec_key);
		return ec_key;
	}

	// set the public part of the key
	int setPubKey(EC_KEY *eckey, std::string key, int ybit) {
		if (key.length() != 64) { 
			return -1;
		}else{
			BIGNUM *x = BN_new();
			BN_hex2bn(&x, key.c_str());
			EC_POINT *pub = EC_POINT_new(EC_KEY_get0_group(eckey));
			EC_POINT_set_compressed_coordinates_GFp(
				EC_GROUP_new_by_curve_name(NID_secp256k1),
				pub, x, ybit, NULL);
			return 0;
		}
	}

	// set the private part of the key
	int setPrivKey(EC_KEY *eckey, std::string key) {
		if (key.length()!=64) {
			return -1;
		}else {
			BIGNUM *bnkey = BN_new();
			BN_hex2bn(&bnkey, key.c_str());
			EC_KEY_set_private_key(eckey, bnkey);
			return 0;
		}
	}

	// return the signature of the message by private key
	std::string sign(std::string message, std::string strPrivKey) {
		EC_KEY *privKey = EC_KEY_new();
		EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		EC_KEY_set_group(privKey, ec_group);

		BIGNUM *bnpriv = BN_new();
		BN_hex2bn(&bnpriv, strPrivKey.c_str());
		EC_KEY_set_private_key(privKey, bnpriv);

		unsigned char hash[SHA256_DIGEST_LENGTH];
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, message.c_str(), message.length());
		SHA256_Final(hash, &sha256);

		const ECDSA_SIG *signature = ECDSA_do_sign(hash, SHA256_DIGEST_LENGTH, privKey);
		const BIGNUM *r = BN_new();
		const BIGNUM *s = BN_new();
		ECDSA_SIG_get0(signature, &r, &s);

		std::string sig = BN_bn2hex(r);
		sig += BN_bn2hex(s);
		return sig;
	}

	// return bool wether or not the signature is verfied
	int verify(std::string message, std::string signature, std::string publicKey) {
		if (publicKey.length() != 128 && signature.length() != 128) {
			return -1;
		}else {
			ECDSA_SIG *sig = ECDSA_SIG_new();
			BIGNUM *r = BN_new();
			BIGNUM *s = BN_new();
			BN_hex2bn(&r, signature.substr(1, 64).c_str());
			BN_hex2bn(&s, signature.substr(64, 64).c_str());
			ECDSA_SIG_set0(sig, r, s);

			unsigned char hash[SHA256_DIGEST_LENGTH];
			SHA256_CTX sha256;
			SHA256_Init(&sha256);
			SHA256_Update(&sha256, message.c_str(), message.length());
			SHA256_Final(hash, &sha256);
			
			EC_KEY *pubKey = EC_KEY_new();
			EC_KEY_set_group(pubKey, EC_GROUP_new_by_curve_name(NID_secp256k1));
			EC_POINT *pubKeyPoint = EC_POINT_new(EC_KEY_get0_group(pubKey));
			BIGNUM *bnx = BN_new();
			BIGNUM *bny = BN_new();
			BN_hex2bn(&bnx, publicKey.substr(1,64).c_str());
			BN_hex2bn(&bny, publicKey.substr(64, 64).c_str());
			EC_POINT_set_affine_coordinates_GFp(EC_KEY_get0_group(pubKey), pubKeyPoint, bnx, bny, NULL);
			EC_KEY_set_public_key(pubKey, pubKeyPoint);

			return ECDSA_do_verify(hash, SHA256_DIGEST_LENGTH, sig, pubKey);
		}
	}

}
