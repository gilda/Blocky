#pragma once
#include <string>
#include "Block.h"
#include "FileManager.h"

class Blockchain {
	std::string filePath;
	int numBlocks;
	std::vector<Block> blocks;
	int difficulty;
	int reward;
	unsigned int maxMetadataChar;
	std::string name;

public:

	Blockchain(std::string filePath, int difficulty, int reward, int maxMetadataChar, std::string name);
	Block *getGenesis();
	Block *getLastBlock();
	Block *getBlock(int index);
	int getDifficulty();
	int getReward();
	std::string getName();
	int getMaxMetadataChar();
	std::string getFilePath();
	void addBlock(Block blockToAdd);
	void writeTransactionUTXO(Transaction trans);
	void writeLastBlock();
	Transaction getTransactionByHashUTXO(std::string hash);
	bool validateBlockHashes(Block vBlock);
	bool validateBlockTransactionSig(Block vBlock);
	bool validateLastBlockUTXO(Block vBlock);
	static Blockchain parseBlockchain(std::string filePath);
	static void addToTransactionPool(std::string filePath, std::string privKey, std::string donor, int amount, std::string recepient);
};
