#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"
#include "FileManager.h"

int main() {
	//TODO ordered by importance
	//TODO Blockchain: bool validateBlock(Block){validate amount of gldc using UTXO file and BLCK file, validate signatures}
	//TODO Block: check Block::mined use
	//TODO Network:
	//TODO CLI: 
	//TODO GUI:
	//TODO Transaction: 
	//TODO Crypto: 
	//TODO Util: 
	//TODO FileManager:

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc", 16, 10);
	remove((gldc.getFilePath()+".utxo").c_str());
	remove((gldc.getFilePath()+".blck").c_str());

	EC_KEY *key = Crypto::genKey();
	EC_KEY *key1 = Crypto::genKey();
	
	gldc.mineLastBlock(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)));
	gldc.addTransaction(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 6, Util::base58Encode(Crypto::getPublicString(key1)));
	gldc.mineLastBlock(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)));
	gldc.addTransaction(Util::base58Encode(Crypto::getPrivateString(key1)), Util::base58Encode(Crypto::getPublicString(key1)), 6, Util::base58Encode(Crypto::getPublicString(key)));
	gldc.mineLastBlock(Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)));

	printf("block utxo validation: %s\n", gldc.validateLastBlockUTXO() == true ? "True" : "False");

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
