// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ENCRYPTDECRYPTECBMODE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ENCRYPTDECRYPTECBMODE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#ifdef ENCRYPTDECRYPTECBMODE_EXPORTS
#define ENCRYPTDECRYPTECBMODE_API __declspec(dllexport)
#else
#define ENCRYPTDECRYPTECBMODE_API __declspec(dllimport)
#endif

#ifndef ENCRYPT_DECRYPT_INTERFACES_H
#define ENCRYPT_DECRYPT_INTERFACES_H


#include <cstddef>
#include <string>

class ENCRYPTDECRYPTECBMODE_API ICrypt
{
public:
	//[[nodiscard]]virtual bool Encrypt(const std::string& text, std::string& cipher) const = 0;
	//[[nodiscard]]virtual bool Decrypt(const std::string& cipher, std::string& text) const = 0;
	//[[nodiscard]]virtual void CreateKey(const std::size_t key_size) = 0;
	[[nodiscard]] virtual void CreatePrivatePublicKeys() = 0;
	[[nodiscard]] virtual CryptoPP::RSA::PrivateKey GetPrivateKey() const = 0;
	[[nodiscard]] virtual CryptoPP::RSA::PublicKey GetPublicKey() const = 0;
	[[nodiscard]] virtual void SetPublicKey(const CryptoPP::RSA::PublicKey& paramPublicKey) = 0;
	[[nodiscard]] virtual std::vector<char> EncryptStrPhoto(std::vector<char>& VecPhoto) = 0;
	[[nodiscard]] virtual std::vector<char> DecryptStrPhoto(std::vector<char>& cipher) = 0;
};
#endif // !ENCRYPT_DECRYPT_INTERFACES_H