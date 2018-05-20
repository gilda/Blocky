#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"
#include "FileManager.h"

int main() {
	//TODO list ordered by importance level
	//TODO Blockchain: write UTXO file writing \ reading functions
	//                 generate output change transaction
	//				   bool validateBlock(){validate amount of gldc using UTXO file, validate hashes and validate signatures}
	//TODO Transaction: make input and outputs
	//TODO Crypto: 
	//TODO Block: 
	//TODO CLI: 
	//TODO GUI: 
	//TODO Util: 
	//TODO FileManager: 

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc", 32, 10, 10);

	EC_KEY *key = Crypto::genKey();
	EC_KEY *key1 = Crypto::genKey();
	gldc.mineLastBlock(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)));
	gldc.addTransaction(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 5, Util::base58Encode(Crypto::getPublicString(key1)));
	gldc.mineLastBlock(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)));

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
