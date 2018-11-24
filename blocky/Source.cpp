#include "stdafx.h"
#include "Crypto.h"
#include "Util.h"
#include "Blockchain.h"
#include "Block.h"
#include "Transaction.h"

int main(int argc, char* argv[]) {
	//TODO ordered by importance
	//TODO COMMENT LAST COMMIT!
	//TODO CLI: TODO get rid of all the system("pause") before release
	//TODO Block: TODO insert metadata
	//TODO Blockchain: TODO parseBlockchain, TODO constructor writes to block file the params
	//TODO GUI:
	//TODO Transaction: 
	//TODO Crypto: 
	//TODO Util: fix help message. 
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
		}else if(argv[1] == std::string("init").c_str()){ // create a new blockchain
			// TODO
		}else if(argv[1] == std::string("printBlockchainParams").c_str()){ // print the init params of some blockchain
			// syntax: blocky printBlockchainParams <filePath>
			std::string filePath = argv[2];

			// TODO possibly use Blockchian.parseBlockchain(filePath);
			int numBlocks;
			int reward;
			int difficulty;
			
			system("pause");
			return 0;
		}else if(argv[1] == std::string("printBlock").c_str()){
			// syntax: blocky printBlock <file path> <block height>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);
			// TODO find block in block file and print the entire block with possibly metadata

			system("pause");
			return 0;
		}else if(argv[1] == std::string("printTransaction").c_str()){
			// syntax: blocky printTransaction <file path> <transaction hash>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			std::string hash = argv[3];

			// TODO loop through all of blockchain and find the transaction
			system("pause");
			return 0;
		}else if(argv[1] == std::string("genKey").c_str()){
			// syntax: blocky genKey
			system("pause");
			return 0;
		}else if(argv[1] == std::string("getBalance").c_str()){
			// syntax: blocky getBalance <file path> <address>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			std::string address = argv[3];

			// loop through all of UTXO file and accumulate balance of address
			system("pause");
			return 0;
		}else if(argv[1] == std::string("sendTransaction").c_str()){
			// syntax: blocky sendTransaction <file path> <private key> <public key> <amount> <address>
			if(argc < 7){return 1;}
			std::string filePath = argv[2];
			std::string privKey = argv[3];
			std::string pubKey = argv[4];
			int amount = std::stoi(argv[5]);
			std::string address = argv[6];

			// TODO add to the transaction pool file the transaction
			system("pause");
			return 0;
		}else if(argv[1] == std::string("mineBlock").c_str()){
			// syntax: blocky mineBlock <file path> <tranaction hash> [<--metadata>]
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			
			// TODO loop through all argv[3++], construct block, mine it, and add it to the block file
			system("pause");
			return 0;
		}else if(argv[1] == std::string("verifyBlock").c_str()){
			// syntax: blocky verifyBlock <file path> <block height>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);

			// TODO loop through all of block file, find the block and dump it with metadata
			system("pause");
			return 0;
		}else if(argv[1] == std::string("verifyBlockchain").c_str()){
			// syntax: blocky verifyBlockchain <file path>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];

			// TODO loop through entire block file and verify each block, if one is invalid, produce a message
			system("pause");
			return 0;
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
