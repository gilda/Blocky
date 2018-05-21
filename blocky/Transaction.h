#pragma once
#include <string>
#include <vector>
#include "Crypto.h"
#include "FileManager.h"

class Transaction {
	std::vector<Transaction> input;
	std::string hash;
	std::string donor;
	int amount;
	std::string recipient;
	std::string signature;
	int nonce;

public:
	Transaction(std::string donor, int amount, std::string recipient);
	Transaction(std::string hash, std::string donor, int amount, std::string recipient, std::string signature);
	Transaction(std::vector<Transaction> input, std::string hash, std::string donor, int amount, std::string recipient, std::string signature, int nonce);
	Transaction(std::vector<Transaction> input, std::string hash, std::string donor, int amount, std::string recipient, std::string signature);
	Transaction();
	std::string stringify();
	std::string toString();
	int sign(std::string privKey);
	std::string stringifyVerify();
	void setInput(std::vector<Transaction> input);
	std::string getSignature();
	std::string getHash();
	std::string getDonor();
	std::string getRecipient();
	std::vector<Transaction> getInput();
	int getAmount();
	static Transaction parseTransaction(std::string file, int index);
};
