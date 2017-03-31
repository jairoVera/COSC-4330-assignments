/**
* Name:   	Jairo Vera
* PS ID:  	1170501
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* device.cpp
* The .cpp file for the Device class. The class is an entry for the
* DeviceTable class.
*/
#include <iostream>
#include <cstdlib>
#include "device.h"

Device::Device(string name){
	this->name = name;
	this->flag = FREE;
	this->processNumber = -1;
	this->completionTime = -1;
}

Device::Device(){
    std::cout<<"Error. Called Default Device Constructor.\n";
    std::cout<<"Program will terminate . . .\n";
    exit(EXIT_FAILURE);
}

string Device::getName() const {
	return name;
}

Flag Device::getFlag() const {
	return flag;
}

int Device::getProcessNumber() const {
	return processNumber;
}

int Device::getCompletionTime() const {
	return completionTime;
}

void Device::setFlag(Flag flag) {
	this->flag = flag;
}

void Device::setProcessNumber(int processNumber) {
	this->processNumber = processNumber;
}

void Device::setCompletionTime(int completionTime) {
	this->completionTime = completionTime;
}

ostream& operator<< (ostream& out, const Device& device){
	string stringFlag;
	if (device.flag == BUSY)
		stringFlag = "BUSY";
	else
		stringFlag = "FREE";
	
	out<<"Name: "<< device.name <<"\tStatus: "<< stringFlag 
	   <<"\tProcess Number: "<< device.processNumber 
	   <<"\tCompletion Time: " << device.completionTime <<"\n";
	
	return out;
}
