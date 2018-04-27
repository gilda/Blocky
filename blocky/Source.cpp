#include <stdio.h>
#include <fstream>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"
#include "FileManager.h"

int main() {
	//TODO FileManager: 
	//TODO Blockchain: bool validateBlock(){validate amount of gldc using UTXO file} \
	//                 write UTXO file writing reading functions
	//TODO Transaction: change members to UTXO based
	//TODO CLI:
	//TODO Block: 
	//TODO Crypto: 
	//TODO Util:
	//TODO GUI:

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc.ledg", 32, 10);
	
	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
