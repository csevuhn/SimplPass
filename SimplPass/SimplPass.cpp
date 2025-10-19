#include <iostream>
#include <fstream>
#include <limits>
#include "Helpers/Messages/MessageHelper.h"
#include "Encryption/EncryptionHandler.h"



void displayMsg(const std::string& msg)
{
    std::cout << msg << "\n"; 
}


void saveDetails(const std::string& site, const std::string& username, const std::string& password)
{
    std::ofstream file("pwds.txt", std::ios::app);
    if (file.is_open())
    {
        file << site << " " << username << " " << password << "\n";
        file.close();
        std::cout << "password for site "<< site << " saved successfully.\n";
    }
    else
    {
        MessageHelper msghelper;
        msghelper.printError("unable to open file for writing.");
    }
}

void loadPasswords()
{
    std::ifstream file("pwds.txt");
    if (file.is_open())
    {
        std::string site, username, password;
        displayMsg("\n Saved passwords:");
        while (file >> site >> username >> password)
        {
            std::cout << " site: " << site << " username: " << username << " password: " << password << "\n";
            
        }
        file.close();
    } else
    {
            MessageHelper msghelper;
            msghelper.printError("no passwords saved yet.");
    }
}

int main() 
{
    int choice;
    std::string site, password, username;

    do
    {
        MessageHelper msghelper;
        msghelper.displayMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch(choice)
        {
            case 1:
                displayMsg("enter site name: ");
                std::cin >> site;
                displayMsg("enter username: ");
                std::cin >> username;
                displayMsg("enter password: ");
                std::cin >> password;
                saveDetails(site, username, password);
                break;
            case 2:
                loadPasswords();
                break;
            case 3:
                displayMsg("\n exiting...");
                break;
        default:
            msghelper.printError("invalid option");
            break;

        }
        
    } while (choice != 3);
    
    return 0;
}