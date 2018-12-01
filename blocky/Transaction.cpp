#include "stdafx.h"
#include "Transaction.h"

// Constructor
Transaction::Transaction(std::string donor, int amount, std::string recipient) {
	// parameter assigning
	this->hash = "";
	this->donor = donor;
	this->amount = amount;
	this->recipient = recipient;
	this->signature = "";
	this->nonce = Util::random(0, RAND_MAX);
}

//parser constructor
Transaction::Transaction(std::string hash, std::string donor, int amount, std::string recipient, std::string signature){
	this->hash = hash;
	this->donor = donor;
	this->amount = amount;
	this->recipient = recipient;
	this->signature = signature;
	this->nonce = Util::random(0, RAND_MAX);
}

//parser constructor
Transaction::Transaction(std::vector<Transaction> input,std::string hash, std::string donor, int amount, std::string recipient, std::string signature, int nonce){
	this->input = input;
	this->hash = hash;
	this->donor = donor;
	this->amount = amount;
	this->recipient = recipient;
	this->signature = signature;
	this->nonce = nonce;
}

Transaction::Transaction(std::vector<Transaction> input, std::string hash, std::string donor, int amount, std::string recipient, std::string signature){
	this->input = input;
	this->hash = hash;
	this->donor = donor;
	this->amount = amount;
	this->recipient = recipient;
	this->signature = signature;
	this->nonce = Util::random(0, RAND_MAX);
}

// default constructor for arrays
Transaction::Transaction() {
	this->donor = "";
	this->amount = 0;
	this->recipient = "";
	this->nonce = Util::random(0, RAND_MAX);
}

// stringify for hashing and file writing
std::string Transaction::stringify() {
	std::string ret = "";
	for(std::vector<Transaction>::iterator it = this->input.begin(); it!=input.end(); it++){
		ret += it->getHash() + ",";
	}
	ret += "N" + std::to_string(this->nonce) +
		"{HASH" +
		this->hash + "HASH[" +
		this->donor + "]>" +
		std::to_string(this->amount) + "<(" +
		this->recipient + ")SIG" +
		this->signature + "SIG}";
	return ret;
}

// toString prints all info for debugging or viewing
std::string Transaction::toString() {
	std::string ret="INP\n";
	for(std::vector<Transaction>::iterator it = this->input.begin(); it!=input.end(); it++){
		ret += it->getHash()+",\n";
	}

	ret+= 
		"HASH: " + this->hash + "\n" +
		"donor: " + this->donor + "\n"+ 
		"amount: " + std::to_string(this->amount) + "\n" +
		"recepient: " + this->recipient + "\n" +
		"signature: " + this->signature + "\n";
	return ret;
}

// returns the transactions without member signature
std::string Transaction::stringifyVerify() {
	return 
		"N"+std::to_string(this->nonce)+"{["+
		this->donor+"]>"+
		std::to_string(this->amount)+"<("+
		this->recipient+")}";
}


// signs the transaction and updates member signature
int Transaction::sign(std::string privKey) {
	// sign the verified message with secp256 ECDSA
	std::string sign = Crypto::sign(this->stringifyVerify(), Util::base58Decode(privKey)); // sign the stringify of Transaction
	this->signature = Util::base58Encode(sign);
	this->hash = Util::Hash256(this->stringifyVerify());
	return 1;
}

// set the input vector
void Transaction::setInput(std::vector<Transaction> input){
	// set input vector
	this->input = input;
}

// reuturn the signature of this Transaction
std::string Transaction::getSignature() {
	return this->signature;
}

// returns the transactions hash
std::string Transaction::getHash() {
	return this->hash;
}

// returns the donor
std::string Transaction::getDonor(){
	return this->donor;
}

// returns the recepient
std::string Transaction::getRecipient(){
	return this->recipient;
}

// returns the input vector
std::vector<Transaction> Transaction::getInput(){
	return this->input;
}

// returns amount of tokens transacted
int Transaction::getAmount(){
	return this->amount;
}

// parses the transaction from utxo file
Transaction Transaction::parseTransaction(std::string file, int index){
	// read the requested Line
	std::string str = FileManager::readLine(file, index);
	std::vector<Transaction> input;

	// find all input hashes
	std::string hash = str.substr(0, str.find_first_of(","));
	while(hash!=""){
		// insert new hash to vector
		input.push_back(Transaction(hash, "", 0, "", ""));
		str.erase(0, str.find_first_of(",")+1);
		if(str.find_first_of(",")!=-1){
			hash = str.substr(0, str.find_first_of(","));
		}else{
			hash = "";
		}
	}

	// parse using delimiter of utxo file
	Transaction parsed = Transaction(input,
									 str.substr(str.find("{HASH")+5, str.find("HASH[")-str.find("{HASH")-5),
									 str.substr(str.find("[")+1, str.find("]")-str.find("[")-1),
									 std::stoi(str.substr(str.find(">")+1, str.find("<")-str.find(">")-1)),
									 str.substr(str.find("(")+1, str.find(")")-str.find("(")-1),
									 str.substr(str.find(")SIG")+4, str.find("SIG}")-str.find(")SIG")-4),
									 std::stoi(str.substr(str.find("N")+1, str.find("{")-1)));
	return parsed;
}

bool Transaction::empty(){
	return (this->input.empty() && this->hash=="" && this->donor=="" && this->amount==0 && this->recipient=="" && this->signature=="");
}

bool Transaction::operator==(Transaction rhs){
	return (this->donor==rhs.getDonor() && this->hash==rhs.getHash() && this->recipient == this->getRecipient() && this->signature == rhs.getSignature() && this->amount == rhs.getAmount());
}
