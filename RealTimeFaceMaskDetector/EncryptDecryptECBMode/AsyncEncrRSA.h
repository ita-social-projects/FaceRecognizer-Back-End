#pragma once
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
//maybe extra headers
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include "ICrypt.h"
const size_t cKeySizeBits = 3072;
const size_t cBlockSize = 341;

class  ENCRYPTDECRYPTECBMODE_API EncryptionRSA: public ICrypt {
private:
	CryptoPP::RSA::PublicKey mPublicKey;
	CryptoPP::RSA::PrivateKey mPrivateKey;
public:
	EncryptionRSA() {};
	void CreatePrivatePublicKeys();
	CryptoPP::RSA::PrivateKey GetPrivateKey() const;
	CryptoPP::RSA::PublicKey GetPublicKey() const;
	void SetPublicKey(const CryptoPP::RSA::PublicKey& paramPublicKey);
	std::vector<std::vector<char>>* SplitMessageIntoBlocks(std::vector<char>& vecPhoto);
	std::vector<char> EncryptStrPhoto(std::vector<char>& vecPhoto);
	std::vector<char> DecryptStrPhoto(std::vector<char>& cipher);
	void SsavePublicKey(const std::string& filePath);
	void LloadPublicKey(const std::string& filePath);
	~EncryptionRSA() {};
};