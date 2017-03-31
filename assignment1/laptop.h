/**
* Name:   	Jairo Vera
* PS ID:  	1170501
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* laptop.h
* Header file for the Laptop class.
*/
#ifndef LAPTOP_H
#define LAPTOP_H

#include <list>
#include <vector>
#include <queue>
#include "dataTable.h"
#include "deviceTable.h"
#include "processTable.h"
using std::list;
using std::vector;
using std::queue;

class Laptop{
public:
	Laptop();
	void simulateProcessScheduling();
	
private:
	void fillDataTable();
	void fillProcessTable();
	void fillProcessList();
	
	void checkForNewProcesses();
	void freeCores();
	void freeDisk_AND_Input();
	void fillCores();
	void fillDisk_AND_Input();
		
	int attemptFreeDevice(Device& device);
	void predictNextEvent();
	
	void printReport(int terminatedID);
	void printQueue(queue<int> queue, string name);
	
	ProcessTable processTable;
	DeviceTable deviceTable;
	DataTable dataTable;
	
	queue<int> readyQueue;
	queue<int> diskQueue;
	queue<int> inputQueue;
	
	int currentTime;
	int totalCPUTime;
	int activeProcesses;
	
	int nextProcessTime;
};

#endif	//LAPTOP_H
