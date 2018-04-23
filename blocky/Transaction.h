#pragma once
#include <string>
#include "Crypto.h"

class Transaction {
	int id;
	std::string donor;
	int amount;
	std::string recipient;
	std::string signature;

public:
	Transaction(int id, std::string donor, int amount, std::string recipient);
	Transaction();
	std::string stringify();
	std::string toString();
	int sign(std::string privKey);
	std::string Transaction::stringifyVerify();
	std::string getSignature();
};
