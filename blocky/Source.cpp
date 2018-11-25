#include "stdafx.h"
#include "Crypto.h"
#include "Util.h"
#include "Blockchain.h"
#include "Block.h"
#include "Transaction.h"

int main(int argc, char* argv[]) {
	//TODO ordered by importance
	//TODO COMMENT LAST COMMIT!
	//TODO CLI: TODO get rid of all the system("pause") before release, get rid of weird printf()s
	//TODO Block: TODO insert metadata
	//TODO Blockchain: TODO parseBlockchain test and check, TODO 3 block UTXO bug
	//TODO GUI:
	//TODO Transaction: 
	//TODO Crypto: TODO genPubFromPriv()
	//TODO Util:
	//TODO FileManager:

	/*remove(std::string("gldc.utxo").c_str());
	remove(std::string("gldc.blck").c_str());
	remove(std::string("gldc.meta").c_str());
	Blockchain gldc = Blockchain("gldc", 16, 10, "gilda");

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
			printf("%s", Util::helpText().c_str());
			system("pause");
			return 0;
		}else if(argv[1] == std::string("init")){
			// syntax: blocky init <file path> <difficulty> <reward>
			if(argc < 5){return 1;}
			std::string filePath = argv[2];
			int difficulty = std::stoi(argv[3]);
			int reward= std::stoi(argv[4]);

			// TODO create the folder and the metadata file
			system("pause");
			return 0;
		}else if(argv[1] == std::string("printBlockchainParams")){ // print the init params of some blockchain
			// syntax: blocky printBlockchainParams <filePath>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];

			// TODO possibly use Blockchian.parseBlockchain(filePath);			
			system("pause");
			return 0;
		}else if(argv[1] == std::string("printBlock")){
			// syntax: blocky printBlock <file path> <block height>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			int blockHeight = std::stoi(argv[3]);
			// TODO find block in block file and print the entire block with possibly metadata

			system("pause");
			return 0;
		}else if(argv[1] == std::string("printTransaction")){
			// syntax: blocky printTransaction <file path> <transaction hash>
			if(argc < 4){return 1;}
			std::string filePath = argv[2];
			std::string hash = argv[3];

			// TODO loop through all of blockchain and find the transaction
			system("pause");
			return 0;
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
			
			Util::cleanupOpenSSL();
		}else if(argv[1] == std::string("getBalance")){
			// syntax: blocky getBalance <file path> <address>
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			std::string address = argv[3];

			// loop through all of UTXO file and accumulate balance of address
			system("pause");
			return 0;
		}else if(argv[1] == std::string("sendTransaction")){
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
		}else if(argv[1] == std::string("mineBlock")){
			// syntax: blocky mineBlock <file path> <tranaction hash> [<--metadata>]
			// TODO add option for just line numbers in transaction pool file
			if(argc < 3){return 1;}
			std::string filePath = argv[2];
			
			// TODO loop through all argv[3++], construct block, mine it, and add it to the block file
			system("pause");
			return 0;
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
		}
	}

	system("pause");
	return 0; // exit Process
}
