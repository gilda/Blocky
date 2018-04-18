#include "Util.h"

namespace Util {
	// initialize OpenSSL crypto library
	void initOpenSSL() {
		SSL_library_init();
		SSL_load_error_strings();
		ERR_load_BIO_strings();
		OpenSSL_add_all_algorithms();
	}

	// CleanUp OpenSSL after use
	void cleanupOpenSSL() {
		FIPS_mode_set(0);
		CRYPTO_set_locking_callback(nullptr);
		CRYPTO_set_id_callback(nullptr);
		ERR_remove_state(0);
		SSL_COMP_free_compression_methods();
		ENGINE_cleanup();
		CONF_modules_free();
		CONF_modules_unload(1);
		COMP_zlib_cleanup();
		ERR_free_strings();
		EVP_cleanup();
		CRYPTO_cleanup_all_ex_data();
	}


	// return the number of bytes that are zero. used for mining Blocks
	int numZeroHash(const std::string hash) {
		int len = 0;
		for (int i = 0; i < (int)hash.size(); i++) {
			char c = hash.at(i);
			if (c == '0') { len += 8; }else{
				switch (c) {
				case '1': len += 7; break;
				case '2': len += 6; break;
				case '3': len += 5; break;
				case '4': len += 4; break;
				case '5': len += 3; break;
				case '6': len += 2; break;
				case '7': len += 1; break;
				case '8': len += 0; break;
				}
				break;
			}
		}
		return len;
	}

	// return string SHA256 hash of @param mess
	std::string Hash256(const std::string mess) {
		// input and output
		std::stringstream ss;
		unsigned char hash[SHA256_DIGEST_LENGTH];
		// init SHA256 from OpenSSL
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		// hash the message
		SHA256_Update(&sha256, mess.c_str(), mess.size());
		// output to the char [] hash
		SHA256_Final(hash, &sha256);
		// re-encode the char[] to hex string
		for (int i = 0; i < 32; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)hash[i];
		}
		return ss.str();
	}
}