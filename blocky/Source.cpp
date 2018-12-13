#include "stdafx.h"
#include "Crypto.h"
#include "Util.h"
#include "Blockchain.h"
#include "Block.h"
#include "Transaction.h"

int main(int argc, char* argv[]) {
	//CLI: TODO get rid of all the system("pause") before release, TODO get rid of weird printf()s, TODO change return 1s to usefull error messages, TODO sanitize inputs, TODO assert upper limits on argc, TODO test verification of tx blck blckchn
	//Block: TODO think about verifyBlock verifying the shit out of the block
	//Blockchain: TODO 3 block UTXO bug
	//Crypto: TODO format hex properly (0x and low caps)
	//GUI:
	//Transaction: TODO think about verifyTransaction verifying the shit out of the Transaction
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
			// syntax: blocky printBlock <file path> <block height> [--metadata]
			
			// search for the --metadata option
			bool metadata = false;
			for(int i = 0; i < argc; i++){
				if(argv[i] == std::string("--metadata")){
					metadata = !metadata;
					break;
				}
			}

			// make sure there are enough arguments
			if(argc < 4 && !metadata){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);

			// check if file does not exist
			if(!FileManager::isFile(filePath + ".blck")){return 1;}

			// make sure block exists in the file
			if(Block::parseBlock(filePath + ".blck", blockHeight).empty()){return 1;}
			
			// find block in block file and print the entire block with metadata
			printf("%s\n", Block::parseBlock(filePath + ".blck", blockHeight).toString(metadata).c_str());
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
			Util::initOpenSSL();
			
			// gen new key
			if(argc == 2){
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
			}else{
				// derive from private key
				printf("the corresponding public key for this private key is: %s\n", Util::base58Encode(Crypto::derivePublicFromPrivate(Util::base58Decode(argv[2]))).c_str());
				Util::cleanupOpenSSL();
				return 0;
			}
		}else if(argv[1] == std::string("getBalance")){
			// syntax: blocky getBalance <file path> <address>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			std::string address = argv[3];

			// make sure the file exists
			if(!FileManager::isFile(filePath + ".utxo")){return 1;}

			int balance = 0;
			// loop through all of UTXO file and accumulate balance of address
			for(int i = 0; i < FileManager::getLastLineNum(filePath + ".utxo"); i++){
				Transaction t = Transaction::parseTransaction(filePath + ".utxo", i);
				// same address add the values
				if(t.getRecipient() == address){
					balance += t.getAmount();
				}
			}

			// output the address
			printf("the account %s has %d %s in it's balance\n", address.c_str(), balance, Blockchain::parseBlockchain(filePath).getName().c_str());
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
			if(argc < 5){ // not enough parameters
				return 1;
			}else if(argc == 5){ // no transaction to add, only coinbase
				// init the OpenSSL library
				Util::initOpenSSL();
				
				std::string filePath = argv[2];
				std::string privKey = argv[3];
				std::string pubKey = argv[4];
				std::string metadata = ""; // no metadata to add by the number of arguments

				// make sure blockchain parameters were initialized
				if(!FileManager::isFile(filePath + ".meta")){return 1;}

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
				std::string metadata = "";
				std::vector<std::string> transactions;

				// make sure blockchain params were initialized
				if(!FileManager::isFile(filePath + ".meta")){return 1;}
				
				// find the metadata argument
				for(int i = 0; i < argc; i++){
					if(argv[i] == std::string("--metadata")){
						metadata = argv[i + 1];
						break;
					}
				}

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

				if(!FileManager::isFile(filePath + ".utxo") || !FileManager::isFile(filePath + ".txpl") || !FileManager::isFile(filePath + ".blck")){return 1;}

				Block prev = Block::parseBlock(filePath + ".blck", numBlocks - 1);
				Block b = Block(prev.getCurrHash(), prev.getId() + 1);

				// add all of the tx hash to a vector of string type
				for(int i = 5; i < argc; i++){
					transactions.push_back(argv[i]);					
				}
				
				// loop over all hashes and add them from tx pool
				for(std::vector<std::string>::iterator it = transactions.begin(); it != transactions.end(); it++){
					for(int i = 0; i < FileManager::getLastLineNum(filePath + ".txpl"); i++){
						Transaction t = Transaction::parseTransaction(filePath + ".txpl", i);
						if(t.getHash() == *it){
							b.addTransaction(t);
							printf("added transaction 0x%s to the block\n", t.getHash().c_str());
							// remove the line from the tx pool
							FileManager::deleteLine(filePath + ".txpl", i);
						}
					}
				}

				// mine and add the block to the chain
				b.mine(Blockchain::parseBlockchain(filePath).getDifficulty(), privKey, pubKey, Blockchain::parseBlockchain(filePath).getReward(), metadata);
				Blockchain::parseBlockchain(filePath).addBlock(b);
				
				// clean the OpenSSL library
				Util::cleanupOpenSSL();
				return 0;
			}
		}else if(argv[1] == std::string("verifyTransaction")){
			// syntax: blocky verifyTransaction <file path> <transaction hash> <--txpool, --utxo>
			if(argc != 5){return 1;}
			std::string filePath = argv[2];
			std::string file;

			// make sure a file is specified and get the file extension right
			if(argv[4] == std::string("--txpool")){file = ".txpl";
			}else if(argv[4] == std::string("--utxo")){file = ".utxo";
			}else{return 1;}

			// make sure the specified file exists
			if(!FileManager::isFile(filePath + file)){return 1;}

			for(int i = 0; i < FileManager::getLastLineNum(filePath + file); i++){
				Transaction t = Transaction::parseTransaction(filePath + file, i);
				// same hash, same transaction, verify it
				if(t.getHash() == argv[3]){
					
					// verify hash
					if(t.getHash() != Util::Hash256(t.stringifyVerify())){
						printf("the transaction hash is worng! 0x%s != 0x%s\n", t.getHash().c_str(), Util::Hash256(t.stringifyVerify()).c_str());
						return 0;
					}

					// verify the transaction signature
					if(Crypto::verify(t.stringifyVerify(), Util::base58Decode(t.getSignature()), Util::base58Decode(t.getDonor())) != 1){
						printf("the transaction signature is wrong!\n");
						return 0;
					}

					// transaction is completely valid
					printf("the transaction 0x%s is valid!\n", t.getHash().c_str());
					return 0;
				}
			}
			// no transaction was found
			printf("didn't find any 0x%s transaction in %s%s file", std::string(argv[3]).c_str(), filePath.c_str(), file.c_str());
			return 0;
		}else if(argv[1] == std::string("verifyBlock")){
			// syntax: blocky verifyBlock <file path> <block height>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);
			
			
			// count the number of blocks in the blockchain
			int numBlocks = 0;
			if(FileManager::isFile(filePath + ".blck")){
				for(int i = 0; i < FileManager::getLastLineNum(filePath + ".blck"); i++){
					std::string str = FileManager::readLine(filePath + ".blck", i);
					if(str.find("#") != -1) numBlocks++;
				}
			}

			// verify each block hash and transaction inside
			Block b = Block::parseBlock(filePath + ".blck", blockHeight);
				
			// verify previous block hash
			if(blockHeight != 0){
				Block prev = Block::parseBlock(filePath + ".blck", blockHeight - 1);
				if(prev.getCurrHash() != b.getPrevHash()){
					printf("block #%d's previous hash does not match block #%d's hash! 0x%s != 0x%s\n", blockHeight, blockHeight - 1, b.getPrevHash().c_str(), prev.getCurrHash().c_str());
				}
			}

			// verify current block hash
			if(b.getCurrHash() != Util::Hash256(b.stringify())){
				printf("block #%d's hash does not match its curent hash! 0x%s != 0x%s\n", blockHeight, b.getCurrHash().c_str(), Util::Hash256(b.stringify()).c_str());
				return 0;
			}
			
			// verify each transaction
			for(int i = 0; i < b.getNumTrans(); i++){
				Transaction t = *b.getTransaction(i);
				// verify transaction hash
				if(t.getHash() != Util::Hash256(t.stringifyVerify())){
					printf("transaction 0x%s of block #%d's hash is invalid! 0x%s != 0x%s\n", t.getHash().c_str(), blockHeight, t.getHash().c_str(), Util::Hash256(t.stringifyVerify()).c_str());
					return 0;
				}

				// verify transaction signature
				if(Crypto::verify(t.stringifyVerify(), Util::base58Decode(t.getSignature()), Util::base58Decode(t.getDonor())) != 1){
					printf("transaction 0x%s of block #%d's signature is invalid!\n", t.getHash().c_str(), blockHeight);
					return 0;
				}
			}

			// the whole block is completly valid
			printf("block #%d is valid!\n", blockHeight);
			return 0;
		}else if(argv[1] == std::string("verifyBlockchain")){
			// syntax: blocky verifyBlockchain <file path>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			
			// make sure all necesseray files exist
			if(!FileManager::isFile(filePath + ".meta") || !FileManager::isFile(filePath + ".blck") || !FileManager::isFile(filePath + ".utxo")){return 1;}

			Blockchain bc = Blockchain::parseBlockchain(filePath);
			Blockchain bv = Blockchain::Blockchain(filePath + "fake", bc.getDifficulty(), bc.getReward(), bc.getMaxMetadataChar(), bc.getName());

			// count the number of blocks in the blockchain
			int numBlocks = 0;
			if(FileManager::isFile(filePath + ".blck")){
				for(int i = 0; i < FileManager::getLastLineNum(filePath + ".blck"); i++){
					std::string str = FileManager::readLine(filePath + ".blck", i);
					if(str.find("#") != -1) numBlocks++;
				}
			}

			// verify the blocks and add them to fake blockchain
			for(int i = 0; i < numBlocks; i++){
				bv.addBlock(Block::parseBlock(filePath + ".blck", i));
			}

			// verify that the utxo is the same
			for(int i = 0; i < FileManager::getLastLineNum(filePath + ".utxo"); i++){
				if(FileManager::readLine(filePath + ".utxo", i) != FileManager::readLine(filePath + "fake.utxo", i)){
					printf("utxo output from blockchain processing is invalid!\n");

					// remove fake files
					remove(std::string(filePath + "fake.meta").c_str());
					remove(std::string(filePath + "fake.blck").c_str());
					remove(std::string(filePath + "fake.utxo").c_str());
					return 0;
				}
			}

			// remove fake files
			remove(std::string(filePath + "fake.meta").c_str());
			remove(std::string(filePath + "fake.blck").c_str());
			remove(std::string(filePath + "fake.utxo").c_str());

			printf("blockchain %s is completly valid!\n", Blockchain::parseBlockchain(filePath).getName().c_str());
			return 0;
		}
	}

	// no arguments given, display help text
	printf("no arguments given\n%s", Util::helpText().c_str());
	return 0; // exit Process
}
