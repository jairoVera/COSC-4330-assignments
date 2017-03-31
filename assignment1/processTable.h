/**
* Name:   	Jairo Vera
* PS ID:  	1170501
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* processTable.h
* Header file for the Process Table class.
*/
#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

#include <vector>
#include <ostream>
#include "process.h"
using std::vector;
using std::ostream;

class ProcessTable{
public:
	void append(Process process);
	Process& operator[] (const int index);
	Process& getProcess(const int processNumber);
	
	int getID_BeginsAtTime(int currentTime);
	int getBeginTime_AfterTime(int currentTime);
	int getSize() const;
	
	void print(int terminatedID);
	
private:
	vector<Process> processTable;
};

#endif	//PROCESS_TABLE_H
