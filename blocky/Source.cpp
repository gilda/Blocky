#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"

int main() {
	//TODO Crypto: EC_KEY genKey(), void setPubKey(key, std::string key), void setPrivKey(key, std::string key), string sign(hash, privkey), bool verify(hash, pubkey)
	//TODO BlockChain: addTransaction, addBlock, validateTransaction, validateBlock
	//TODO Ledger: openFile, writeBlockchain, start parser
	//TODO Util: 
	//TODO UI:
	//TODO Transaction:
	//TODO Block: 
	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc.ledg", 32, 10);
	gldc.mineLastBlock();
	printf("%s", gldc.getGenesis()->toString().c_str());


	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
