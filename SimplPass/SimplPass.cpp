#include <iostream>
#include <fstream>
#include <limits>
#include "Helpers/Messages/MessageHelper.h"
#include "Encryption/EncryptionHandler.h"
#include <sstream>



void displayMsg(const std::string& msg)
{
    std::cout << "[sys] " << msg << "\n";
}

int main()
{
    std::string encryptionPassword;
    displayMsg("enter encryption password: ");
    std::getline(std::cin, encryptionPassword);

    EncryptionHandler handler(encryptionPassword);

    int choice;
    std::string site, username, password;

    do
    {
        MessageHelper msghelper;
        msghelper.displayMenu();

        std::cin >> choice;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n' );
        }
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            {
                displayMsg("enter site name:");
                std::getline(std::cin, site);
                displayMsg("enter username:");
                std::getline(std::cin, username);
                displayMsg("enter password:");
                std::getline(std::cin, password);

                std::string data = site + " " + username + " " + password;

                if (handler.encryptFile(data))
                {
                    displayMsg("password for site " + site + " saved successfully");
                } else
                {
                    std::cout << "[err] failed to save password" << "\n";
                }
                break;
                
            }

        case 2:
            {
                std::vector<std::string> decryptedData;
                if (handler.decryptFile(decryptedData))
                {
                    displayMsg("\nSaved passwords: ");
                    for (const auto& entry : decryptedData)
                    {
                        std::istringstream iss(entry);
                        std::string site, username, password;
                        iss >> site >> username >> password;
                        std::cout << "Site: " << site << " Username: " << username << "Password: " << password << "\n";
                        
                    }
                } else
                {
                    std::cout << "[err] failed to load passwords or no passwords saved" << "\n";
                }
                break;
                
            }

        case 3:
            displayMsg("\nExiting...");
            break;
        default:
            std::cout << "[err] invalid option" << "\n";
            break;
        

    }
    } while (choice != 3);

    return 0;
}