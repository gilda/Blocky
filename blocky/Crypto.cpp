#include "stdafx.h"
#include "Crypto.h"

namespace Crypto {
	
	// generate private public key pair
	EC_KEY *genKey() {
		EC_KEY *ec_key = EC_KEY_new(); // allocate memory
		EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		EC_KEY_set_group(ec_key, ec_group); // set curve group
		EC_KEY_generate_key(ec_key); // find keys
		return ec_key;
	}

	// returns the string of public key given
	// not base58 encoded
	std::string getPublicString(EC_KEY *key) {
		const EC_POINT *point = EC_KEY_get0_public_key(key); // get point of public key
		BIGNUM *x = BN_new();
		BIGNUM *y = BN_new();
		EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(key), point, x, y, NULL); // extract to BN
		std::string ret = BN_bn2hex(x)+std::to_string(BN_is_odd(y));
		return ret; // return as string the public key in compressed form
	}

	// returns the string of private key given
	// not base58 encoded
	std::string getPrivateString(EC_KEY *key) {
		const BIGNUM *priv = EC_KEY_get0_private_key(key);
		std::string ret = BN_bn2hex(priv);
		return ret;
	}

	// sets the public string of key
	void setPublicString(EC_KEY *key, std::string pubKey){
		BIGNUM *pubKeyx = BN_new();
		BN_hex2bn(&pubKeyx, pubKey.substr(0, 64).c_str()); // parse x part
		int ybit; // ybit on y is positive or negative
		sscanf_s(pubKey.substr(64, 1).c_str(), "%d", &ybit); // parse y bit
		EC_POINT *pubKeyPoint = EC_POINT_new(EC_KEY_get0_group(key)); // init point
	}

	// sets the private part of key
	void setPrivateString(EC_KEY *key, std::string privKey){
		BIGNUM *bnPriv = BN_new();
		BN_hex2bn(&bnPriv, privKey.c_str()); // parse private key
		EC_KEY_set_private_key(key, bnPriv); // set private part to parsed BN
	}

	// return the signature of the message by private key
	// not base58 encoded
	std::string sign(std::string message, std::string strPrivKey) {
		unsigned char hash[SHA256_DIGEST_LENGTH]; // allocate hash char
		SHA256_CTX sha256;
		SHA256_Init(&sha256); // init hash object
		SHA256_Update(&sha256, message.c_str(), message.length()); // hash the message given
		SHA256_Final(hash, &sha256); // ouput to allocated char

		EC_KEY *ec_key = EC_KEY_new();
		EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		EC_KEY_set_group(ec_key, ec_group); // allocate memory and init key
		BIGNUM *bnPriv = BN_new();
		BN_hex2bn(&bnPriv, strPrivKey.c_str()); // parse private key
		EC_KEY_set_private_key(ec_key, bnPriv); // set private part to parsed BN

		const ECDSA_SIG *sig = ECDSA_do_sign(hash, SHA256_DIGEST_LENGTH, ec_key); // sign the message
		const BIGNUM *r = BN_new();
		const BIGNUM *s = BN_new();
		ECDSA_SIG_get0(sig, &r, &s); // get EC_POINT to BN

		std::string signature(BN_bn2hex(r)); // parse r part of sig
		signature += BN_bn2hex(s); // parse s part of sig
		return signature;
	}

	// return bool wether or not the signature is verfied
	bool verify(std::string message, std::string signature, std::string publicKey) {
		unsigned char hash[SHA256_DIGEST_LENGTH]; // allocate hash char
		SHA256_CTX sha256;
		SHA256_Init(&sha256); // init hash object
		SHA256_Update(&sha256, message.c_str(), message.length()); // hash the message given
		SHA256_Final(hash, &sha256); // ouput to allocated char

		ECDSA_SIG *sig = ECDSA_SIG_new(); // allocate signature
		BIGNUM *r = BN_new();
		BIGNUM *s = BN_new();
		BN_hex2bn(&r, signature.substr(0, 64).c_str()); // parse r part
		BN_hex2bn(&s, signature.substr(64, 64).c_str()); // parse s part
		ECDSA_SIG_set0(sig, r, s); // set r and s to sig

		EC_KEY *ec_key = EC_KEY_new();
		EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		EC_KEY_set_group(ec_key, ec_group); // init and setup key
			
		BIGNUM *pubKeyx = BN_new();
		BN_hex2bn(&pubKeyx, publicKey.substr(0,64).c_str()); // parse x part
		int ybit; // ybit on y is positive or negative
		sscanf_s(publicKey.substr(64, 1).c_str(), "%d", &ybit); // parse y bit
		EC_POINT *pubKeyPoint = EC_POINT_new(EC_KEY_get0_group(ec_key)); // init point
		
		// set point x and y to x by group and y bit
		EC_POINT_set_compressed_coordinates_GFp(EC_KEY_get0_group(ec_key), pubKeyPoint, pubKeyx, ybit, NULL); 
			
		EC_KEY_set_public_key(ec_key, pubKeyPoint); // set public key to point parsed
			
		return ECDSA_do_verify(hash, SHA256_DIGEST_LENGTH, sig, ec_key); // check the signature	
	}
}
