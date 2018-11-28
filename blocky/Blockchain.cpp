#include "stdafx.h"
#include "Blockchain.h"

//Constructor
Blockchain::Blockchain(std::string filePath, int difficulty, int reward, int maxMetadataChar, std::string name) {
	this->filePath = filePath;
	this->difficulty = difficulty;
	this->numBlocks = 0;
	this->reward = reward;
	this->maxMetadataChar = maxMetadataChar;

	// check whether or not there is a blockchain metadata file
	if(!FileManager::isFile(this->filePath+".meta")){
		FileManager::openFile(this->filePath+".meta");
	}

	// write all of the metadata to the file
	FileManager::writeLine(this->filePath+".meta", name+"{"+std::to_string(this->difficulty)+"}["+std::to_string(this->reward)+"]<" + std::to_string(this->maxMetadataChar) + ">", 0);
	
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

int Blockchain::getDifficulty(){
	return this->difficulty;
}

int Blockchain::getReward(){
	return this->reward;
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
	if(this->validateBlockHashes(blockToAdd) != true){
		printf("hash wrong!\n");
		return;
	}else if(this->validateBlockTransactionSig(blockToAdd) != true){
		printf("tx signatures wrong!\n");
		return;
	}else if(this->validateLastBlockUTXO(blockToAdd) != true){
		printf("last block utxo wrong!\n");
		return;
	}else if(this->maxMetadataChar < blockToAdd.getMetadata().length()){
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
	if(vBlock.getCurrHash()!=Util::Hash256(vBlock.stringify())){
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
		if(it->getDonor()!=it->getRecipient()){
			std::vector<Transaction> input = it->getInput();
			int paid = 0;
			for(std::vector<Transaction>::iterator inputTrans = input.begin(); inputTrans!=input.end(); inputTrans++){
				for(int i = 0; i<FileManager::getLastLineNum(this->getFilePath()+".utxo"); i++){
					Transaction transaction = Transaction::parseTransaction(this->getFilePath()+".utxo", i);
					if(transaction.getHash() == inputTrans->getHash()){
						paid += inputTrans->getAmount();
					}
				}
			}
			if(paid < it->getAmount()){
				return false;
			}
		}else{
			// check if coinbase or change
			std::vector<Transaction> input = it->getInput();
			if(input.size()==1 && input.at(0).getHash()==it->getHash() && it->getAmount()==this->reward){
				// coinbase
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
	// TODO check and test
	// check whether the files exist
	if(!FileManager::isFile(filePath + ".blck") || !FileManager::isFile(filePath + ".utxo") || !FileManager::isFile(filePath + ".meta")){return Blockchain("", 0, 0, 0,"");}
	
	std::string metadata = FileManager::readLine(filePath+".meta", 0);
	Blockchain ret = Blockchain(filePath, std::stoi(metadata.substr(metadata.find("{") + 1, metadata.find("}"))), std::stoi(metadata.substr(metadata.find("[") + 1, metadata.find("]"))), std::stoi(metadata.substr(metadata.find("<") + 1, metadata.find(">"))), metadata.substr(0, metadata.find("{")));
	
	// count the blocks to add to the blockchain
	int numBlocks = 0;
	for(int i = 0; i < FileManager::getLastLineNum(filePath + ".blck"); i++){
		if(FileManager::readLine(filePath + ".blck", i).find("}:\n") != std::string::npos){numBlocks++;}
	}

	// loop over and add all of the parsed blocks
	for(int i = 0; i < numBlocks; i++){
		// make sure the blockcs are valid
		if(ret.validateBlockHashes(Block::parseBlock(filePath + ".blck", i)) != true){
			printf("parsed blockchain is invalid");
			return ret;	
		}else if(ret.validateBlockTransactionSig(Block::parseBlock(filePath + ".blck", i)) != true){
			printf("parsed blockchain is invalid");
			return ret;
		}

		// if valid, add the block and parse the next one
		ret.blocks.push_back(Block::parseBlock(filePath + ".blck", i));
		ret.writeLastBlock();
		ret.numBlocks++;
	}
	// return fully parsed blockchain that was loaded to memory
	return ret;
}