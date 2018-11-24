#include "stdafx.h"
#include "Util.h"

namespace Util {
	// initialize OpenSSL crypto library
	void initOpenSSL() {
		printf("initializing OpenSSL...\n");
		srand((unsigned)time(0));
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
		SSL_COMP_free_compression_methods();
		ENGINE_cleanup();
		CONF_modules_free();
		CONF_modules_unload(1);
		COMP_zlib_cleanup();
		ERR_free_strings();
		EVP_cleanup();
		CRYPTO_cleanup_all_ex_data();
		printf("cleaning up OpenSSL...\n");
	}


	// return the number of bits that are zero. used for mining Blocks
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

	// returns base58 encoding of hex number
	std::string base58Encode(std::string hexnum){
		// all legal chars
		char alphabet[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
		unsigned int zeros = 0;

		// count zeros ahead of string base58('0')='1'
		bool cont = true;
		unsigned int i = 0;
		while(cont){
			if(i<hexnum.length() && hexnum.at(i)=='0'){ // make sure not going out of bounds
				//hexnum.erase(i, 1);
				i++;
				zeros++;
			} else{
				cont = false;
			}
		}

		BIGNUM *num = BN_new();
		BN_hex2bn(&num, hexnum.c_str()); // convert input to BIGNUM
		std::string ret;

		if(hexnum!=""){
			// init all inside vars
			BIGNUM *BN58 = BN_new();
			BIGNUM *div = BN_new();
			BN_CTX *ctx = BN_CTX_new(); // ctx for div
			BN_copy(div, num);
			BN_set_word(BN58, 58); // 58

			if(BN_is_zero(num)){
				ret = alphabet[0];
				// return '1' if input is zero
			}else{
				BIGNUM *idx = BN_new();
				while(!BN_is_zero(div)){
					BN_div(div, idx, num, BN58, ctx); // div = num / 58 , idx = num % 58
					BN_copy(num, div); // num = div
					ret = alphabet[std::atoi(BN_bn2dec(idx))]+ret; // add character to the string
				}
			}
		}
		
		// prepend 1's as they are decoded to zero
		for(unsigned int i = 0; i<zeros; i++){
			ret = "1"+ret;
		}

		return ret;
	}

	// returns hex number of base58 encoding
	std::string base58Decode(std::string str){
		// reverse the string
		unsigned int zeros = 0;

		// count zeros ahead of string base58('0')='1'
		bool cont = true;
		unsigned int i = 0;
		while(cont){
			if(i<str.length()&&str.at(i)=='1'){ // make sure not going out of bounds
				//str.erase(i,1);
				i++;
				zeros++;
			} else{
				cont = false;
			}
		}

		std::reverse(str.begin(), str.end());
		std::string ret;
		if(str!=""){
			//init variables and context
			std::string alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
			BN_CTX *ctx = BN_CTX_new();
			BN_CTX_start(ctx);

			unsigned int index = 0; // init index of string
			BIGNUM *dec = BN_new();
			BIGNUM *multi = BN_new();
			BIGNUM *inter = BN_new();
			BIGNUM *indexbn = BN_new();
			BIGNUM *b58 = BN_new();

			BN_set_word(b58, 58); // b58 = 58
			BN_one(multi); // multi = 1
			BN_zero(dec); // dec = 0

			while(zeros+index<str.length()){
				BN_set_word(indexbn, (int)alphabet.find(str.at(index))); // indexbn = alphabet.indexof(str.at(index))
				BN_mul(inter, multi, indexbn, ctx); // inter = multi * indexbn
				BN_add(dec, dec, inter); // dec = dec + inter
				BN_mul(multi, multi, b58, ctx); // multi = multi * 58
				index++; // increment index for next character
			}
			ret = BN_bn2hex(dec);
			if(ret.at(0)=='0'){
				ret.erase(0, 1);
			}
		}

		// prepend zeros
		for(unsigned int i = 0; i<zeros; i++){
			ret = "0"+ret;
		}

		return ret;
	}

	// returns random int between min and max
	int random(int min, int max){
		return rand()%max+min;
	}

	// return the help text for the program
	std::string helpText(){
		return "Welcome to the gildots blockchain program!\nLegal commands are:\n\nhelp    display this help message\n\n";
	}
}
