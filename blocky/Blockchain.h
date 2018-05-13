#pragma once
#include <string>
#include "Block.h"
#include "FileManager.h"

class Blockchain {
	std::string filePath;
	int numBlocks;
	Block *blocks;
	int MAX_TRANS;
	int difficulty;

public:

	Blockchain(std::string filePath, int difficulty, int MAX_TRANS);
	void createGenesis();
	Block *getGenesis();
	Block *getLastBlock();
	Block *getBlock(int index);
	std::string getFilePath();
	bool mineLastBlock();
	void addTransaction(std::string pubkey, std::string donor, int amount, std::string recepient);
	void addBlock();
	bool validateBlock(int index);
	void writeTransactionUTXO(int index);
};
