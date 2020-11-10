#pragma once
#include <memory>
#include "ICrypt.h"
#include "..\..\3rdPartyLibs\cryptopp820\aes.h"
#include "..\..\3rdPartyLibs\cryptopp820\modes.h"
#include "..\..\3rdPartyLibs\cryptopp820\filters.h"
#include "..\..\3rdPartyLibs\cryptopp820\osrng.h"

template class __declspec(dllexport) std::unique_ptr<CryptoPP::byte*>;
class ENCRYPTDECRYPTECBMODE_API EncryptDecryptAES_ECBMode : public ICrypt
{
public:
	EncryptDecryptAES_ECBMode();
	EncryptDecryptAES_ECBMode(unsigned char* key, const std::size_t key_size);
	virtual ~EncryptDecryptAES_ECBMode();
	virtual bool Ecnrypt(const std::string& text, std::string& cipher) const override;
	virtual bool Decrypt(const std::string& cipher, std::string& text) const override;
	virtual void CreateKey(const size_t key_size = 16) override;
	virtual const unsigned char* get_key() const;
	virtual std::size_t get_key_size() const;
	virtual bool set_key(unsigned char* key, const std::size_t key_size);

private:
	std::unique_ptr<CryptoPP::byte*> m_key;
	std::size_t m_key_size;
};

