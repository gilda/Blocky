#pragma once
#include <string>
#include "Block.h"

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
	bool mineLastBlock();
	void addTransaction(std::string privKey, std::string donor, int amount, std::string recepient);
	void addBlock();
	//bool validateBlock(); loops over all transactions and check's signatures, checks block hash
};
