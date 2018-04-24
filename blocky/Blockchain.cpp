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

// creates the first block
void Blockchain::createGenesis() {
	this->blocks = new Block(Util::Hash256(this->filePath), 0);
}

// gets the first block
Block *Blockchain::getGenesis() {
	return this->blocks;
}

// gets the last block
Block *Blockchain::getLastBlock() {
	return this->blocks + this->numBlocks - 1;
}

// mines last block
bool Blockchain::mineLastBlock() {
	return this->getLastBlock()->mine(this->difficulty);
}

// adds transaction to the last block
void Blockchain::addTransaction(std::string privKey, std::string donor, int amount, std::string recepient) {
	Transaction transaction = Transaction(this->getLastBlock()->getNumTrans() + 1, donor, amount, recepient);
	this->getLastBlock()->addTransaction(transaction, privKey);
}

// adds a new block to the block chain
void Blockchain::addBlock() {
	Block *temp = new Block[this->numBlocks+1];
	for(int i = 0; i < this->numBlocks; i++) {
		*(temp + i) = *(this->blocks + i);
	}
	*(temp + this->numBlocks) = Block(this->getLastBlock()->getCurrHash(), this->getLastBlock()->getId()+1);
	this->blocks = temp;
	this->numBlocks++;
}
