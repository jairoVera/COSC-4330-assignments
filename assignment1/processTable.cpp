/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* processTable.cpp
* CPP file for the Process Table class. The class encapsulates & handles
* access to the process table.
*/
#include <iostream>
#include <cstdlib>
#include <cassert>
#include "processTable.h"

void ProcessTable::append(Process process){
	processTable.push_back(process);
}

Process& ProcessTable::operator[] (const int index){
	assert (index >=0 && index < getSize());
	return processTable[index];
}

Process& ProcessTable::getProcess(const int processNumber){
	if (getSize() == 0) {
		std::cout<<"getProcess() dealt with size of 0.\n";
		std::cout<<"Program will terminate . . .\n";
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < getSize(); i++) {
		if (processTable[i].getID() == processNumber)
			return processTable[i];
	}

	// The process search failed
	std::cout<<"ERROR. ProcessTable could not find Process "<< processNumber <<".\n";
	std::cout<<"Program will terminate . . .\n";
	exit(EXIT_FAILURE);
}

/**
* Get process ID of a process that begins at current time.
* Returns -1 if no process beings at this time.
*/
int ProcessTable::getID_BeginsAtTime(int currentTime){
	for (int i = 0; i < getSize(); i++){
		if (processTable[i].getStartTime() == currentTime)
			return processTable[i].getID();
	}
	return -1;
}

/**
* Get arrival time of the 1st process that begins AFTER current time.
* Returns -1 if no process beings at this time.
*/
int ProcessTable::getBeginTime_AfterTime(int currentTime){
	for (int i = 0; i < getSize(); i++){
		if (processTable[i].getStartTime() > currentTime)
			return processTable[i].getStartTime();
	}
	return -1;
}

int ProcessTable::getSize() const {
	return static_cast<int>(processTable.size());
}

void ProcessTable::print(int terminatedID){
	std::cout<<"Process ID\tStart Time\tCPU Time\tStatus\n";
	for (unsigned int i = 0; i < getSize(); i++) {
		if ( (processTable[i].getStatus() != TERMINATED ) ||
			 (processTable[i].getID() == terminatedID) )
		{
			std::cout<< processTable[i];
		}
	}
}
