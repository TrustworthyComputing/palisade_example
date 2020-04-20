#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <random>

#include "palisade.h"

#include "cryptocontextgen.h"
#include "cryptocontexthelper.h"

#include "utils/debug.h"

using namespace std;
using namespace lbcrypto;


void BGV_demo();

int main() {
	BGV_demo();
	return 0;
}

CryptoContext<DCRTPoly> GenerateBGVrnsContext(usint ptm, usint mult_depth) {

	usint m = 32768;
	usint numPrimes = 10;
	usint relin = 10;
	usint maxDepth = 10; //the maximum power of secret key for which the relinearization key is generated
	usint firstModSize = 50;
	usint dcrtBits = 50; // size of "small" CRT moduli


	auto cc = CryptoContextFactory<DCRTPoly>::genCryptoContextBGVrnsWithParamsGen(
		m, numPrimes, ptm, relin, OPTIMIZED, mult_depth, maxDepth, BV, firstModSize, dcrtBits, 0);
  cc->Enable(ENCRYPTION);
  cc->Enable(SHE);
  cc->Enable(LEVELEDSHE);

  std::cout << "\nParameters BGVrns for depth " << mult_depth << std::endl;
  std::cout << "p = " << cc->GetCryptoParameters()->GetPlaintextModulus() <<
  std::endl; std::cout << "n = " <<
  cc->GetCryptoParameters()->GetElementParams()->GetCyclotomicOrder() / 2 <<
  std::endl; std::cout << "log2 q = " <<
  log2(cc->GetCryptoParameters()->GetElementParams()->GetModulus().ConvertToDouble())
  << "\n" << std::endl;

  return cc;
}

void BGV_demo() {

	usint ptm = 1021;

	CryptoContext<DCRTPoly> cc = GenerateBGVrnsContext(ptm, 10);

	// KeyGen
	LPKeyPair<DCRTPoly> keyPair = cc->KeyGen();
	cc->EvalMultKeyGen(keyPair.secretKey);

	std::vector<int64_t> vectorOfInts = {1,1};
	Plaintext plaintext = cc->MakeCoefPackedPlaintext(vectorOfInts);

	vector<Ciphertext<DCRTPoly>> ciphertexts;

	for (int i = 0; i < 10; i++)
		ciphertexts.push_back(cc->Encrypt(keyPair.publicKey, plaintext));

	Ciphertext<DCRTPoly> ciphertextMult;
	ciphertextMult = cc->EvalMultMany(ciphertexts);

	Plaintext plaintextDec;
	cc->Decrypt(keyPair.secretKey, ciphertextMult, &plaintextDec);
	//plaintextDec->SetLength(plaintext->GetLength());
	std::cout << "Original plaintext: " << plaintext << std::endl;
	std::cout << "Evaluated plaintext: " << plaintextDec << std::endl;
}
