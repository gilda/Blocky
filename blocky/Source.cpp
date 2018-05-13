#include <stdio.h>
#include <fstream>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"
#include "FileManager.h"

int main() {
	//TODO list ordered by importance level
	//TODO Transaction: implement base58 on keys and signature 
	//TODO Blockchain: write UTXO file writing \ reading functions
	//				   bool validateBlock(){validate amount of gldc using UTXO file}
	//                 change block array to LinkedList \ Vector
	//TODO Util:
	//TODO CLI:
	//TODO GUI:
	//TODO FileManager: 
	//TODO Block: 
	//TODO Crypto: setPublicKey(std::string), setPrivateKey(std::string)

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc", 32, 10);

	EC_KEY *key = Crypto::genKey();
	gldc.addTransaction(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 5, Util::base58Encode(Crypto::getPublicString(key)));

	printf("public Key: %s\n", Util::base58Encode(Crypto::getPublicString(key)).c_str());
	printf("private Key: %s\n", Util::base58Encode(Crypto::getPrivateString(key)).c_str());
	printf("signature: %s\n", gldc.getLastBlock()->getLastTransaction()->getSignature().c_str());
	printf("verify: %s\n", gldc.getLastBlock()->verifyTransaction(*gldc.getLastBlock()->getLastTransaction(), gldc.getLastBlock()->getLastTransaction()->getSignature(), Util::base58Encode(Crypto::getPublicString(key)))==1 ? "true" : "false");
	printf("\ntoString\n\n%s\n", gldc.getLastBlock()->getLastTransaction()->toString().c_str());

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
