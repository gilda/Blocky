#pragma once
#include <string>
#include "Block.h"
#include "FileManager.h"

class Blockchain {
	std::string filePath;
	int numBlocks;
	std::vector<Block> blocks;
	int MAX_TRANS;
	int difficulty;
	int reward;

public:

	Blockchain(std::string filePath, int difficulty, int MAX_TRANS, int reward);
	void createGenesis();
	Block *getGenesis();
	Block *getLastBlock();
	Block *getBlock(int index);
	std::string getFilePath();
	bool mineLastBlock(std::string minerPrivKey, std::string minerPubKey);
	void addTransaction(std::string pubkey, std::string donor, int amount, std::string recepient);
	void addBlock();
	bool validateBlock(int index);
	std::vector<Transaction> getTransInputForValue(std::string pubKey, int amount);
	void writeTransactionUTXO(int index);
};
