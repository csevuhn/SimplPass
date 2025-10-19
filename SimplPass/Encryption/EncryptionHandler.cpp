#include "EncryptionHandler.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

EncryptionHandler::EncryptionHandler(const std::string& password) : encryptionPassword(password)
{
    if (password.empty())
    {
        throw std::runtime_error("[err] encryption password cannot be empty");
    }
}

std::vector<unsigned char> deriveKey(const std::string& password, const std::vector<unsigned char>& salt)
{
    std::vector<unsigned char> key(32);
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.data(), salt.size(), 100000, EVP_sha256(), 32, key.data()) != 1)
    {
        throw std::runtime_error("[err] key derivation failed");
    }
    return key;    
}

std::vector<unsigned char> generateRandom(size_t length)
{
    std::vector<unsigned char> data(length);
    if (RAND_bytes(data.data(), length) != 1)
    {
        throw std::runtime_error("[err] failed to generate random characters");
    }

    return data;
}

bool EncryptionHandler::encryptFile(const std::string& data)
{
    try
    {
        std::vector<unsigned char> salt = generateRandom(16);
        std::vector<unsigned char> nonce = generateRandom(12);
        std::vector<unsigned char> key = deriveKey(encryptionPassword, salt);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
        {
            throw std::runtime_error("[err] failed to create cipher context");
        }

        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("[err] failed to initialize encryption");
        }

        if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), nonce.data()) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("[err] failed to set key and nonce ");
        }

        std::vector<unsigned char> plaintext(data.begin(), data.end());
        std::vector<unsigned char> ciphertext(plaintext.size());
        int length;
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &length, plaintext.data(), plaintext.size()) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("[err] encryption failed");
        }

        int ciphertextLength = length;

        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + length, &length) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("[err] final encryption failed");
        }

        ciphertextLength += length;

        std::vector<unsigned char> tag (16);
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag.data()) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("[err] failed to get authentication tag");
        }

        EVP_CIPHER_CTX_free(ctx);

        std::ofstream outFile("pwds.txt", std::ios::binary | std::ios::app);
        if (!outFile)
        {
            throw std::runtime_error("[err] cannot open passwords file for writing");
        }

        outFile.write(reinterpret_cast<const char*>(salt.data()), salt.size());
        outFile.write(reinterpret_cast<const char*>(nonce.data()), nonce.size());
        outFile.write(reinterpret_cast<const char*>(tag.data()), tag.size());
        outFile.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertextLength());
        outFile.close();

        return true;
    } catch (const std::exception& e)
    {
        std::cerr << "[err] encryption error: " << e.what() << "\n";
        return false;
    }

    

}

bool EncryptionHandler::decryptFile(std::vector<std::string>& decryptedData)
{
    try
    {
        std::ifstream inFile("pwds.txt", std::ios::binary);
        if (!inFile)
        {
            throw std::runtime_error("[err] failed to open passwords file for reading");
        }

        decryptedData.clear();
        while (inFile.peek() != EOF)
        {
            std::vector<unsigned char> salt(16);
            std::vector<unsigned char> nonce(12);
            std::vector<unsigned char> tag(16);
            inFile.read(reinterpret_cast<char*>(salt.data()), 16);
            inFile.read(reinterpret_cast<char*>(nonce.data()), 12);
            inFile.read(reinterpret_cast<char*>(tag.data()), 16);
            if (inFile.gcount() < 16)
            {
                break;
            }

            std::vector<unsigned char> ciphertext;
            char byte;
            while (inFile.get(byte))
            {
                ciphertext.push_back(static_cast<unsigned char>(byte));
            }
            inFile.clear();

            std::vector<unsigned char> key = deriveKey(encryptionPassword, salt);

            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx)
            {
                throw std::runtime_error("[err] failed to create cipher context");
            }

            if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1)
            {
                throw std::runtime_error("[err] failed to initialize decryption");
            }

            if (EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), nonce.data()) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("[err] failed to set key and nonce");
            }

            std::vector<unsigned char> plaintext(ciphertext.size());
            int length;
            if (EVP_DecryptUpdate(ctx, plaintext.data(), &length, ciphertext.data(), ciphertext.size()) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("[err] decryption failed");
            }

            int plaintextLength = length;

            if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag.data()) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("[err] failed to set authentication tag");
            }

            if (EVP_DecryptFinal_ex(ctx, plaintext.data() + length, &length) != 1 )
            {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("[err] final decryption failed");
            }

            plaintextLength += length;
            EVP_CIPHER_CTX_free(ctx);

            decryptedData.emplace_back(std::string(plaintext.begin(), plaintext.begin() + plaintextLength));
            
            
        }

        inFile.close();
        return true;
    } catch (const std::exception& e)
    {
        std::cerr << "[err] decryption error: " << e.what() << "\n";
        return false;
    }
}

