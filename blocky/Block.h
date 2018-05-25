#pragma once
#include <string>
#include <chrono>
#include <vector>
#include "Util.h"
#include "Transaction.h"
//#include "Blockchain.h"

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
	void addTransaction(std::string file, std::string prikey, std::string donor, int amount, std::string recepient);
	std::string hashBlock();
	std::string stringify();
	std::string stringifyBLCK();
	std::string toString();
	bool mine(int difficulty, std::string minerPrivKey, std::string minerPubKey, int reward);
	Transaction *getLastTransaction();
	Transaction *getTransaction(int index);
	std::vector<Transaction> getTransactionVec();
	int getId();
	int getNumTrans();
	std::string getCurrHash();
	std::string getPrevHash();
	int verifyTransaction(Transaction trans, std::string sig, std::string pubKey);
	static Block parseBlock(std::string file, int id);
	bool empty();
	std::vector<Transaction> getTransInputForValue(std::string file, std::string pubKey, int amount);
};
