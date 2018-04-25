#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"

int main() {
	//TODO Blockchain: bool validateBlock(){validate amount of gldc}
	//TODO Ledger: openFile(), writeBlockchain(), start parser
	//TODO CLI:
	//TODO Transaction: 
	//TODO Block: 
	//TODO Crypto: 
	//TODO Util:
	//TODO GUI:

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc.ledg", 32, 10);
	EC_KEY *key = Crypto::genKey();
	EC_KEY *key1 = Crypto::genKey();
	gldc.addTransaction(Crypto::getPrivateString(key), Crypto::getPublicString(key), 10, Crypto::getPublicString(key1));
	printf("transaction string: %s\r\n\r\n", gldc.getLastBlock()->getLastTransaction()->stringify().c_str());
	gldc.addTransaction(Crypto::getPrivateString(key1), Crypto::getPublicString(key1), 5, Crypto::getPublicString(key));
	printf("transaction string: %s\r\n\r\n", gldc.getLastBlock()->getTransaction(1)->stringify().c_str());
	gldc.mineLastBlock();
	printf("verify block: %s\r\n\r\n", gldc.validateBlock(0) ? "true" : "false");
	printf("block genesis: %s\r\n\r\n", gldc.getBlock(0)->toString().c_str());

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
