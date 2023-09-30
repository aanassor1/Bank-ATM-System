/*
* Author: Abdul-Aziz Nassor
* File: Bank.cpp
* Implementation file for all the classes used by the Bank System
* Classes: Credit Card, Account, Bank, ATM
* WARNING: Be sure to set a Logs folder to LOGS_FOLDER before initiating program!
*/

// INCLUDE FILES
#include <iostream>
#include <string>
#include <memory>
#include <conio.h>
#include <fstream>                        
#include <cmath>
#include <cstdlib>

// INCLUDE USER MADE FILES
#include "Bank.h"

// USING NAMESPACES
using std::string;
using std::cout;
using std::cin;
using std::unique_ptr;
using std::move;
using std::fstream;
using std::ifstream;
using std::to_string;
using std::ceil;

// CONSTANTS
const int MAX_NUMBER_LENGTH					= 9;
const double MAX_BALANCE					= 999999;
const double MAX_DEPOSIT					= 5000;
const double MIN_DEPOSIT					= 5;
const string LOGS_FOLDER					= ".../Logs"; // CHANGE THIS TO YOUR ALLOCATED LOGS FOLDER PATH


//	*------------------------	ACCOUNT CLASS	-------------------------*	//
Account::Account(string FirstName, string LastName, int AccountNumber, int SortCode, double Balance) : 
	firstName(FirstName), lastName(LastName), accountNumber(AccountNumber), sortCode(SortCode), balance(Balance) {}

Account::~Account() {}

void Account::addCard(string CardNumber, string Pin) {
	CreditCard card = CreditCard(CardNumber, Pin);
	creditCards.emplace_back(card);
}

vector<CreditCard> Account::getCards() {
	return creditCards;
}

CreditCard Account::getCardFromCardNumber(string CardNumber) {
	for (CreditCard card : creditCards) {
		if (card.getCardNumber() == CardNumber) {
			return card;
		}
	}
}

string Account::getFirstName() {
	return firstName;
}

string Account::getLastName() {
	return lastName;
}

string Account::getFullName() {
	return firstName + " " + lastName;
}

int Account::getAccountNumber() {
	return accountNumber;
}

int Account::getSortCode() {
	return sortCode;
}

double Account::getBalance() {
	return balance;
}

/*
* Account::withdrawCash 
* Return codes:
* 0: No balance
* 1: Successful
* 2: Above maximum withdraw limit
* 3: Too low of an amount to withdraw
*/
int Account::withdrawCash(double amount) {
	if (amount < 0) {
		return 0;
	}
	if (amount < MIN_DEPOSIT) {
		return 3;
	}
	if ((balance - amount) > 0) {
		if (amount > MAX_DEPOSIT) {
			return 2;
		}
		else {
			balance = balance - amount;
			return 1;
		}
	}
	return 0;
}

/*
* Account::depositCash
* Return codes:
* 0: No balance
* 1: Successful
* 2: Above maximum deposit limit
* 3: Too low of an amount to deposit
* 4: The end result balance exceeds maximum balance limit
*/
int Account::depositCash(double amount) {
	if (amount < 0) {
		return 0;
	}
	if (amount < MIN_DEPOSIT) {
		return 3;
	}
	if ((balance + amount) > MAX_BALANCE) {
		return 4;
	}
	else {
		if (amount > MAX_DEPOSIT) {
			return 2;
		}
		else {
			balance = balance + amount;
			return 1;
		}
	}
}


//	*------------------------	CREDIT CARD CLASS	-------------------------*	//
CreditCard::CreditCard(string CardNumber, string Pin) : cardNumber(CardNumber), pin(Pin) {}

CreditCard::~CreditCard() {}

string CreditCard::getCardNumber() {
	return cardNumber;
}

bool CreditCard::verifyPin(string Pin) {
	if (Pin == pin) {
		return true;
	}
	else {
		return false;
	}
}


//	*------------------------	BANK CLASS	-------------------------*	//
Bank::Bank(string Name) : name(Name) {}

Bank::~Bank() {}

string Bank::getName() {
	return name;
}

void Bank::addAccount(Account account) {
	accounts.push_back(account);
}

Account Bank::getAccountFromCardNumber(string CardNumber) {
	for (Account account : accounts) {
		vector<CreditCard> accountCards = account.getCards();
		for (CreditCard creditCard : accountCards) {
			if (creditCard.getCardNumber() == CardNumber) {
				return account;
			}
		}
	}
}

Account Bank::getAccountFromAccountDetails(int AccountNumber, int SortCode) {
	for (Account account : accounts) {
		if (account.getAccountNumber() == AccountNumber && account.getSortCode() == SortCode) {
			return account;
		}
	}
}


//	*------------------------	ATM CLASS	-------------------------*	//
ATM::ATM(Bank Bank_) : bank(Bank_) {}

ATM::~ATM() {
	delete user;
}

bool is_digits(std::string& str) {
	return str.find_first_not_of("0123456789") == std::string::npos;
}

bool is_double(std::string& str) {
	return str.find_first_not_of(".0123456789") == std::string::npos;
}

double round(double value) {
	string num_text = to_string(value);
	string rounded = num_text.substr(0, num_text.find(".") + 3);
	return stod(rounded);
}

string double_to_string(double value) {
	string num_text = to_string(value);
	string rounded = num_text.substr(0, num_text.find(".") + 3);
	return rounded;
}

void EndOfSequence(ATM* atm) {
	cout << "Press any key to return to menu" << "\n";
	_getch();
}

void ATM::withdraw() {
	cout << "Would you like to see your balance before withdrawing?" << "\n";
	do {
		cout << "Enter Y or N: ";
		string choice;
		cin >> choice;
		for (char & c : choice) {
			c = toupper(c);
		}
		if (choice == "Y") {
			cout << "Current balance: $" << user->getBalance() << "\n";
			break;
		}
		else if (choice == "N") {
			break;
		}
		else {
			cout << "Invalid input." << "\n";
		}
	} while (true);
	cout << "Enter amount of cash to withdraw: $";
	string input;
	cin >> input;
	if (is_double(input) && input.length() < MAX_NUMBER_LENGTH) {
		double amount = stod(input);
		amount = round(amount);
		int success = user->withdrawCash(amount);
		switch (success) {
		case 1:
			cout << "Successfully withdrawed $" << amount << "!\n";
			createTransaction("Withdraw", amount);
			break;
		case 2:
			cout << "Exceeded maximum withdraw amount! ($" << MAX_DEPOSIT << ")\n";
			break;
		case 3:
			cout << "Minimum amount to withdraw must be or exceed $" << MIN_DEPOSIT << "\n";
			break;
		default:
			cout << "Not enough balance." << "\n";
			break;
		}
	}
	EndOfSequence(this);
}

void ATM::deposit() {
	cout << "How much would you like to deposit into your account?" << "\n";
	cout << "Enter amount: $";
	string input;
	cin >> input;
	if (is_double(input) && input.length() < MAX_NUMBER_LENGTH) {
		double amount = stod(input);
		amount = round(amount);
		int success = user->depositCash(amount);
		switch (success) {
		case 1:
			cout << "Successfully deposited $" << amount << "!\n";
			createTransaction("Deposit", amount);
			break;
		case 2:
			cout << "Exceeded maximum deposit amount! ($" << MAX_DEPOSIT << ")\n";
			break;
		case 3:
			cout << "Minimum amount to deposit must be or exceed $" << MIN_DEPOSIT << "\n";
			break;
		case 4:			
			cout << "Exceeded maximum balance limit! ($" << MAX_BALANCE << ")\n";
			break;
		default:
			cout << "Not enough balance." << "\n";
			break;
		}
	}
	EndOfSequence(this);
}

void ATM::viewBalance() {
	cout << "Current balance: $" << user->getBalance() << "\n";
	EndOfSequence(this);
}

void ATM::createTransaction(string action, double amount) {
	fstream file;
	string fileName = to_string(user->getAccountNumber()) + "_" + to_string(user->getSortCode());
	file.open(LOGS_FOLDER + "/" + fileName + ".txt", std::ios::app);
	if (!file) {
		cout << "WARNING: Error occured - This transaction is not tracked!" << "\n";
	}
	else {
		string output;
		output = output + "------------------------------------" + "\n";
		output = output + "Name: " + user->getFullName() + "\n";
		output = output + "Account Number: " + to_string(user->getAccountNumber()) + "\n";
		output = output + "Sort Code: " + to_string(user->getSortCode()) + "\n";
		output = output + "Credit Card Number: " + currentCardNumber + "\n\n";

		if (action == "Withdraw") {
			output = output + "Action: Withdrew $" + double_to_string(amount) + "\n";
			output = output + "Old balance: $" + double_to_string(user->getBalance() + amount) + "\n";
			output = output + "New Balance: $" + double_to_string(user->getBalance()) + "\n";
		}
		else if (action == "Deposit") {
			output = output + "Action: Deposited $" + double_to_string(amount) + "\n";
			output = output + "Old balance: $" + double_to_string(user->getBalance() - amount) + "\n";
			output = output + "New Balance: $" + double_to_string(user->getBalance()) + "\n";
		}
		file << output;
	}
	file.close();
}

void ATM::viewTransactions() {
	ifstream file;              
	string fileName = to_string(user->getAccountNumber()) + "_" + to_string(user->getSortCode());
	file.open(LOGS_FOLDER + "/" + fileName + ".txt", std::ios::in);
	if (file) {
		std::string line;
		while (std::getline(file, line)) {
			cout << line.c_str() << "\n";
		}
	}
	else {
		cout << "No transactions can be found for this account." << "\n";
	}
	file.close();
	EndOfSequence(this);
}

void ATM::viewDetails() {
	cout << "Current account details:" << "\n";
	cout << "Name: " << user->getFullName() << "\n";
	cout << "Account Number: " << user->getAccountNumber() << "\n";
	cout << "Sort Code: " << user->getSortCode() << "\n";
	cout << "Cards associated with this account:" << "\n";
	for (int i = 0; i < user->getCards().size(); i++) {
		CreditCard card = user->getCards()[i];
		cout << card.getCardNumber() << "\n";
	}
	EndOfSequence(this);
}

void ATM::bankServicesMenu() {
	bool exitFunction = false;
	do {
		cout << "---------------------------" << "\n";
		cout << "Welcome to " << bank.getName() << ", " << user->getFullName() << "!\n";
		cout << "1. Withdraw cash" << "\n";
		cout << "2. Deposit cash" << "\n";
		cout << "3. Check balance" << "\n";
		cout << "4. Check bank details" << "\n";
		cout << "5. Transaction History" << "\n";
		cout << "6. Exit" << "\n";
		bool exitLoop = false;
		do {
			cout << "Enter a valid option below.." << "\n";
			string choice;
			int intChoice;
			cin >> choice;
			if (is_digits(choice) && choice.length() < 2) {
				int intChoice = stoi(choice);
				switch (intChoice) {
				case 1:
					withdraw();
					exitLoop = true;
					break;
				case 2:
					deposit();
					exitLoop = true;
					break;
				case 3:
					viewBalance();
					exitLoop = true;
					break;
				case 4:
					viewDetails();
					exitLoop = true;
					break;
				case 5:
					viewTransactions();
					exitLoop = true;
					break;
				case 6:
					exitLoop = true;
					exitFunction = true;
					break;
				default:
					cout << "Invalid input." << "\n";
				}
			}
			else {
				cout << "Invalid input." << "\n";
			}

		} while (exitLoop == false);
	} while (exitFunction == false);
}

void ATM::login() {
	cout << "Enter your credit card number: ";
	string userCardNumber;
	cin >> userCardNumber;
	Account guest = bank.getAccountFromCardNumber(userCardNumber);
	if (guest.getFirstName() == "") {
		cout << "Account/Credit Card doesn't exist." << "\n";
	} else {
		user = &guest;
		currentCardNumber = userCardNumber;
		int wrong = 0;
		for (int i = 0; i <= 3; i++) {
			cout << "Enter your credit card pin: ";
			string requestedPin;
			cin >> requestedPin;
			CreditCard userCreditCard = user->getCardFromCardNumber(currentCardNumber);
			if (userCreditCard.verifyPin(requestedPin) == true) {
				bankServicesMenu();
				break;
			}
			else {
				wrong++;
				cout << "Wrong pin entered." << "\n";
				cout << (3 - wrong) << " attempts left." << "\n";
				if (wrong == 3) {
					cout << "Too many wrong attempts." << "\n";
					break;
				}
			}
		}
		user = nullptr;
		currentCardNumber = "";
		cout << "Signing out of account" << "\n";
	}
	cout << "Returning to menu." << "\n";
}

void ATM::menu() {
	bool exitProgram = false;
	do {
		cout << "---------------------------" << "\n";
		cout << "Bank ATM" << "\n";
		cout << "1. Use bank services" << "\n";
		cout << "2. Exit ATM" << "\n";
		bool exitLoop = false;
		do {
			cout << "Enter a valid option below.." << "\n";
			string choice;
			int intChoice;
			cin >> choice;
			if (is_digits(choice) && choice.length() < 2) {
				int intChoice = stoi(choice);
				switch (intChoice) {
				case 1:
					login();
					exitLoop = true;
					break;
				case 2:
					exitLoop = true;
					exitProgram = true;
					cout << "Exiting ATM..." << "\n";
					break;
				default:
					cout << "Invalid input." << "\n";
				}
			}
			else {
				cout << "Invalid input." << "\n";
			}

		} while (exitLoop == false);
	} while (exitProgram == false);
}