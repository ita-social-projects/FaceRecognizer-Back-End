#include "pch.h"
#include "AsyncEncrRSA.h"


void EncryptionRSA::CreatePrivatePublicKeys() {
	//create randomizer with auto seed
	CryptoPP::AutoSeededRandomPool rng;
	// Generate Parameters
	CryptoPP::InvertibleRSAFunction params;
	//throws KeyingErr if a key can't be generated or algorithm parameters are invalid
	//3072 bits usually used size 
	params.GenerateRandomWithKeySize(rng, 3072);
	//create public and private keys
	//typedef RSAFunction PublicKey;
	//typedef InvertibleRSAFunction PrivateKey;
	CryptoPP::RSA::PrivateKey mPrivateKey(params);
	std::cout << " Private key created. " << std::endl;
	CryptoPP::RSA::PublicKey mPublicKey(params);
	std::cout << " Public key created. " << std::endl;
}

CryptoPP::RSA::PrivateKey EncryptionRSA::GetPrivateKey() const{
	std::cout << " GetPrivateKey() called. " << std::endl;
	return mPrivateKey;
}

CryptoPP::RSA::PublicKey EncryptionRSA::GetPublicKey() const{
	std::cout << " GetPublicKey() called. " << std::endl;
	return mPublicKey;
}

void EncryptionRSA::SetPublicKey(const CryptoPP::RSA::PublicKey& paramPublicKey) {
	mPublicKey = paramPublicKey;
	std::cout << " Public key set successfully. " << std::endl;
}

std::string EncryptionRSA::EncryptStrPhoto(std::string& strPhoto) {
	try {
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::RSAES_OAEP_SHA_Encryptor encr(mPublicKey);
		std::string cipher;
		// StringSource and  PK_EncryptorFilter
		CryptoPP::StringSource ss1(strPhoto, true, new CryptoPP::PK_EncryptorFilter(rng, encr,
			new CryptoPP::StringSink(cipher)));
		return cipher;
	}
	catch (CryptoPP::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

std::string EncryptionRSA::DecryptStrPhoto(std::string& cipher) {
	try {
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::RSAES_OAEP_SHA_Decryptor decr(mPrivateKey);
		std::string recovered;
		// StringSource and PK_DecryptorFilter
		CryptoPP::StringSource ss2(cipher, true, new CryptoPP::PK_DecryptorFilter(rng, decr,
			new CryptoPP::StringSink(recovered)));
		return recovered;
	}
	catch (CryptoPP::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}