#include <stdio.h>
#include "Node.h"
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"
#include "FileManager.h"

int main() {
	//TODO ordered by importance
	//TODO COMMENT LAST COMMIT!
	//TODO Network: send & recv thread style, Kad DHT, Bootstrap, ping pong, gossip
	//TODO CLI: 
	//TODO Block: 
	//TODO Blockchain: 
	//TODO GUI:
	//TODO Transaction: 
	//TODO Crypto: 
	//TODO Util: 
	//TODO FileManager:

	Util::initOpenSSL(); // init OpenSSL
	Network::initNet(); // init WSA
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

	Node a = Node(Crypto::genKey(), 4590);
	printf("\n%s\n\n", a.toString().c_str());

	a.connectToServer("127.0.0.1", 4591);
	a.netSend("Hello this is the client socket", false);

	Network::cleanUpNet(); // CleanUp WSA
	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
