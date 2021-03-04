#pragma once
#include <memory>
#include "ICrypt.h"
#include <cryptopp/rsa.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

//class ENCRYPTDECRYPTECBMODE_API EncryptDecryptRSA : public ICrypt
//{
//public:
//	
//	EncryptDecryptRSA();
//	EncryptDecryptRSA(const CryptoPP::RSA::PrivateKey& private_key, const CryptoPP::RSA::PublicKey& public_key);
//	EncryptDecryptRSA(const CryptoPP::RSA::PrivateKey& private_key);
//	EncryptDecryptRSA(const CryptoPP::RSA::PublicKey& public_key);
//	~EncryptDecryptRSA();
//	bool Encrypt(const std::string& text, std::string& cipher) const override;
//	bool Decrypt(const std::string& cipher, std::string& text) const override;
//	void CreateKey(const size_t key_size = 3072) override;
//	CryptoPP::RSA::PrivateKey get_private_key() const;
//	CryptoPP::RSA::PublicKey get_public_key() const;
//	void set_keys(const CryptoPP::RSA::PrivateKey& private_key, const CryptoPP::RSA::PublicKey& public_key);
//	void set_private_key(const CryptoPP::RSA::PrivateKey& private_key);
//	void set_public_key(const CryptoPP::RSA::PublicKey& public_key);
//
//private:
//
//	CryptoPP::RSA::PrivateKey m_private_key;
//	CryptoPP::RSA::PublicKey m_public_key;
//};

