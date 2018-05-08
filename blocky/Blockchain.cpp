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

// returns the index'th block of the blockchain
Block *Blockchain::getBlock(int index){
	return this->blocks+index;
}

// gets the last block
Block *Blockchain::getLastBlock() {
	return this->blocks + this->numBlocks - 1;
}

// returns the blockchain path
std::string Blockchain::getFilePath(){
	return this->filePath;
}

// mines last block
bool Blockchain::mineLastBlock() {
	return this->getLastBlock()->mine(this->difficulty);
}

// adds transaction to the last block
void Blockchain::addTransaction(std::string privKey, std::string donor, int amount, std::string recepient) {
	Transaction transaction = Transaction(donor, amount, recepient);
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

// returns true or false for the block
bool Blockchain::validateBlock(int index){
	if(this->getBlock(index)->getCurrHash()!=Util::Hash256(this->getBlock(index)->stringify())){
		return false;
	} else {
		for(int i = 0; i<this->getBlock(index)->getNumTrans(); i++){
			if(this->getBlock(index)->verifyTransaction(*this->getBlock(index)->getTransaction(i),
														this->getBlock(index)->getTransaction(i)->getSignature(),
														this->getBlock(index)->getTransaction(i)->getDonor())!=1){
				return false;
			}
		}
		return true;
	}
}

// write the index'th transaction from the last block to the UTXO file
void Blockchain::writeTransactionUTXO(int index){
	FileManager::writeLine(this->getFilePath()+".utxo", this->getLastBlock()->getTransaction(index)->stringify(), 0);
}
