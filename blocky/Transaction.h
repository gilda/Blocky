#pragma once
#include <string>

class Transaction {
	std::string donor;
	int amount;
	std::string recipient;

public:
	Transaction(std::string, int, std::string);
	Transaction();
	std::string stringify();
	std::string toString();
};
