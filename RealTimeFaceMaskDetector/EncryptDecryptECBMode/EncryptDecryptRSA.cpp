#include "pch.h"
#include "EncryptDecryptRSA.h"


EncryptDecryptRSA::EncryptDecryptRSA()
{
	//logger message
	CryptoPP::AutoSeededRandomPool randomizer;
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(randomizer, 3072);
	m_private_key = params;
	m_public_key = params;
}
EncryptDecryptRSA::EncryptDecryptRSA(const CryptoPP::RSA::PrivateKey& private_key, const CryptoPP::RSA::PublicKey& public_key)
{		
	m_private_key = private_key;
	m_public_key = public_key;
	//logger message
	
}
EncryptDecryptRSA::EncryptDecryptRSA(const CryptoPP::RSA::PrivateKey& private_key)
{
	m_private_key = private_key;
	//logger message
}
EncryptDecryptRSA::EncryptDecryptRSA(const CryptoPP::RSA::PublicKey& public_key)
{
	m_public_key = public_key;
	//logger message
}
EncryptDecryptRSA::~EncryptDecryptRSA()
{
	//logger message
}
bool EncryptDecryptRSA::Ecnrypt(const std::string& text, std::string& cipher) const 
{
	try
	{
		if (text.empty())
		{
			return false;
			//logger error about empty text
		}
		CryptoPP::AutoSeededRandomPool randomizer;
		CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(m_public_key);
		std::unique_ptr<CryptoPP::StringSink*> string_sink = std::make_unique<CryptoPP::StringSink*>(new CryptoPP::StringSink(cipher));
		std::unique_ptr<CryptoPP::PK_EncryptorFilter*> filter = std::make_unique<CryptoPP::PK_EncryptorFilter*>(new CryptoPP::PK_EncryptorFilter(randomizer, encryptor, *string_sink.get()));

		CryptoPP::StringSource encrypt(text, true, *filter.get());
		return true;
	}
	catch (...)
	{
		//logger message
		return false;
	}
}
bool EncryptDecryptRSA::Decrypt(const std::string& cipher, std::string& text) const
{
	try
	{
		if (cipher.empty())
		{
			return false;
			//logger error about empty text
		}
		CryptoPP::AutoSeededRandomPool randomizer;
		CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(m_private_key);
		std::unique_ptr<CryptoPP::StringSink*> string_sink = std::make_unique<CryptoPP::StringSink*>(new CryptoPP::StringSink(text));
		std::unique_ptr<CryptoPP::PK_DecryptorFilter*> filter = std::make_unique<CryptoPP::PK_DecryptorFilter*>(new CryptoPP::PK_DecryptorFilter(randomizer, decryptor, *string_sink.get()));


		CryptoPP::StringSource decrypt(cipher, true, *filter.get());
		return true;
	}
	catch (...)
	{
		//logger message
		return false;
	}
}
void EncryptDecryptRSA::CreateKey(const size_t key_size ) 
{
	//logger message
	CryptoPP::AutoSeededRandomPool randomizer;
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(randomizer, key_size);
	m_private_key = params;
	m_public_key = params;
}
CryptoPP::RSA::PrivateKey EncryptDecryptRSA::get_private_key() const
{
	return m_private_key;
}
CryptoPP::RSA::PublicKey EncryptDecryptRSA::get_public_key() const
{
	return m_public_key;
}
void EncryptDecryptRSA::set_keys(const CryptoPP::RSA::PrivateKey& private_key, const CryptoPP::RSA::PublicKey& public_key)
{
	m_private_key = private_key;
	m_public_key = public_key;
}
void EncryptDecryptRSA::set_private_key(const CryptoPP::RSA::PrivateKey& private_key)
{
	m_private_key = private_key;
}

void EncryptDecryptRSA::set_public_key(const CryptoPP::RSA::PublicKey& public_key)
{
	m_public_key = public_key;
}