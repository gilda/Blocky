#include "stdafx.h"
#include "Blockchain.h"

//Constructor
Blockchain::Blockchain(std::string filePath, int difficulty, int reward, int maxMetadataChar, std::string name) {
	this->filePath = filePath;
	this->difficulty = difficulty;
	this->numBlocks = 0;
	this->reward = reward;
	this->maxMetadataChar = maxMetadataChar;
	this->name = name;

	// check whether or not there is a blockchain metadata file
	if(!FileManager::isFile(this->filePath+".meta") && filePath != ""){
		FileManager::openFile(this->filePath+".meta");
	}

	// write all of the metadata to the file
	FileManager::writeLine(this->filePath+".meta", this->name+"{"+std::to_string(this->difficulty)+"}["+std::to_string(this->reward)+"]<" + std::to_string(this->maxMetadataChar) + ">", 0);
	
	// create genesis block
	this->blocks = std::vector<Block>(0);

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

// returns the mining diffculty of this blockchain
int Blockchain::getDifficulty(){
	return this->difficulty;
}

// returns the mining reward for this blockchain
int Blockchain::getReward(){
	return this->reward;
}

// return the name for this blockchain
std::string Blockchain::getName(){
	return this->name;
}

// returns the max metadata char count allowed for blocks in this blockchain
int Blockchain::getMaxMetadataChar(){
	return this->maxMetadataChar;
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

// adds a new empty block to the block chain
void Blockchain::addBlock(Block blockToAdd) {
	if(!this->validateBlockHashes(blockToAdd)){
		printf("cant add block, hash wrong!\n");
		return;
	}else if(!this->validateBlockTransactionSig(blockToAdd)){
		printf("cand add block, transactions signatures wrong!\n");
		return;
	}else if(!this->validateLastBlockUTXO(blockToAdd)){
		printf("cant add block, last block utxo wrong!\n");
		return;
	}else if(this->maxMetadataChar < blockToAdd.getMetadata().length()){
		printf("cant add block, maximum block metadata char count was exceeded!\n");
		return;
	}
	
	this->blocks.push_back(blockToAdd);
	this->writeLastBlock();
	this->numBlocks++;

	// delete all inputs from utxo file
	std::vector<Transaction> transactions = blockToAdd.getTransactionVec();
	for(std::vector<Transaction>::iterator it = transactions.begin(); it!=transactions.end(); it++){
		if(blockToAdd.getId() != 0){
			std::vector<Transaction> input = it->getInput();
			for(std::vector<Transaction>::iterator inputIt = input.begin(); inputIt!=input.end(); inputIt++){
				for(int i = 0; i<FileManager::getLastLineNum(this->getFilePath()+".utxo"); i++){
					Transaction inputTrans = Transaction::parseTransaction(this->getFilePath()+".utxo", i);
					if(inputTrans.getHash()==inputIt->getHash()){
						FileManager::deleteLine(this->getFilePath()+".utxo", i);
					}
				}
			}
		}
		this->writeTransactionUTXO(*it);
	}
}

// write the index'th transaction from the last block to the UTXO file
void Blockchain::writeTransactionUTXO(Transaction trans){
	// if file exists
	if(!FileManager::isFile(this->getFilePath()+".utxo")){
		FileManager::openFile(this->getFilePath()+".utxo");
	}
	// write the UTXO for Transaction stringify
	FileManager::writeLine(this->getFilePath()+".utxo", trans.stringify(), FileManager::getLastLineNum(this->getFilePath()+".utxo"));
}

void Blockchain::writeLastBlock(){
	if(!FileManager::isFile(this->getFilePath()+".blck")){
		FileManager::openFile(this->getFilePath()+".blck");
	}
	// write the block BLCK format
	FileManager::writeLine(this->getFilePath()+".blck", this->getLastBlock()->stringifyBLCK(), FileManager::getLastLineNum(this->getFilePath()+".blck"));
}

// returns a transaction given the hash of it in UTXO format
Transaction Blockchain::getTransactionByHashUTXO(std::string hash){
	Transaction trans;
	int line = 0;
	// loop over file to find the hash of the transaction
	while(hash!=trans.getHash() && line <= FileManager::getLastLineNum(this->getFilePath()+".utxo")){
		trans = Transaction::parseTransaction(this->getFilePath()+".utxo", line);
		line++;
	}

	// return empty transaction if not found
	if(line > FileManager::getLastLineNum(this->getFilePath()+".utxo")){
		return Transaction();
	}

	return trans;
}

// validate that all the hashes in the block are valid and correct
bool Blockchain::validateBlockHashes(Block vBlock){
	// hash is not correct
	if(vBlock.getCurrHash() != Util::Hash256(vBlock.stringify())){
		printf("current hash of block is invalid!\n");
		return false;
	}
	// block or last block were not mined properly
	if(Util::numZeroHash(vBlock.getCurrHash()) < this->difficulty){
		return false;
	}
	// check last block hash
	if(vBlock.getId() != 0){
		if(Block::parseBlock(this->getFilePath()+".blck", vBlock.getId()-1).getCurrHash()!=vBlock.getPrevHash()){ return false;}
	}
	// all transaction hashes are correct
	std::vector<Transaction> trans = vBlock.getTransactionVec();
	for(std::vector<Transaction>::iterator it = trans.begin(); it != trans.end(); it++){
		// hash transaction
		if(it->getHash() != Util::Hash256(it->stringifyVerify())){
			return false;
		}
	}
	return true;
}

// validate that all signatures in the block are valid and correct
bool Blockchain::validateBlockTransactionSig(Block vBlock){
	std::vector<Transaction> trans = vBlock.getTransactionVec();
	// loop over all transactions
	for(std::vector<Transaction>::iterator it = trans.begin(); it!=trans.end(); it++){
		if(Crypto::verify(it->stringifyVerify(), Util::base58Decode(it->getSignature()), Util::base58Decode(it->getDonor())) != 1){
			return false;
		}
	}
	return true;
}

// checks the incoming block's utxo use
bool Blockchain::validateLastBlockUTXO(Block vBlock){
	std::vector<Transaction> trans = vBlock.getTransactionVec();
	// each transactiom
	for(std::vector<Transaction>::iterator it = trans.begin(); it!=trans.end(); it++){
		// payment transaction
		if(it->getDonor() != it->getRecipient()){
			std::vector<Transaction> input = it->getInput();
			int paid = 0;
			// find all of the inputs for the given transaction
			for(std::vector<Transaction>::iterator inputTrans = input.begin(); inputTrans!=input.end(); inputTrans++){
				for(int i = 0; i < FileManager::getLastLineNum(this->getFilePath() + ".utxo"); i++){
					Transaction transaction = Transaction::parseTransaction(this->getFilePath() + ".utxo", i);
					// make sure the hash is the same and add the value to the paid
					if(transaction.getHash() == inputTrans->getHash()){
						paid += transaction.getAmount();
					}
				}
			}

			// after finding all inputs, make sure paid is larger than the sum
			if(paid < it->getAmount()){
				return false;
			}
		}else{
			// check if coinbase or change
			std::vector<Transaction> input = it->getInput();
			if(input.size() == 1 && input.at(0).getHash() == it->getHash() && it->getAmount() == this->reward){
				// coinbase has only one input and the reward is the same as the blockchain reward
				continue;
			}else{
				// change loop over all transactions and find twin
				// check that twin.getAmount() + it->getAmount() < paid
				int paid = 0;
				for(std::vector<Transaction>::iterator inputTrans = input.begin(); inputTrans!=input.end(); inputTrans++){
					for(int i = 0; i<FileManager::getLastLineNum(this->getFilePath()+".utxo"); i++){
						Transaction transaction = Transaction::parseTransaction(this->getFilePath()+".utxo", i);
						if(transaction.getHash()==inputTrans->getHash()){
							paid += inputTrans->getAmount();
						}
					}
				}
				int got = 0;
				for(std::vector<Transaction>::iterator twin = trans.begin(); twin!=trans.end(); twin++){
					
					if(twin->getInput().at(0).getHash()==it->getInput().at(0).getHash()){
						std::vector<Transaction> twinInput = twin->getInput();
						
						for(std::vector<Transaction>::iterator twinInpt = twinInput.begin(); twinInpt!=twinInput.end(); twinInpt++){
							
							for(int i = 0; i<FileManager::getLastLineNum(this->getFilePath()+".utxo"); i++){
								
								Transaction transaction = Transaction::parseTransaction(this->getFilePath()+".utxo", i);
								if(transaction.getHash()==twinInpt->getHash()){
									got += twinInpt->getAmount();
								}
							}
						}
						break;
					}
				}
				if(got!=paid){
					return false;
				}
			}
		}
	}
	return true;
}

Blockchain Blockchain::parseBlockchain(std::string filePath){
	// check and parse from the file
	Blockchain ret = Blockchain("", 0, 0, 0, "");
	if(FileManager::isFile(filePath + ".meta")){
		std::string line = FileManager::readLine(filePath + ".meta", 0);
		std::string name = line.substr(0, line.find("{"));
		int difficulty = std::stoi(line.substr(line.find("{") + 1, line.find("}") - line.find("{") - 1));
		int reward = std::stoi(line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1));
		int maxMetadataChar = std::stoi(line.substr(line.find("<") + 1, line.find(">") - line.find("<") - 1));
		ret = Blockchain(filePath, difficulty, reward, maxMetadataChar, name);
	}

	// add the blocks and utxo if any exist
	if(!FileManager::isFile(filePath + ".blck") || !FileManager::isFile(filePath + ".utxo")){return ret;}
	
	// count the blocks to add to the blockchain
	int numBlocks = 0;
	for(int i = 0; i < FileManager::getLastLineNum(filePath + ".blck"); i++){
		std::string str = FileManager::readLine(filePath + ".blck", i);
		if(str.find("#") != -1) numBlocks++;
	}

	// loop over and add all of the parsed blocks
	for(int i = 0; i < numBlocks; i++){
		// make sure the blockcs are valid
		if(!ret.validateBlockHashes(Block::parseBlock(filePath + ".blck", i))){
			printf("parsed blockchain is invalid (hash error)\n");
			return ret;	
		}else if(!ret.validateBlockTransactionSig(Block::parseBlock(filePath + ".blck", i))){
			printf("parsed blockchain is invalid (transaction sig error)\n");
			return ret;
		}else if(Block::parseBlock(filePath + ".blck", i).getMetadata().length() > ret.maxMetadataChar){
			printf("parsed blockchain is invalid (metadata length exceeded)\n");
			return ret;
		}

		// if valid, add the block and parse the next one
		ret.blocks.push_back(Block::parseBlock(filePath + ".blck", i));
		ret.numBlocks++;
	}
	// return fully parsed blockchain that was loaded to memory
	return ret;
}

// add the transaction to the transaction pool
void Blockchain::addToTransactionPool(std::string filePath, std::string privKey, std::string donor, int amount, std::string recepient){
	// create the transaction and sign it
	Transaction t = Transaction(donor, amount, recepient);
	t.sign(privKey);

	// find transaction input for the transaction, throw if non was found
	t.setInput(Block::getTransInputForValue(filePath, donor, amount));
	if(t.getInput().size() == 0){
		printf("no valid input for this transaction!\n");
		return;
	}
	
	// find the amount of input that was found
	std::string str;
	int found = 0;
	int length = FileManager::getLastLineNum(filePath + ".utxo");

	// loop over utxo file
	for(int i = 0; i < length && found < amount; i++){
		Transaction trans = Transaction::parseTransaction(filePath + ".utxo", i);

		// transaction belongs to pubKey
		if(trans.getRecipient() == donor){
			found += trans.getAmount();
		}
	}

	// create and add the change transaction
	Transaction change = Transaction(t.getInput(), "", donor, found - amount, donor, "");
	change.sign(privKey);
	
	// open a new txpool if the file does not exist
	if(!FileManager::isFile(filePath + ".txpl")){FileManager::openFile(filePath + ".txpl");}

	// add to the transaction pool the new transaction
	FileManager::writeLine(filePath + ".txpl", t.stringify(), FileManager::getLastLineNum(filePath + ".txpl"));
	FileManager::writeLine(filePath + ".txpl", change.stringify(), FileManager::getLastLineNum(filePath + ".txpl"));
	printf("added transaction %s to the transaction pool successfully with change transaction %s!\n", t.getHash().c_str(), change.getHash().c_str());
}