#include "Transaction.h"

// Constructor
Transaction::Transaction(std::string donor, int amount, std::string recipient) {
	// parameter assigning
	this->hash = "";
	this->donor = donor;
	this->amount = amount;
	this->recipient = recipient;
	this->signature = "";
}

// default constructor for arrays
Transaction::Transaction() {
	this->donor = "";
	this->amount = 0;
	this->recipient = "";
}

// stringify for hashing and file writing
std::string Transaction::stringify() {
	return
		"{HASH"+
		this->hash+"HASH["+
		this->donor+"]>"+
		std::to_string(this->amount)+">["+
		this->recipient+"]SIG"+
		this->signature+"SIG}";
}

// toString prints all info for debugging or viewing
std::string Transaction::toString() {
	return 
		"{HASH"+
		this->hash + "HASH[" + 
		this->donor+"] > "+
		std::to_string(this->amount)+" > ["+
		this->recipient + "]SIG" +
		this->signature + "SIG}";
}

// returns the transactions without member signature
std::string Transaction::stringifyVerify() {
	return 
		"{["+
		this->donor+"]>"+
		std::to_string(this->amount)+">["+
		this->recipient+"]}";
}


// signs the transaction and updates member signature
int Transaction::sign(std::string privKey) {
	std::string sign = Crypto::sign(this->stringifyVerify(), privKey); // sign the stringify of Transaction
	this->signature = sign;
	this->hash = Util::Hash256(this->stringifyVerify());
	return 1;
}

// reuturn the signature of this Transaction
std::string Transaction::getSignature() {
	return this->signature;
}

// returns the transactions hash
std::string Transaction::getHash() {
	return this->hash;
}

std::string Transaction::getDonor(){
	return this->donor;
}
