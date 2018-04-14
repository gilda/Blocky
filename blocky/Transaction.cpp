#include "Transaction.h"

// Constructor
Transaction::Transaction(std::string donor, int amount, std::string recipient) {
	// parameter assigning
	this->donor = donor;
	this->amount = amount;
	this->recipient = recipient;
}

// default constructor for arrays
Transaction::Transaction() {
	this->donor = "";
	this->amount = 0;
	this->recipient = "";
}

// stringify for hashing and file writing
std::string Transaction::stringify() {
	return this->donor + "->" + std::to_string(this->amount) +"->"+ this->recipient;
}

// toString prints all info for debugging or viewing
std::string Transaction::toString() {
	return 
		"{"+
		this->donor+" -> "+
		std::to_string(this->amount)+" -> "+
		this->recipient+"}";
}
