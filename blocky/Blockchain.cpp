#include "Blockchain.h"

//Constructor
Blockchain::Blockchain(std::string filePath, int difficulty, int MAX_TRANS, int reward) {
	this->filePath = filePath;
	this->difficulty = difficulty;
	this->MAX_TRANS = MAX_TRANS;
	this->numBlocks = 1;
	this->reward = reward;
	// create genesis
	this->createGenesis();

}

// creates the first block
void Blockchain::createGenesis() {
	this->blocks = std::vector<Block>(this->numBlocks);
}

// gets the first block
Block *Blockchain::getGenesis() {
	Block *ret = &(this->blocks.front());
	return ret;
}

// returns the index'th block of the blockchain
Block *Blockchain::getBlock(int index){
	Block *ret = &(this->blocks.at(index));
	return ret;
}

// gets the last block
Block *Blockchain::getLastBlock() {
	Block *ret = &(this->blocks.back());
	return ret;
}

// returns the blockchain path
std::string Blockchain::getFilePath(){
	return this->filePath;
}

// mines last block
bool Blockchain::mineLastBlock(std::string minerPrivKey, std::string minerPubKey) {
	// mine it
	bool mine = this->getLastBlock()->mine(this->difficulty, minerPrivKey, minerPubKey, this->reward);
	
	// write into unspent transaction pool
	for(int i = 0; i<this->getLastBlock()->getNumTrans(); i++){
		this->writeTransactionUTXO(i);
	}

	return mine;
}

// adds transaction to the last block
void Blockchain::addTransaction(std::string prikey, std::string donor, int amount, std::string recepient) {
	// cant give yourself money
	if(donor!=recepient){
		Transaction transaction = Transaction(donor, amount, recepient);
		this->getLastBlock()->addTransaction(prikey, transaction);
	}
}

// adds a new empty block to the block chain
void Blockchain::addBlock() {
	this->blocks.push_back(Block());
	this->numBlocks++;
}

// returns true or false for the block TODO
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

// returns pointer to array of tranactions on utxo that can be used by pubkey
std::vector<int> Blockchain::getTransInputForValue(std::string pubKey, int amount){
	std::vector<int> ret;
	if(FileManager::isFile(this->getFilePath()+".utxo")){
		std::string str;
		int found=0;
		int length = FileManager::getLastLineNum(this->getFilePath()+".utxo");

		// loop over utxo file
		for(int i = 0; i<length && found<amount; i++){
			str = FileManager::readLine(this->getFilePath()+".utxo", i);
			Transaction trans = Transaction(str.substr(str.find("[")+1, str.find("]")-str.find("[")-1), std::stoi(str.substr(str.find(">")+1, str.find("<")-str.find(">")-1).c_str()), str.substr(str.find("(")+1, str.find(")")-str.find("(")-1));
			
			// transaction belongs to pubKey
			if(trans.getRecipient()==pubKey){
				found += trans.getAmount();
				ret.push_back(i); // add to vector the file line index
			}
		}

		return ret;
	}
	return ret;
}

// write the index'th transaction from the last block to the UTXO file
void Blockchain::writeTransactionUTXO(int index){
	// if file exists
	if(!FileManager::isFile(this->getFilePath()+".utxo")){
		FileManager::openFile(this->getFilePath()+".utxo");
	}
	FileManager::writeLine(this->getFilePath()+".utxo", this->getLastBlock()->getTransaction(index)->stringify(), FileManager::getLastLineNum(this->getFilePath()+".utxo"));
}
