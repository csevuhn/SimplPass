#pragma once
#include <string>
#include <vector>
#include <tuple>

class EncryptionHandler
{
private:
    std::string encryptionPassword;
public:
    EncryptionHandler(const std::string& password);

    bool encryptFile(const std::string& data);

    bool decryptFile(std::vector<std::string>& decryptedData);
};
