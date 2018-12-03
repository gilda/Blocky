#include "stdafx.h"
#include "Crypto.h"
#include "Util.h"
#include "Blockchain.h"
#include "Block.h"
#include "Transaction.h"

int main(int argc, char* argv[]) {
	//CLI: TODO get rid of all the system("pause") before release, TODO get rid of weird printf()s, TODO change return 1s to usefull error messages, TODO sanitize inputs
	//Block: 
	//Blockchain: TODO 3 block UTXO bug
	//Crypto: TODO genPubFromPriv()
	//GUI:
	//Transaction:
	//Util:
	//FileManager:

	/*remove(std::string("gldc.utxo").c_str());
	remove(std::string("gldc.blck").c_str());
	remove(std::string("gldc.meta").c_str());
	Blockchain gldc = Blockchain("gldc", 16, 10, 40, "gilda");

	EC_KEY *key = Crypto::genKey();
	EC_KEY *key1 = Crypto::genKey();

	Block b0 = Block(Util::Hash256(gldc.getFilePath()), 0);
	b0.mine(gldc.getDifficulty(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), gldc.getReward(), "Gilda mined this block!");
	gldc.addBlock(b0);
	
	/*Block b1 = Block::parseBlock(gldc.getFilePath() + ".blck", 0);
	printf("%s\n", b1.stringifyBLCK().c_str());
	
	Block b1 = Block(gldc.getLastBlock()->getCurrHash(), gldc.getLastBlock()->getId()+1);
	b1.addTransaction(gldc.getFilePath(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 6, Util::base58Encode(Crypto::getPublicString(key1)));
	b1.mine(gldc.getDifficulty(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), gldc.getReward());
	gldc.addBlock(b1);

	Block b2 = Block(gldc.getLastBlock()->getCurrHash(), gldc.getLastBlock()->getId()+1);
	b2.addTransaction(gldc.getFilePath(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), 6, Util::base58Encode(Crypto::getPublicString(key1)));
	b2.mine(gldc.getDifficulty(), Util::base58Encode(Crypto::getPrivateString(key)), Util::base58Encode(Crypto::getPublicString(key)), gldc.getReward());
	gldc.addBlock(b2);

	Blockchain parsed = Blockchain::parseBlockchain("gldc");*/
	
	// parse command line arguments and act accordingly

	if(argc <= 1){
		printf("%s", Util::helpText().c_str());
		system("pause");
		return 0;
	}else{
		if(argv[1] == std::string("help") || argv[1] == std::string("--help")){
			// syntax: blocky {help, --help}
			// print the help text
			printf("%s", Util::helpText().c_str());
			return 0;
		
		}else if(argv[1] == std::string("init")){
			// syntax: blocky init <file path> <difficulty> <reward> <maxMetadataChar> <name>
			// parse the command line argumenst
			if(argc < 7){return 1;}
			// init the OpenSSL library
			Util::initOpenSSL();

			std::string filePath = argv[2];
			int difficulty = std::stoi(argv[3]);
			int reward = std::stoi(argv[4]);
			int maxMetadataChar = std::stoi(argv[5]);
			std::string name = argv[6];
			
			// create the new blockchain
			Blockchain newBlockchain = Blockchain(filePath, difficulty, reward, maxMetadataChar,name);
			printf("created a new blockcahin named %s under the file name %s with mining difficulty of %d and reward of %d", name.c_str(), filePath.c_str(), difficulty, reward);
			
			// clean the OpenSSL library
			Util::cleanupOpenSSL();
			return 0;
		}else if(argv[1] == std::string("printBlockchainParams")){ // print the init params of some blockchain
			// syntax: blocky printBlockchainParams <filePath>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			// check that the metadata file exists
			if(!FileManager::isFile(filePath + ".meta")){return 1;}

			// parse and print the parameters
			std::string name = Blockchain::parseBlockchain(filePath).getName();
			int difficulty = Blockchain::parseBlockchain(filePath).getDifficulty();
			int reward = Blockchain::parseBlockchain(filePath).getReward();
			
			// output the relevant data
			printf("this is the blockchain file path for a blockchain named %s with mining difficulty of %d and block reward of %d", name.c_str(), difficulty, reward);
			
			return 0;
		}else if(argv[1] == std::string("printBlock")){
			// TODO add option for metadata printage
			// syntax: blocky printBlock <file path> <block height>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);

			// check if file does not exist
			if(!FileManager::isFile(filePath + ".blck")){return 1;}

			// make sure block exists in the file
			if(Block::parseBlock(filePath + ".blck", blockHeight).empty()){return 1;}
			
			// find block in block file and print the entire block with metadata
			printf("%s\n", Block::parseBlock(filePath + ".blck", blockHeight).toString().c_str());

			return 0;
		}else if(argv[1] == std::string("printTransaction")){
			// syntax: blocky printTransaction <file path> <transaction hash>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			std::string hash = argv[3];
			
			// check if file does not exist
			if(!FileManager::isFile(filePath + ".blck")){return 1;}

			// find the max block
			int maxBlock = 0;
			while(!Block::parseBlock(filePath + ".blck", maxBlock).empty()){
				maxBlock++;
			}
			
			// loop over all transaction in blockchain until the one with the same hash is found
			for(int i = 0; i < maxBlock; i++){
				Block blockToCheck = Block::parseBlock(filePath + ".blck", i);
				for(std::vector<Transaction>::iterator it = blockToCheck.getTransactionVec().begin(); it != blockToCheck.getTransactionVec().end(); it++){
					// check whether the hash of the transaction found is the same as the one given
					if(it->getHash() == hash){
						printf("%s\n", it->toString().c_str());
						return 0;
					}
				}
			}

			return 1;
		}else if(argv[1] == std::string("genKey")){
			// syntax: blocky genKey
			// TODO add parameter for deriving public from private
			Util::initOpenSSL();
			// create a new key pair
			EC_KEY *newKey = Crypto::genKey();
			
			// prompt the user
			printf("this is your new public key, you can send this key to others so they can send you some tokens:\n%s\n", Util::base58Encode(Crypto::getPublicString(newKey)).c_str());
			printf("if you are ready, press any key to view your private key\n");
			system("pause");
			printf("this is your new private key, keep it only to yourself and it should be only used to recover your public key or send tokens:\n%s\n", Util::base58Encode(Crypto::getPrivateString(newKey)).c_str());
			
			// clean the OpenSSL library
			Util::cleanupOpenSSL();
			return 0;
		}else if(argv[1] == std::string("getBalance")){
			// syntax: blocky getBalance <file path> <address>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			std::string address = argv[3];

			// loop through all of UTXO file and accumulate balance of address
			system("pause");
			return 0;
		}else if(argv[1] == std::string("addTransaction")){
			// syntax: blocky addTransaction <file path> <private key> <public key> <amount> <address>
			if(argc < 7){return 1;}
			// init the OpenSSL library
			Util::initOpenSSL();

			std::string filePath = argv[2];
			std::string privKey = argv[3];
			std::string pubKey = argv[4];
			int amount = std::stoi(argv[5]);
			std::string address = argv[6];

			// add the transaction to the txpool for miners to add
			Blockchain::addToTransactionPool(filePath, privKey, pubKey, amount, address);
			
			// clean the OpenSSL library
			Util::cleanupOpenSSL();
			return 0;
		}else if(argv[1] == std::string("mineBlock")){
			// syntax: blocky mineBlock <file path> <private Key> <public Key> <tranaction hash>... [<--metadata>]
			// TODO add option for just line numbers in transaction pool file
			if(argc < 5){ // not enough parameters
				return 1;
			}else if(argc == 5){ // no transaction to add, only coinbase
				// init the OpenSSL library
				Util::initOpenSSL();
				
				std::string filePath = argv[2];
				std::string privKey = argv[3];
				std::string pubKey = argv[4];
				std::string metadata = ""; // TODO

				// count the number of blocks in the blockchain
				int numBlocks = 0;
				if(FileManager::isFile(filePath + ".blck")){
					for(int i = 0; i < FileManager::getLastLineNum(filePath + ".blck"); i++){
						std::string str = FileManager::readLine(filePath + ".blck", i);
						if(str.find("#") != -1) numBlocks++;
					}
				}


				// first block, dont bother finding the previous one
				if(numBlocks == 0){
					// create a block with id 0
					Block b0 = Block(Util::Hash256(filePath), 0);
					b0.mine(Blockchain::parseBlockchain(filePath).getDifficulty(), privKey, pubKey, Blockchain::parseBlockchain(filePath).getReward(), metadata);
					// add the block after mining it
					Blockchain::parseBlockchain(filePath).addBlock(b0);
					
					// clean the OpenSSL library
					Util::cleanupOpenSSL();
					return 0;
				}
				
				// a prevoius block exists, find it
				Block prev = Block::parseBlock(filePath + ".blck", numBlocks - 1);
				Block b = Block(prev.getCurrHash(), prev.getId() + 1);
				
				// mine the block and add it to the blockchain
				b.mine(Blockchain::parseBlockchain(filePath).getDifficulty(), privKey, pubKey, Blockchain::parseBlockchain(filePath).getReward(), metadata);
				Blockchain::parseBlockchain(filePath).addBlock(b);
				
				// clean the OpenSSL library
				Util::cleanupOpenSSL();
				return 0;
			}else{
				// init the OpenSSL library
				Util::initOpenSSL();

				std::string filePath = argv[2];
				std::string privKey = argv[3];
				std::string pubKey = argv[4];
				std::string metadata = ""; // TODO
				std::vector<std::string> transactions;
				
				if(!FileManager::isFile(filePath + ".utxo") || !FileManager::isFile(filePath + ".txpl") || !FileManager::isFile(filePath + ".blck")){return 1;}
			
				// count the number of blocks in the blockchain
				int numBlocks = 0;
				if(FileManager::isFile(filePath + ".blck")){
					for(int i = 0; i < FileManager::getLastLineNum(filePath + ".blck"); i++){
						std::string str = FileManager::readLine(filePath + ".blck", i);
						if(str.find("#") != -1) numBlocks++;
					}
				}

				Block prev = Block::parseBlock(filePath + ".blck", numBlocks - 1);
				Block b = Block(prev.getCurrHash(), prev.getId() + 1);

				// add all of the tx hash to a vector
				for(int i = 5; i < argc; i++){
					transactions.push_back(argv[i]);					
					printf("added transaction 0x%s to the block\n", argv[i]);
				}
				
				// loop over all hashes and add them from tx pool
				for(std::vector<std::string>::iterator it = transactions.begin(); it != transactions.end(); it++){
					for(int i = 0; i < FileManager::getLastLineNum(filePath + ".txpl"); i++){
						Transaction t = Transaction::parseTransaction(filePath + ".txpl", i);
						if(t.getHash() == *it){b.addTransaction(t);}
					}
				}

				b.mine(Blockchain::parseBlockchain(filePath).getDifficulty(), privKey, pubKey, Blockchain::parseBlockchain(filePath).getReward(), metadata);
				Blockchain::parseBlockchain(filePath).addBlock(b);
				
				// clean the OpenSSL library
				Util::cleanupOpenSSL();
				return 0;
			}
		}else if(argv[1] == std::string("verifyBlock")){
			// syntax: blocky verifyBlock <file path> <block height>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);

			// TODO loop through all of block file, find the block and dump it with metadata
			system("pause");
			return 0;
		}else if(argv[1] == std::string("verifyBlockchain")){
			// syntax: blocky verifyBlockchain <file path>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];

			// TODO loop through entire block file and verify each block, if one is invalid, produce a message
			system("pause");
			return 0;
		}else if(argv[1] == std::string("verifyTransaction")){
			// syntax: blocky verifyTransaction <file path> <transaction hash>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];

			// TODO loop through entire block file and verify each block, if one is invalid, produce a message
			system("pause");
			return 0;
		}
	}

	system("pause");
	return 0; // exit Process
}
