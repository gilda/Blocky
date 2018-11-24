#include "stdafx.h"
#include "Crypto.h"
#include "Util.h"
#include "Blockchain.h"
#include "Block.h"
#include "Transaction.h"

int main(int argc, char* argv[]) {
	//TODO ordered by importance
	//TODO COMMENT LAST COMMIT!
	//TODO CLI: get rid of all the system("pause") before release
	//TODO Block: 
	//TODO Blockchain: 
	//TODO GUI:
	//TODO Transaction: 
	//TODO Crypto: 
	//TODO Util: 
	//TODO FileManager:

	// parse command line arguments and act accordingly
	if(argc <= 1){
		printf("%s", Util::helpText().c_str());
		system("pause");
		return 0;
	}else{
		if(argv[1] == std::string("help").c_str()){
			printf("%s", Util::helpText().c_str());
			system("pause");
			return 0;
		}else if(argv[1] == std::string("printBlockchainParams").c_str()){
			// TODO
		}else if(argv[1] == std::string("printBlock").c_str()){
			// TODO
		}else if(argv[1] == std::string("printTransaction").c_str()){
			// TODO
		}else if(argv[1] == std::string("genKey").c_str()){
			// TODO
		}else if(argv[1] == std::string("getBalance").c_str()){
			// TODO
		}else if(argv[1] == std::string("sendTransaction").c_str()){
			// TODO
		}else if(argv[1] == std::string("mineBlock").c_str()){
			// TODO
		}else if(argv[1] == std::string("verifyBlock").c_str()){
			// TODO
		}else if(argv[1] == std::string("verifyBlockchain").c_str()){
			// TODO
		}
	}

	Util::initOpenSSL(); // init OpenSSL
	/*Blockchain gldc = Blockchain("gldc", 16, 10);
	remove((gldc.getFilePath()+".utxo").c_str());
	remove((gldc.getFilePath()+".blck").c_str());

	EC_KEY *key = Crypto::genKey();
	EC_KEY *key1 = Crypto::genKey();
	
	Block b0 = Block(Util::Hash256(gldc.getFilePath()), 0);
	b0.mine(gldc.getDifficulty(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), gldc.getReward());
	gldc.addBlock(b0);

	Block b1 = Block(gldc.getLastBlock()->getCurrHash(), gldc.getLastBlock()->getId()+1);
	b1.addTransaction(gldc.getFilePath(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 6, Util::base58Encode(Crypto::getPublicString(key1)));
	b1.mine(gldc.getDifficulty(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), gldc.getReward());
	gldc.addBlock(b1);

	Block b2 = Block(gldc.getLastBlock()->getCurrHash(), gldc.getLastBlock()->getId()+1);
	b2.addTransaction(gldc.getFilePath(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 6, Util::base58Encode(Crypto::getPublicString(key1)));
	b2.mine(gldc.getDifficulty(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), gldc.getReward());
	gldc.addBlock(b2);*/

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
