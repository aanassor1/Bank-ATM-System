/*
* Author: Abdul-Aziz Nassor
* File: Main.cpp
* Main file for the C++ program.
* Initialises sample bank, account and ATM classes for use for the program.
* WARNING: Be sure to set a Logs folder to LOGS_FOLDER in Bank.cpp before initiating program!
*/

// INCLUDE FILES
#include "Bank.h"
#include <memory>

// USING NAMESPACES 
using std::unique_ptr;
using std::make_unique;
using std::move;

int main()
{   
    Bank bank = Bank("HSBC");

    Account acc1 = Account("John", "Doe", 12345678, 010100, 100);
    acc1.addCard("4024007178080551", "1234");
    acc1.addCard("346203489008543", "4567");
    acc1.addCard("6011417272764770", "8901");

    Account acc2 = Account("Jane", "Doe", 87654321, 020201, 2500);
    acc2.addCard("5768235467581928", "1111");
    acc2.addCard("6011390695966977", "3969");

    Account acc3 = Account("Mary", "Major", 13246587, 301223, 1250);
    acc3.addCard("1389544623879911", "9876");

    bank.addAccount(acc1);
    bank.addAccount(acc2);
    bank.addAccount(acc3);

    ATM atm(bank);
    atm.menu();
}