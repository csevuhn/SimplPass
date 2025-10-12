#include <iostream>
#include <fstream>
#include <limits>
#include "Helpers/MessageHelper.h"

void displayMsg(const std::string& msg)
{
    std::cout << msg << std::endl; 
}


void saveDetails(const std::string& site, const std::string& username, const std::string& password)
{
    std::ofstream file("pwds.txt", std::ios::app);
    if (file.is_open())
    {
        file << site << " " << username << " " << password << "\n";
        file.close();
        std::cout << "Password for site "<< site << " saved successfully.\n";
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing. \n";
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
            std::cout << " Site: " << site << " Username: " << username << " Password: " << password << "\n";
            
        }
        file.close();
    } else
    {
            std::cerr << "\n[System] No passwords saved yet.\n";
    }
}

int main()
{
    int choice;
    std::string site, password, username;

    do
    {
        MessageHelper.displayMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch(choice)
        {
            case 1:
                displayMsg("Enter site name: ");
                std::cin >> site;
                displayMsg("Enter username: ");
                std::cin >> username;
                displayMsg("Enter password: ");
                std::cin >> password;
                saveDetails(site, username, password);
                break;
            case 2:
                loadPasswords();
                break;
            case 3:
                displayMsg("\n Exiting...");
                break;
        default:
            displayMsg("\n Invalid choice");
            break;

        }
        
    } while (choice != 3);
    
    return 0;
}