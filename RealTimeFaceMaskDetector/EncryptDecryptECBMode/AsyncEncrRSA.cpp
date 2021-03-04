#include "pch.h"
#include "AsyncEncrRSA.h"


void EncryptionRSA::CreatePrivatePublicKeys() {
	//create randomizer with auto seed
	CryptoPP::AutoSeededRandomPool rng;
	// Generate Parameters
	CryptoPP::InvertibleRSAFunction params;
	//throws KeyingErr if a key can't be generated or algorithm parameters are invalid
	//3072 bits (342 bytes text) usually used size
	params.GenerateRandomWithKeySize(rng, cKeySizeBits);
	//create public and private keys
	//typedef RSAFunction PublicKey;
	//typedef InvertibleRSAFunction PrivateKey;
	CryptoPP::RSA::PrivateKey PrivateKey(params);
	mPrivateKey = PrivateKey;
	std::cout << " Private key created. " << std::endl;
	CryptoPP::RSA::PublicKey PublicKey(params);
	mPublicKey = PublicKey;
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

std::vector<std::vector<char>>* EncryptionRSA::SplitMessageIntoBlocks(std::vector<char>& vecPhoto) {
	const size_t size = (vecPhoto.size() - 1) / cBlockSize + 1;
	std::vector<std::vector<char>>* pVecOfVecs = new std::vector<std::vector<char>>;
	//to not allow iter out of range error
	std::vector<char> toPush{};
	for (size_t i = 0; i < size; ++i) {
		auto startIter = std::next(vecPhoto.begin(), i * cBlockSize);
		auto endIter = std::next(vecPhoto.begin(), i * cBlockSize + cBlockSize);
		pVecOfVecs->push_back(toPush);
		pVecOfVecs->at(i).resize(cBlockSize);
		if (i * cBlockSize + cBlockSize > vecPhoto.size()) {
			endIter = vecPhoto.end();
			pVecOfVecs->at(i).resize(vecPhoto.size() - i * cBlockSize);
		}
		std::copy(startIter, endIter, pVecOfVecs->at(i).begin());
	}
	return pVecOfVecs;
}

std::vector<char> EncryptionRSA::EncryptStrPhoto(std::vector<char>& vecPhoto) {
	try {
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::RSAES_OAEP_SHA_Encryptor encr(mPublicKey);
		//std::cout << "encr.FixedMaxPlaintextLength() " << encr.FixedMaxPlaintextLength() << std::endl;
		//std::cout << "plaintext.size() " << strPhoto.size() << std::endl;
		std::string cipher;
		std::string strPhoto(vecPhoto.begin(), vecPhoto.end());
		// StringSource and  PK_EncryptorFilter
		CryptoPP::StringSource ss1(strPhoto, true, new CryptoPP::PK_EncryptorFilter(rng, encr,
			new CryptoPP::StringSink(cipher)));
		std::vector<char> vecCipher(cipher.begin(), cipher.end());
		return vecCipher;
	}
	catch (CryptoPP::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

std::vector<char> EncryptionRSA::DecryptStrPhoto(std::vector<char>& cipher) {
	try {
		std::string strCipher(cipher.begin(), cipher.end());
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::RSAES_OAEP_SHA_Decryptor decr(mPrivateKey);
		std::string recovered;
		// StringSource and PK_DecryptorFilter
		CryptoPP::StringSource ss2(strCipher, true, new CryptoPP::PK_DecryptorFilter(rng, decr,
			new CryptoPP::StringSink(recovered)));
		std::vector<char> vecRecovered(recovered.begin(), recovered.end());
		return vecRecovered;
	}
	catch (CryptoPP::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void EncryptionRSA::SsavePublicKey(const std::string& filePath) {
	try {
		std::cout << " Begin public key save " << std::endl;
		CryptoPP::FileSink file(filePath.c_str());
		mPublicKey.Save(file);
		std::cout << " End public key save " << std::endl;
	} catch (CryptoPP::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void EncryptionRSA::LloadPublicKey(const std::string& filePath) {
	try {
		std::cout << " Begin public key load " << std::endl;
		CryptoPP::FileSource file(filePath.c_str(), true);
		mPublicKey.Load(file);
		std::cout << " End public key load " << std::endl;
	}
	catch (CryptoPP::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}