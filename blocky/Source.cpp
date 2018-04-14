#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"

int main() {
	//TODO Util: int bits numZeroHash()
	//TODO Crypto user and signing
	//TODO BlockChain: addTransaction, addBlock, validateTransaction, validateBlock
	//TODO Ledger: openFile, writeBlockchain, start parser
	//TODO UI:
	//TODO Transaction:
	//TODO Block: 
	Util::initOpenSSL(); // init OpenSSL

	Blockchain gldc = Blockchain("gldc.ledg", 4, 10);
	gldc.mineLastBlock();
	printf("%s", gldc.getGenesis()->toString().c_str());


	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
