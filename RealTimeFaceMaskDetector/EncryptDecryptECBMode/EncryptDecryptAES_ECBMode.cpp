#include "pch.h"
#include "EncryptDecryptAES_ECBMode.h"
#include <fstream>

//EncryptDecryptAES_ECBMode::EncryptDecryptAES_ECBMode() 
//{
//	//logger message
//	unsigned char* key = (unsigned char*)(R"(}8)
//		åµÎðD & :·Ïýýýý)");
//	m_key = std::make_unique<CryptoPP::byte*>(key);
//	m_key_size = 0;
//}
//EncryptDecryptAES_ECBMode::EncryptDecryptAES_ECBMode(unsigned char* key, const std::size_t key_size) 
//{
//	if (!key)
//	{
//		m_key = nullptr;
//		m_key_size = 0;
//		//logger message
//		return;
//	}
//	if (key_size != 16
//		&& key_size != 32
//		&& key_size != 64)
//	{
//		m_key = nullptr;
//		m_key_size = 0;
//		//logger message
//	}
//	else
//	{
//		m_key = std::make_unique<CryptoPP::byte*>(reinterpret_cast<CryptoPP::byte*>(key));
//		m_key_size = key_size;
//		//logger message
//	}
//}
//EncryptDecryptAES_ECBMode::~EncryptDecryptAES_ECBMode() 
//{
//	//logger message?
//}
//bool EncryptDecryptAES_ECBMode::Decrypt(const std::string& cipher, std::string& text) const 
//{
//	if (cipher.empty() || !*m_key.get())
//	{
//		return false;
//		//logger error about empty key
//	}
//	try
//	{
//		text.clear();
//		CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption decryptor;
//		decryptor.SetKey(*m_key.get(), m_key_size);
//
//
//		std::unique_ptr<CryptoPP::StringSink*> string_source = std::make_unique<CryptoPP::StringSink*>(new CryptoPP::StringSink(text));
//		std::unique_ptr<CryptoPP::StreamTransformationFilter*> string_transformation_filter =
//			std::make_unique<CryptoPP::StreamTransformationFilter*>(new CryptoPP::StreamTransformationFilter(decryptor, *string_source.get()));
//		// The StreamTransformationFilter removes
//		//  padding as required.
//		CryptoPP::StringSource ss3(cipher, true, *string_transformation_filter.get());
//
//		//logger message about positive decryption
//		return true;
//	}
//	catch (CryptoPP::Exception& e)
//	{
//		//logger message about exception
//		return false;
//	}
//}
//bool EncryptDecryptAES_ECBMode::Encrypt(const std::string& text, std::string& cipher) const 
//{
//	if (text.empty()||!*m_key.get())
//	{
//		return false;
//		//logger error about empty key
//	}
//	try
//	{
//		CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption encryptor;
//		encryptor.SetKey(*m_key.get(), m_key_size);
//		std::unique_ptr<CryptoPP::StringSink*> string_source = std::make_unique<CryptoPP::StringSink*>(new CryptoPP::StringSink(cipher));
//		std::unique_ptr<CryptoPP::StreamTransformationFilter*> string_transformation_filter = 
//			std::make_unique<CryptoPP::StreamTransformationFilter*> (new CryptoPP::StreamTransformationFilter(encryptor, *string_source.get()));  
//		
//		// The StreamTransformationFilter adds padding
//		//  as required. ECB and CBC Mode must be padded
//		//  to the block size of the cipher.
//		CryptoPP::StringSource ss1(text, true, *string_transformation_filter.get());
//		//logger message
//		return true;
//	}
//	catch (CryptoPP::Exception& e)
//	{
//		return false;
//		//logger error of exception
//	}
//}
//void EncryptDecryptAES_ECBMode::CreateKey(const size_t key_size)
//{
//	m_key.release();
//	if (key_size != 16
//		&& key_size != 32
//		&& key_size != 64)
//	{
//		m_key_size = 16;
//	}
//	else
//	{
//		m_key_size = key_size;
//	}
//	CryptoPP::AutoSeededRandomPool randomizer;
//	CryptoPP::byte* key = new CryptoPP::byte[key_size];
//	randomizer.GenerateBlock(key, key_size);
//	m_key = std::make_unique<CryptoPP::byte*>(key);
//	
//}
//const unsigned char* EncryptDecryptAES_ECBMode::get_key() const 
//{
//	return reinterpret_cast<const unsigned char*>(*m_key.get());
//}
//std::size_t EncryptDecryptAES_ECBMode::get_key_size() const 
//{
//	return m_key_size;
//}
//bool EncryptDecryptAES_ECBMode::set_key(unsigned char* key, const std::size_t key_size) 
//{	
//	if (!key)
//	{
//		m_key = nullptr;
//		m_key_size = 0;
//		//logger message
//		return false;
//	}
//	if (key_size != 16
//		&& key_size != 32
//		&& key_size != 64)
//	{
//		m_key = nullptr;
//		m_key_size = 0;
//		//logger message
//		return false;
//	}
//	else
//	{
//		m_key.release();
//		m_key_size = key_size;
//		m_key = std::make_unique<CryptoPP::byte*>(reinterpret_cast<CryptoPP::byte*>(key));
//		//logger message
//		return true;
//	}
//}