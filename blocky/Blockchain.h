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
	int addTransaction(std::string donor, int amount, std::string recepient);
};
