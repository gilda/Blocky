#include "Block.h"

// Constructor only dependant on prevHash
Block::Block(std::string prevHash, int id) {
	// parameters assigning
	this->id = id;
	this->prevHash = prevHash;
	this->nonce = 0;
	this->currHash = "";
	this->numTrans = 0;
	this->transactions = std::vector<Transaction>(0);
	this->transactions.resize(this->numTrans);
	this->mined = false;
}

// parser constructor
Block::Block(int id, std::string prevHash, long long int nonce, std::string currHash, std::vector<Transaction> transactions, int numTrans){
	this->id = id;
	this->prevHash = prevHash;
	this->nonce = nonce;
	this->currHash = currHash;
	this->transactions = transactions;
	this->numTrans = numTrans;
}

// default constructor
Block::Block() {
	this->id = 0;
	this->prevHash = "";
	this->nonce = 0;
	this->currHash = "";
	this->numTrans = 0;
	this->transactions = std::vector<Transaction>(0);
	this->mined = false;
}

// adding a transaction to the Block
void Block::addTransaction(std::string prikey, Transaction transToAdd) {
	if(transToAdd.getSignature()==""){
		transToAdd.sign(prikey);
	}
	this->transactions.push_back(transToAdd);
	this->numTrans++; // increment the number of transactions
}

// returns string for hashing
std::string Block::stringify(){
	std::string rets =
		std::to_string(this->id) + "#" +
		this->prevHash + "|" +
		std::to_string(this->nonce) + "N" +
		std::to_string(this->numTrans) + "T:\n";
	// loop over all transactions
	std::vector<Transaction> trans = this->transactions;
	for (std::vector<Transaction>::iterator it = trans.begin(); it != trans.end(); it++) {
		rets += it->stringify() + ":\n";
	}
	return rets;
}

// returns string for BLCK writing
std::string Block::stringifyBLCK(){
	std::string rets =
		std::to_string(this->id)+"#"+
		this->prevHash+"|"+
		this->currHash+"^"+
		std::to_string(this->nonce)+"N"+
		std::to_string(this->numTrans)+"T:\n";
	// loop over all transactions
	std::vector<Transaction> trans = this->transactions;
	for(std::vector<Transaction>::iterator it = trans.begin(); it!=trans.end(); it++){
		rets += it->stringify()+":\n";
	}
	return rets;
}

// hash the current block and update its currHash
std::string Block::hashBlock() {
	this->currHash = Util::Hash256(this->stringify());
	return this->currHash;
}

// returns string with all parameters for debugging and viewing
std::string Block::toString() {
	std::string rets =
		"id: " + std::to_string(this->id) + "\r\n" +
		"prevHash: " + this->prevHash + "\r\n" +
		"nonce: " + std::to_string(this->nonce) + "\r\n" +
		"mined: " + (this->mined ? "true" : "false") + "\r\n" +
		"currHash: " + this->currHash + "\r\n" +
		"numTrans: " + std::to_string(this->numTrans) + "\r\n\r\n";
	// loop over all transactions
	int index = 0;
	std::vector<Transaction> trans = this->transactions;
	for(std::vector<Transaction>::iterator it = trans.begin(); it != trans.end(); it++){
		rets += std::to_string(index)+": "+it->toString() +"\r\n";
		index++;
	}
	return rets;
}

bool Block::mine(int difficulty, std::string minerPubKey) {
	puts("starting to mine...");
	printf("%s", ("raw data:\r\n" + this->stringify() + "\r\n").c_str());
	// start clock
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (Util::numZeroHash(Util::Hash256(this->stringify()))<difficulty && this->nonce < INT64_MAX) {
		this->nonce++;
		this->hashBlock();
	}
	// end clock
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	this->mined = true;
	long long int eTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	printf("%s", ("elapsed time: "+std::to_string((double)eTime/1000000)+" seconds\r\n\r\n").c_str());
	return this->mined;
}

// returns the last transaction added to the block
Transaction *Block::getLastTransaction() {
	Transaction *ret = &this->transactions.back();
	return ret;
}

// returns transaction at given index
Transaction *Block::getTransaction(int index){
	Transaction *ret = &(this->transactions.at(index));
	return ret;
}

std::vector<Transaction> Block::getTransactionVec(){
	return this->transactions;
}

// returns the block id
int Block::getId() {
	return this->id;
}

// returns NumTrans
int Block::getNumTrans(){
	return this->numTrans;
}

// retutns the hash of the current block
std::string Block::getCurrHash() {
	return this->currHash;
}

// verifies the transaction
int Block::verifyTransaction(Transaction trans, std::string sig, std::string pubKey) {
	if (Crypto::verify(trans.stringifyVerify(), Util::base58Decode(sig), Util::base58Decode(pubKey)) && trans.getSignature()!="") {
		return 1; // success
	}else if(trans.getSignature()!=""){
		return 0; // verify failed
	}else {
		return -1; // error
	}
}

// parse a block from the BLCK
Block Block::parseBlock(std::string file, int id){
	// find correct starting line of block
	int line = 0;
	std::string str = FileManager::readLine(file, line);
	std::string idString = str.substr(0, str.find("#"));
	while(idString != std::to_string(id)){
		str = FileManager::readLine(file, line);
		idString = str.substr(0, str.find("#"));
		line++;
	}

	str = FileManager::readLine(file , line-1);
	std::vector<Transaction> empty(0);

	int numTrans = std::stoi(str.substr(str.find("N")+1, str.find("T")-str.find("N")-1));
	int idParse = std::stoi(str.substr(0, str.find("#")));
	std::string prevHash = str.substr(str.find("#")+1, str.find("#")-str.find("|")-1);
	long long int nonce = std::stoi(str.substr(str.find("^")+1, str.find("N")-str.find("^")-1));
	std::string currHash = str.substr(str.find("|")+1, str.find("^")-str.find("|")-1);

	Block ret = Block(idParse, prevHash, nonce, currHash, empty, 0);

	for(int i = 0; i < numTrans; i++){
		ret.addTransaction("", Transaction::parseTransaction(file, line+i));
	}

	return ret;
}
