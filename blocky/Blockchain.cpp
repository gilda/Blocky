#include "Blockchain.h"

//Constructor
Blockchain::Blockchain(std::string filePath, int difficulty, int reward) {
	this->filePath = filePath;
	this->difficulty = difficulty;
	this->numBlocks = 1;
	this->reward = reward;
	// create genesis
	this->createGenesis();

}

// creates the first block
void Blockchain::createGenesis() {
	this->blocks = std::vector<Block>(this->numBlocks, Block(Util::Hash256(this->getFilePath()), this->numBlocks-1));
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
	// add coinbase
	std::vector<Transaction> inputMine;
	this->getLastBlock()->addTransaction(minerPrivKey, Transaction(minerPubKey, this->reward, minerPubKey));
	inputMine.push_back(Transaction(this->getLastBlock()->getLastTransaction()->getHash(), "", 0, "", ""));
	this->getLastBlock()->getLastTransaction()->setInput(inputMine);
	
	// mine it
	bool mine = this->getLastBlock()->mine(this->difficulty, minerPubKey);
	
	// delete unspent transaction pool
	if(this->getLastBlock()->getId() != 0){
		std::vector<Transaction> trans = this->getLastBlock()->getTransactionVec();
		for(std::vector<Transaction>::iterator it = trans.begin(); it!=trans.end(); it++){

			for(int i = 0; i<FileManager::getLastLineNum(this->getFilePath()+".utxo"); i++){
				std::string str = FileManager::readLine(this->getFilePath()+".utxo", i);
				Transaction utxo = Transaction(str.substr(str.find("{HASH")+5, str.find("HASH[")-str.find("{HASH")-5),
					str.substr(str.find("[")+1, str.find("]")-str.find("[")-1),
					std::stoi(str.substr(str.find(">")+1, str.find("<")-str.find(">")-1).c_str()),
					str.substr(str.find("(")+1, str.find(")")-str.find("(")-1),
					str.substr(str.find(")SIG")+4, str.find("SIG}")-str.find(")SIG")-4));
				std::vector<Transaction> input = it->getInput();
					
				for(std::vector<Transaction>::iterator inpt = input.begin(); inpt != input.end(); inpt++){
					if(utxo.getHash() == inpt->getHash()){
						FileManager::deleteLine(this->getFilePath()+".utxo", i);
						i--;
					}
				}
			}
		}
	}

	// write into unspent transaction pool
	for(int i = 0; i<this->getLastBlock()->getNumTrans(); i++){
		this->writeTransactionUTXO(i);
	}

	// write new block to blockchain file
	this->writeLastBlock();

	// add new block
	this->blocks.push_back(Block(this->getLastBlock()->getCurrHash(), this->getLastBlock()->getId()+1));

	return mine;
}

// adds transaction to the last block
void Blockchain::addTransaction(std::string prikey, std::string donor, int amount, std::string recepient) {
	// cant give yourself money
	if(donor!=recepient){
		std::vector<Transaction> input = this->getTransInputForValue(donor, amount);
		Transaction transaction = Transaction(input, "", donor, amount, recepient, "");
		int paid = 0;

		for(std::vector<Transaction>::iterator it = input.begin(); it != input.end(); it++){
			paid += it->getAmount();
		}
		this->getLastBlock()->addTransaction(prikey, transaction);
		if(paid-amount!=0){
			std::vector<Transaction> changeInput;
			changeInput.push_back((Transaction(this->getLastBlock()->getLastTransaction()->getHash(), "", 0, "", "")));
			this->getLastBlock()->addTransaction(prikey, Transaction(changeInput, "", donor, paid-amount, donor, ""));
		}
	}
}

// adds a new empty block to the block chain
void Blockchain::addBlock() {
	this->blocks.push_back(Block());
	this->numBlocks++;
}

// returns true or false for the block TODO
bool Blockchain::validateBlock(int index){
	return false;
}

// returns pointer to array of tranactions on utxo that can be used by pubkey
std::vector<Transaction> Blockchain::getTransInputForValue(std::string pubKey, int amount){
	std::vector<Transaction> ret;
	if(FileManager::isFile(this->getFilePath()+".utxo")){
		std::string str;
		int found=0;
		int length = FileManager::getLastLineNum(this->getFilePath()+".utxo");

		// loop over utxo file
		for(int i = 0; i<length && found<amount; i++){
			str = FileManager::readLine(this->getFilePath()+".utxo", i);
			Transaction trans = Transaction(str.substr(str.find("{HASH")+5, str.find("HASH[")-str.find("{HASH")-5),
											str.substr(str.find("[")+1, str.find("]")-str.find("[")-1),
											std::stoi(str.substr(str.find(">")+1, str.find("<")-str.find(">")-1).c_str()),
											str.substr(str.find("(")+1, str.find(")")-str.find("(")-1),
											str.substr(str.find(")SIG")+4, str.find("SIG}")-str.find(")SIG")-4));
			
			// transaction belongs to pubKey
			if(trans.getRecipient()==pubKey){
				found += trans.getAmount();
				ret.push_back(trans); // add to vector the file line index
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

void Blockchain::writeLastBlock(){
	if(!FileManager::isFile(this->getFilePath()+".blck")){
		FileManager::openFile(this->getFilePath()+".blck");
	}

	FileManager::writeLine(this->getFilePath()+".blck", this->getLastBlock()->stringifyBLCK(), FileManager::getLastLineNum(this->getFilePath()+".blck"));
}

Transaction Blockchain::getTransactionByHashUTXO(std::string hash){
	std::string str;
	Transaction trans;
	int line = 0;
	while(hash!=trans.getHash() && line <= FileManager::getLastLineNum(this->getFilePath()+".utxo")){
		str = FileManager::readLine(this->getFilePath()+".utxo", line);

		trans = Transaction(str.substr(str.find("{HASH")+5, str.find("HASH[")-str.find("{HASH")-5),
			str.substr(str.find("[")+1, str.find("]")-str.find("[")-1),
			std::stoi(str.substr(str.find(">")+1, str.find("<")-str.find(">")-1).c_str()),
			str.substr(str.find("(")+1, str.find(")")-str.find("(")-1),
			str.substr(str.find(")SIG")+4, str.find("SIG}")-str.find(")SIG")-4));
		line++;
	}

	if(line > FileManager::getLastLineNum(this->getFilePath()+".utxo") + 1){
		return Transaction();
	}

	return trans;
}


Block Blockchain::parseBlock(int id){
	Block ret = Block();
	return ret;
}


bool Blockchain::validateBlock(Block vBlock){
	if(vBlock.stringify().substr(vBlock.stringify().find("#")+1, vBlock.stringify().find("|")-vBlock.stringify().find("#")-1)!=Util::Hash256(vBlock.stringify())){
		return false;
	}
	return true;
}
