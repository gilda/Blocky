#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"

int main() {
	//TODO Transaction: stringifyVerify(), verify() 
	//TODO Crypto: sign return
	//TODO Block: validateTransaction()
	//TODO BlockChain: addTransaction(), addBlock(), validateBlock()
	//TODO Ledger: openFile(), writeBlockchain(), start parser
	//TODO Util:
	//TODO UI:

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc.ledg", 32, 10);
	gldc.mineLastBlock();
	printf("%s\r\n", gldc.getGenesis()->toString().c_str());

	Transaction money = Transaction(0, "gilda", 5, "baba");
	std::string raw = money.stringify();
	printf("stringify: %s\r\n", money.stringify().c_str());
	printf("hash: %s\r\n", Util::Hash256(money.stringify()).c_str());
	EC_KEY *key = Crypto::genKey();
	printf("pubKey: %s\r\n", Crypto::getPublicString(key).c_str());
	printf("priKey: %s\r\n", Crypto::getPrivateString(key).c_str());
	money.sign(Crypto::getPrivateString(key));
	printf("signature: %s\r\n", money.getSignature().c_str());
	printf("verify: %s\r\n", Crypto::verify("0#gilda->5->baba&&", money.getSignature(), Crypto::getPublicString(key))== 1 ? "true" : "false");

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
