#include "stdafx.h"
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
Block::Block(int id, std::string prevHash, long long int nonce, std::string currHash, std::vector<Transaction> transactions, int numTrans, std::string metadata){
	this->id = id;
	this->prevHash = prevHash;
	this->nonce = nonce;
	this->currHash = currHash;
	this->transactions = transactions;
	this->numTrans = numTrans;
	this->metadata = metadata;
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
void Block::addTransaction(std::string file, std::string prikey, std::string donor, int amount, std::string recepient) {
	if(donor!=recepient){
		std::vector<Transaction> input = this->getTransInputForValue(file, donor, amount);
		Transaction transToAdd = Transaction(donor, amount, recepient);
		transToAdd.setInput(input);
		transToAdd.sign(prikey);
		int paid = 0;
		
		for(std::vector<Transaction>::iterator it = input.begin(); it!=input.end(); it++){
			paid += it->getAmount();
		}
		this->transactions.push_back(transToAdd);
		this->numTrans++; // increment the number of transactions

		if(paid-amount!=0){
			// create and add the change transaction
			Transaction change = Transaction(input, "", donor, paid-amount, donor, "");
			change.sign(prikey);
			this->transactions.push_back(change);
			this->numTrans++; // increment the number of transactions
		}
	}
}

// add a new transaction to the block
// does not verify anything because blockchain will when this block will be added to the blockchain after beign mined
void Block::addTransaction(Transaction t){
	this->transactions.push_back(t);
}

// returns string for hashing
std::string Block::stringify(){
	std::string rets =
		std::to_string(this->id) + "#" +
		this->prevHash + "|^" +
		std::to_string(this->nonce) + "N" +
		std::to_string(this->numTrans) + "T:" + 
		this->metadata +"\n";
	// loop over all transactions
	std::vector<Transaction> trans = this->transactions;
	for (std::vector<Transaction>::iterator it = trans.begin(); it != trans.end(); it++) {
		rets += it->stringify() + ":\n";
	}
	rets = rets.substr(0, rets.length() - 1);
	return rets;
}

// returns string for BLCK writing
std::string Block::stringifyBLCK(){
	std::string rets =
		std::to_string(this->id)+"#"+
		this->prevHash+"|"+
		this->currHash+"^"+
		std::to_string(this->nonce)+"N"+
		std::to_string(this->numTrans)+"T:" + 
		this->metadata + "\n";
	// loop over all transactions
	std::vector<Transaction> trans = this->transactions;
	for(std::vector<Transaction>::iterator it = trans.begin(); it!=trans.end(); it++){
		rets += it->stringify()+":\n";
	}
	// remove the last \n from the tx
	rets = rets.substr(0, rets.length() - 1);
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
		"numTrans: " + std::to_string(this->numTrans) + 
		"metadata" + this->metadata + "\r\n\r\n";
	// loop over all transactions
	int index = 0;
	std::vector<Transaction> trans = this->transactions;
	for(std::vector<Transaction>::iterator it = trans.begin(); it != trans.end(); it++){
		rets += std::to_string(index)+": "+it->toString() +"\r\n";
		index++;
	}
	return rets;
}

bool Block::mine(int difficulty, std::string minerPrivKey, std::string minerPubKey, int reward, std::string metadata) {
	// add coinbase
	std::vector<Transaction> inputMine;
	Transaction coinbase = Transaction(minerPubKey, reward, minerPubKey);
	coinbase.sign(minerPrivKey);
	coinbase.setInput(std::vector<Transaction>(1, Transaction(coinbase.getHash(), "", 0, "", "")));
	this->transactions.push_back(coinbase);
	this->numTrans++;

	this->metadata = metadata;
	printf("starting to mine...");
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

std::string Block::getPrevHash(){
	return this->prevHash;
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
	// make sure the file exists
	if(!FileManager::isFile(file)){return Block::Block();}
	// find correct starting line of block
	int line = 0;
	// read the line
	std::string str = FileManager::readLine(file, line);
	std::string idString = str.substr(0, str.find("#"));
	while(idString != std::to_string(id)){
		str = FileManager::readLine(file, line);
		idString = str.substr(0, str.find("#"));
		if(idString != std::to_string(id)) line++;
	}

	str = FileManager::readLine(file, line);
	// parse all parameters using delimeters in BLCK file
	int idParse = std::stoi(str.substr(0, str.find("#")));
	std::string prevHash = str.substr(str.find("#") + 1, str.find("|")-str.find("#")-1);
	long long int nonce = std::stoi(str.substr(str.find("^") + 1, str.find("N") - str.find("^") - 1));
	std::string currHash = str.substr(str.find("|") + 1, str.find("^")-str.find("|") - 1);
	int numTrans = std::stoi(str.substr(str.find("N") + 1, str.find("T") - str.find("N") - 1));
	std::vector<Transaction> empty(0);
	std::string metadata = str.substr(str.find("T") + 2, str.find("\n") - str.find("T") - 2);

	Block ret = Block(idParse, prevHash, nonce, currHash, empty, numTrans, metadata);
	// add all transactions to the block
	for(int i = 0; i < numTrans; i++){
		Transaction t = Transaction::parseTransaction(file, line + i + 1);
		ret.transactions.push_back(t);
	}
	return ret;
}

bool Block::empty(){
	return (this->id==0 && this->prevHash=="" && this->nonce==0 && this->currHash=="" && this->numTrans==0 && this->mined==false && this->transactions.empty());
}

// returns vector of input that a transaction can use
std::vector<Transaction> Block::getTransInputForValue(std::string file, std::string pubKey, int amount){
	std::vector<Transaction> ret;
	if(FileManager::isFile(file + ".utxo")){
		std::string str;
		int found = 0;
		int length = FileManager::getLastLineNum(file + ".utxo");

		// loop over utxo file
		for(int i = 0; i < length && found < amount; i++){
			Transaction trans = Transaction::parseTransaction(file + ".utxo", i);

			// transaction belongs to pubKey
			if(trans.getRecipient() == pubKey){
				found += trans.getAmount();
				ret.push_back(trans); // add to vector the file line index
			}
		}

		return ret;
	}
	return ret;
}

// return the metadata of the block
std::string Block::getMetadata(){
	return this->metadata;
}
