#include "Transaction.h"

// Constructor
Transaction::Transaction(int id, std::string donor, int amount, std::string recipient) {
	// parameter assigning
	this->id = id;
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
		std::to_string(this->id) + "#"
		+ this->donor + "->"
		+ std::to_string(this->amount) + "->"
		+ this->recipient + "&"
		+ this->signature + "&";
}

// toString prints all info for debugging or viewing
std::string Transaction::toString() {
	return 
		"{"+
		std::to_string(this->id) + "#" + 
		this->donor+" -> "+
		std::to_string(this->amount)+" -> "+
		this->recipient + "&" +
		this->signature + "&}";
}

// returns the transactions without member signature
std::string Transaction::stringifyVerify() {
	return
		std::to_string(this->id) + "#"
		+ this->donor + "->"
		+ std::to_string(this->amount) + "->"
		+ this->recipient + "&&";
}


// signs the transaction and updates member signature
int Transaction::sign(std::string privKey) {
	std::string sign = Crypto::sign(this->stringifyVerify(), privKey); // sign the stringify of Transaction
	this->signature = sign;
	return 1;
}

// reuturn the signature of this Transaction
std::string Transaction::getSignature() {
	return this->signature;
}

// returns the transactions id
int Transaction::getId() {
	return this->id;
}
