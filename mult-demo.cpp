/*
 * @file demo-bit-packing.cpp This code shows multiple demonstrations of how to use packing features in PALISADE.
 * @author  TPOC: contact@palisade-crypto.org
 *
 * @copyright Copyright (c) 2019, New Jersey Institute of Technology (NJIT)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <iostream>
#include <fstream>
#include <random>
#include <iterator>

#include "palisade.h"
#include "cryptocontext.h"

#include "encoding/encodings.h"

#include "utils/debug.h"
#include "utils/parmfactory.h"
#include "lattice/elemparamfactory.h"

#include <cmath>

using namespace std;
using namespace lbcrypto;

void BGV_demo();

int main(void) {
	BGV_demo();
	return EXIT_SUCCESS;
}

void BGV_demo() {
	usint m = 32768;
	usint relin = 10;
	float stdDev = 4;
	PlaintextModulus ptm = 1021;
	usint mod_chain_bits = 100;

	shared_ptr<Poly::Params> parms = ElemParamFactory::GenElemParams<Poly::Params>(m, mod_chain_bits);
	CryptoContext<Poly> cc = CryptoContextFactory<Poly>::genCryptoContextBGV(parms, ptm, relin, stdDev, OPTIMIZED, 5);
	cc->Enable(ENCRYPTION);
	cc->Enable(SHE);
	cc->Enable(LEVELEDSHE);

	// Initialize the public key containers.
	LPKeyPair<Poly> kp;
	std::vector<int64_t> vectorOfInts1 = { 1, 0, 1 };
	Plaintext intArray1 = cc->MakeCoefPackedPlaintext(vectorOfInts1);

	std::vector<int64_t> vectorOfInts2 = { 0, 1, 0 };
	Plaintext intArray2 = cc->MakeCoefPackedPlaintext(vectorOfInts2);

	kp = cc->KeyGen();
	cc->EvalMultKeyGen(kp.secretKey);

	Ciphertext<Poly> ciphertext1;
	Ciphertext<Poly> ciphertext2;

	ciphertext1 = cc->Encrypt(kp.publicKey, intArray1);
	ciphertext2 = cc->Encrypt(kp.publicKey, intArray2);

	Ciphertext<Poly> cResult = cc->EvalMult(ciphertext1, ciphertext2);
	cResult = cc->EvalMult(cResult, ciphertext2);

	LPKeyPair<Poly> newKp = cc->KeyGen();
	LPEvalKey<Poly> keySwitchHint2 = cc->KeySwitchGen(kp.secretKey, newKp.secretKey);
	cResult = cc->KeySwitch(keySwitchHint2, cResult);

	Plaintext results;
	cc->Decrypt(newKp.secretKey, cResult, &results);
	std::cout << "Answer: " << results << std::endl;
}
