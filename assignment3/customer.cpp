// Name:  Jairo Vera
// PS ID: 1170501
// COSC 4330 Spring 2015 Third Assignment
// This is the .cpp implementation file for the Customer class
#include "customer.h"

Customer::Customer(string name, int elapsed_sec, int process_sec){
	this->name = name;
	this->elapsed_sec = elapsed_sec;
	this->process_sec = process_sec;
}

ostream& operator<< (ostream& out, const Customer& customer){
	out<<"Name: "         << customer.name 		  <<"\t"
	   <<"Delay Time: "   << customer.elapsed_sec <<"\t"
	   <<"Service Time: " << customer.process_sec <<"\t\n";
	return out;
}

// Getters
string Customer::get_name(){
	return name;
}

int Customer::get_elapsed_sec(){
	return elapsed_sec;
}

int Customer::get_process_sec(){
	return process_sec;
}
