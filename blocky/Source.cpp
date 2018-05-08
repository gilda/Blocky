#include <stdio.h>
#include <fstream>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"
#include "FileManager.h"

int main() {
	//TODO FileManager: deleteLine(), getLastLineNum()
	//TODO Blockchain: write UTXO file writing reading functions
	//				   bool validateBlock(){validate amount of gldc using UTXO file}                 
	//TODO Transaction: 
	//TODO CLI:
	//TODO Block: 
	//TODO Crypto: 
	//TODO Util:
	//TODO GUI:

	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc", 32, 10);
	
	EC_KEY *key = Crypto::genKey();
	Transaction gilda = Transaction(Crypto::getPublicString(key), 5, Crypto::getPublicString(key));
	gilda.sign(Crypto::getPrivateString(key));
	FileManager::writeLine(gldc.getFilePath()+".utxo", gilda.stringify(), 0);

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
