// Name:  Jairo Vera
// PS ID: 1170501
// COSC 4330 Spring 2015 Third Assignment
// This is the .h header file for the Customer class
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <ostream>
#include <string>
using std::ostream;
using std::string;

class Customer {
public:
	Customer(string name, int elapsed_sec, int process_sec);
	friend ostream& operator<< (ostream& out, const Customer& customer);

	// Getters
	string get_name();
	int get_elapsed_sec();
	int get_process_sec();

private:
	string name;
	int elapsed_sec;
	int process_sec;
};

#endif
