#pragma once
#include <string>
#include <chrono>
#include <vector>
#include "Util.h"
#include "Transaction.h"

class Block {
	int id;
	std::string prevHash;
	long long int nonce;
	bool mined;
	std::string currHash;
	std::vector<Transaction> transactions;
	int numTrans;
	
public:
	
	Block(std::string prevHash, int id);
	Block(int id, std::string prevHash, long long int nonce, std::string currHash, std::vector<Transaction> transactions, int numTrans);
	Block();
	void addTransaction(std::string pubkey, Transaction transToAdd);
	std::string hashBlock();
	std::string stringify();
	std::string stringifyBLCK();
	std::string toString();
	bool mine(int difficulty, std::string minerPubKey);
	Transaction *getLastTransaction();
	Transaction *getTransaction(int index);
	std::vector<Transaction> getTransactionVec();
	int getId();
	int getNumTrans();
	std::string getCurrHash();
	int verifyTransaction(Transaction trans, std::string sig, std::string pubKey);
	static Block parseBlock(std::string file, int id);
};
