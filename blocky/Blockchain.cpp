#include "Blockchain.h"

//Constructor
Blockchain::Blockchain(std::string filePath, int difficulty, int MAX_TRANS) {
	this->filePath = filePath;
	this->difficulty = difficulty;
	this->MAX_TRANS = MAX_TRANS;
	this->numBlocks = 1;
	// create genesis
	this->createGenesis();

}

void Blockchain::createGenesis() {
	this->blocks =new Block("gilda", 0);
}

Block *Blockchain::getGenesis() {
	return this->blocks;
}

Block *Blockchain::getLastBlock() {
	return this->blocks + this->numBlocks - 1;
}

bool Blockchain::mineLastBlock() {
	return this->getLastBlock()->mine(this->difficulty);
}
