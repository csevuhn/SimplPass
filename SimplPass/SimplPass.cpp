#include <iostream>
#include <fstream>
#include <limits>

void displayMsg(const std::string& msg)
{
    std::cout << msg << std::endl; 
}

void displayMenu()
{
    displayMsg("Welcome to SimplPass");
    displayMsg("1. Add Password");
    displayMsg("2. View Passwords");
    displayMsg("3. Quit");
}

int main()
{
    int choice;
    std::string site, password, username;

    do
    {
        displayMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
    } while (choice != 3);
    
    return 0;
}