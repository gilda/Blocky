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

// returns string for hashing and file writing
std::string Block::stringify(){
	std::string rets =
		std::to_string(this->id) + "#" +
		this->prevHash + "|" +
		std::to_string(this->nonce) + "N" +
		std::to_string(this->numTrans) + "T:";
	// loop over all transactions
	for (std::vector<Transaction>::iterator it=this->transactions.begin(); it!=this->transactions.end(); it++) {
		rets += it->stringify() + ":";
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
	for(std::vector<Transaction>::iterator it = this->transactions.begin(); it!=this->transactions.end(); it++){
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
