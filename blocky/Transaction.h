#pragma once
#include <string>
#include "Crypto.h"

class Transaction {
	Transaction *input;
	std::string hash;
	std::string donor;
	int amount;
	std::string recipient;
	std::string signature;

public:
	Transaction(std::string donor, int amount, std::string recipient);
	Transaction();
	std::string stringify();
	std::string toString();
	int sign(std::string privKey);
	std::string stringifyVerify();
	std::string getSignature();
	std::string getHash();
	std::string getDonor();
	std::string getRecipient();
	int getAmount();
};
