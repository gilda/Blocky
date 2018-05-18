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
	//                 make coinbase transactions verify-able
	//                 delete UTXO lines
	//				   bool validateBlock(){validate amount of gldc using UTXO file}
	//TODO Transaction: make input and outputs
	//TODO Crypto: setPublicKey(std::string), setPrivateKey(std::string)
	//TODO Block: 
	//TODO CLI:
	//TODO GUI:
	//TODO Util:
	//TODO FileManager: 

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc", 32, 10, 10);

	EC_KEY *key = Crypto::genKey();
	EC_KEY *key1 = Crypto::genKey();
	gldc.addTransaction(Util::base58Encode(Crypto::getPrivateString(key1)), Util::base58Encode(Crypto::getPublicString(key1)), 5, Util::base58Encode(Crypto::getPublicString(key)));
	gldc.mineLastBlock(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)));
	std::vector<int> p= gldc.getTransInputForValue(Util::base58Encode(Crypto::getPublicString(key)), 12);
	for(std::vector<int>::iterator it = p.begin(); it != p.end(); it++){
		printf("%s\n", FileManager::readLine(gldc.getFilePath()+".utxo", *it).c_str());
	}

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
