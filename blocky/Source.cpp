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
	//TODO Util: Base58Encode(BIGNUM key), Base58Decode(std::string)
	//TODO Blockchain: write UTXO file writing reading functions
	//				   bool validateBlock(){validate amount of gldc using UTXO file}
	//                 change block array to LinkedList \ Vector
	//TODO CLI:
	//TODO GUI:
	//TODO FileManager: 
	//TODO Transaction: 
	//TODO Block: 
	//TODO Crypto: 

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc", 32, 10);
	
	

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
