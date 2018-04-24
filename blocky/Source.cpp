#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"

int main() {
	//TODO Blockchain: bool validateBlock()
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
	gldc.mineLastBlock();
	printf("block: %s\r\n\r\n", gldc.getGenesis()->toString().c_str());
	printf("verify: %s\r\n", gldc.getLastBlock()->verifyTransaction(
		*gldc.getLastBlock()->getLastTransaction(),
		gldc.getLastBlock()->getLastTransaction()->getSignature(),
		Crypto::getPublicString(key))==1 ? "true" : "false");


	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
