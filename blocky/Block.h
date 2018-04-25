#pragma once
#include <string>
#include <chrono>
#include "Util.h"
#include "Transaction.h"

class Block {
	int id;
	std::string prevHash;
	long long int nonce;
	bool mined;
	std::string currHash;
	Transaction *transactions;
	int numTrans;
	
public:
	
	Block(std::string prevHash, int id);
	Block();
	void addTransaction(Transaction transToAdd, std::string privKey);
	std::string hashBlock();
	std::string stringify();
	std::string toString();
	bool mine(int difficulty);
	Transaction *getLastTransaction();
	Transaction *getTransaction(int index);
	int getId();
	int getNumTrans();
	std::string getCurrHash();
	int verifyTransaction(Transaction trans, std::string sig, std::string pubKey);
};
