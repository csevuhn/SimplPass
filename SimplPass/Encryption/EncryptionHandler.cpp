#include "EncryptionHandler.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>


std::vector<unsigned char> deriveKey(const std::string& password, const std::vector<unsigned char>&salt)
{
    std::vector<unsigned char> key(32);
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.data(), salt.size(), 100000, EVP_sha256(), 32, key.data()) !=1)
    {
        throw std::cout << "[err] key derivation failed" << "\n";
    }

    return key;
}

std::vector<unsigned char> generateRandom(size_t length)
{
    std::vector<unsigned char> data(length);
    if (RAND_bytes(data.data(), length) != 1)
    {
        throw std::cout << "[err] failed to generate random characters" << "\n";
    }

    return data;
}

void encryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& encryptionPassword)
{
    try
    {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile)
        {
            throw std::cout << "[err] cannot open input file for encrypting" << "\n";
        }

        std::vector<unsigned char> plaintext((std::istreambuf_iterator<char>(inFile)), {});
        inFile.close();

        std::vector<unsigned char> salt = generateRandom(16);
        std::vector<unsigned char> nonce = generateRandom(12);

        std::vector<unsigned char> key = deriveKey(encryptionPassword, salt);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
        {
            throw std::cout << "[err] failed to create cipher content" << "\n";
        }

        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1)
        {
            throw std::cout << "[err] failed to initialize encryption" << "\n";
        }

        if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), nonce.data()) != 1)
        {
            throw std::cout << "[err] failed to set key and nonce" << "\n";
        }

        std::vector<unsigned char> ciphertext(plaintext.size());
        int length;
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &length, plaintext.data(), plaintext.size()) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::cout << "[err] encryption failed" << "\n";
        }

        int ciphertextLength = length;

        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + length, &length) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::cout << "[err] final encryption failed" << "\n";
        }

        ciphertextLength += length;

        std::vector<unsigned char> tag(16);
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag.data()) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::cout << "[err] failed to get authentication tag" << "\n";
        }

        EVP_CIPHER_CTX_free(ctx);

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile)
        {
            throw std::cout << "[err] cannot open output file" << "\n";
        }

        outFile.write(reinterpret_cast<const char*>(salt.data()), salt.size());
        outFile.write(reinterpret_cast<const char*>(nonce.data()), nonce.size());
        outFile.write(reinterpret_cast<const char*>(tag.data()), tag.size());
        outFile.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertextLength);
        outFile.close();

        std::cout << "[sys] file encrypted successfully: " << outputFile << "\n";
        
    } catch (const std::exception& e)
    {
        throw std::cerr << "[err] encryption error: " << e.what() << "\n";
    }
}
