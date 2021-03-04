#pragma once
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include "ICrypt.h"


class  ENCRYPTDECRYPTECBMODE_API EncryptionRSA: public ICrypt {
private:
	CryptoPP::RSA::PublicKey mPublicKey;
	CryptoPP::RSA::PrivateKey mPrivateKey;
	//CryptoPP::AutoSeededRandomPool mRng;
public:
	EncryptionRSA() {};
	void CreatePrivatePublicKeys();
	CryptoPP::RSA::PrivateKey GetPrivateKey() const;
	CryptoPP::RSA::PublicKey GetPublicKey() const;
	void SetPublicKey(const CryptoPP::RSA::PublicKey& paramPublicKey);
	std::string EncryptStrPhoto(std::string& strPhoto);
	std::string DecryptStrPhoto(std::string& cipher);
	//save the key to a disk
	//load the key from a disk
	~EncryptionRSA() {};
};