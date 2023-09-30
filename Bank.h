/*
* Author: Abdul-Aziz Nassor
* File: Bank.h
* Header file for all the classes used by the Bank System
* Classes: Credit Card, Account, Bank, ATM
*/

#ifndef BANK_H
#define BANK_H

// INCLUDE FILES
#include <memory>
#include <vector>
#include <string>

// USING NAMESPACES
using std::string;
using std::unique_ptr;
using std::vector;

//	*------------------------	CREDIT CARD CLASS	-------------------------*	//
class CreditCard {
private:
	string cardNumber;
	string pin;
public:
	CreditCard(string CardNumber, string Pin);
	~CreditCard();
	string getCardNumber();
	bool verifyPin(string Pin);
};

//	*------------------------	ACCOUNT CLASS	-------------------------*	//
class Account {
private:
	string firstName;
	string lastName;
	int accountNumber;
	int sortCode;
	double balance;
	vector<CreditCard> creditCards;
public:
	Account(string FirstName, string LastName, int AccountNumber, int SortCode, double Balance);
	~Account();
	void addCard(string CardNumber, string Pin);
	vector<CreditCard> getCards();
	CreditCard getCardFromCardNumber(string CardNumber);
	string getFirstName();
	string getLastName();
	string getFullName();
	int getAccountNumber();
	int getSortCode();
	double getBalance();
	int withdrawCash(double amount);
	int depositCash(double amount);
};

//	*------------------------	BANK CLASS	-------------------------*	//
class Bank {
private:
	string name;
	vector<Account> accounts;
public:
	Bank(string Name);
	~Bank();
	string getName();
	void addAccount(Account account);
	Account getAccountFromCardNumber(string CardNumber);
	Account getAccountFromAccountDetails(int AccountNumber, int SortCode);
};

//	*------------------------	ATM CLASS	-------------------------*	//
class ATM {
private:
	Bank bank;
	Account* user = nullptr;
	string currentCardNumber = "";
public:
	ATM(Bank Bank_);
	~ATM();
	void menu();
	void login();
	void bankServicesMenu();
	void withdraw();
	void deposit();
	void viewBalance();
	void viewDetails();
	void viewTransactions();
	void createTransaction(string action, double amount);
};

#endif //	BANK_H