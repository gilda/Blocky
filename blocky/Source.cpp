#include <stdio.h>
#include "Util.h"
#include "Transaction.h"
#include "Block.h"
#include "Blockchain.h"
#include "Crypto.h"

int main() {
	//TODO Crypto: fix sign and verify, sign return, convert verify to compressed form
	//TODO Transaction: sign(key), member string signature, 
	//TODO Block: validateTransaction
	//TODO BlockChain: addTransaction, addBlock, validateBlock
	//TODO Ledger: openFile, writeBlockchain, start parser
	//TODO Util: 
	//TODO UI:
	Util::initOpenSSL(); // init OpenSSL
	Blockchain gldc = Blockchain("gldc.ledg", 32, 10);
	//gldc.mineLastBlock();
	//printf("%s", gldc.getGenesis()->toString().c_str());

	Transaction money = Transaction(0, "gilda", 5, "baba");
	std::string raw = money.stringify();
	printf("stringify: %s\r\n", money.stringify().c_str());
	EC_KEY *key = Crypto::genKey();
	printf("hash: %s\r\n", Util::Hash256(money.stringify()).c_str());
	std::string privKey(BN_bn2hex(EC_KEY_get0_private_key(key)));
	BIGNUM *x = BN_new();
	BIGNUM *y = BN_new();
	const BIGNUM *bnsp = EC_KEY_get0_private_key(key);
	EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(key), EC_KEY_get0_public_key(key), x, y, NULL);
	std::string sx(BN_bn2hex(x));
	sx += BN_bn2hex(y);
	printf("publicKey: %s\r\n", sx.c_str());
	printf("privKey: %s\r\n", privKey.c_str());
	money.sign(privKey);
	printf("stringify: %s\r\n", money.stringify().c_str());
	printf("verify: %d\r\n", Crypto::verify(Util::Hash256(raw), money.getSignature(), sx));

	Util::cleanupOpenSSL(); // CleanUp SSL
	system("pause");
	return 0; // exit Process
}
