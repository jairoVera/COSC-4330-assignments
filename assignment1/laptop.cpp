/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* laptop.cpp
* CPP file for the Laptop class. This is the main class, where
* the other classes interact with one another to simulate
* the process scheduling.
*/
#include <iostream>
#include "laptop.h"
using std::cout;
using std::string;

Laptop::Laptop(){
	Device core0("CORE 0");
	Device core1("CORE 1");
	Device core2("CORE 2");
	Device core3("CORE 3");
	Device disk("DISK");
	Device input("INPUT");

	deviceTable.append(core0);	// deviceTable[0] = core0
	deviceTable.append(core1);	// deviceTable[1] = core1
	deviceTable.append(core2);	// deviceTable[2] = core2
	deviceTable.append(core3);	// deviceTable[3] = core3
	deviceTable.append(disk);	// deviceTable[4] = disk
	deviceTable.append(input);	// deviceTable[5] = input

	currentTime = 0;
	totalCPUTime = 0;
	activeProcesses = 0;

	nextProcessTime = 0;
}

void Laptop::simulateProcessScheduling(){
	fillDataTable();
	fillProcessTable();

	while (activeProcesses != 0){
		checkForNewProcesses();
		freeCores();
		freeDisk_AND_Input();
		fillCores();
		fillDisk_AND_Input();
		predictNextEvent();
	}
}

void Laptop::fillDataTable(){
    string operation;
    int parameter;

	while (std::cin>>operation && std::cin>>parameter){
        Data data(operation, parameter);
        dataTable.append(data);
	}
}

void Laptop::fillProcessTable(){
	int lineNumber = 0;
	for (int i = 0; i < dataTable.getSize(); i++){
		if (dataTable[i].getOperation() == "NEW"){

			// Fill Process Table
			int startTime = dataTable[i+1].getParameter();
			Process newProcess(dataTable[i].getParameter(), NEW, startTime, lineNumber+1);
			processTable.append(newProcess);
		}
		lineNumber++;
	}

	// calculate the last line for each process object
	lineNumber = 0;
	for (int i = 0; i < processTable.getSize(); i++){
		if ( i == processTable.getSize() - 1)	// last process
			processTable[i].setLastLine(dataTable.getSize() - 1);
		else
			processTable[i].setLastLine(processTable[i+1].getFirstLine() - 2);

		processTable[i].setCurrentLine(processTable[i].getFirstLine());
	}

	activeProcesses = processTable.getSize();
}

void Laptop::checkForNewProcesses(){
	int ID = processTable.getID_BeginsAtTime(currentTime);

	// If a process indeed begins now
	if (ID != -1) {
		// Update New Process to READY State
		processTable.getProcess(ID).setStatus(READY);

		// Increment CurrentLine by 1. This sets the current line to FIRST CPU process
		int currentLine = processTable.getProcess(ID).getCurrentLine();
		processTable.getProcess(ID).setCurrentLine(currentLine + 1);

		// Put new process into ready queue
		readyQueue.push(ID);
	}

	// Predict the future
	nextProcessTime = processTable.getBeginTime_AfterTime(currentTime);
}

void Laptop::freeCores(){
	for (int i = 0; i < 4; i++){		   // core0 - core3
		int returnedID = attemptFreeDevice(deviceTable[i]);

		// core was freed
		if (returnedID != -1){
			// Get current and last line of process
			int currentLine = processTable.getProcess(returnedID).getCurrentLine();
			int lastLine = processTable.getProcess(returnedID).getLastLine();

			// If the main Process is not at its last step process
			if (currentLine != lastLine) {
				// Set the freed process to waiting and set its current line to the next INPUT/DISK process
				processTable.getProcess(returnedID).setStatus(WAITING);
				processTable.getProcess(returnedID).setCurrentLine(currentLine + 1);

				// Get the next operation of the current processs
				int currentLine = processTable.getProcess(returnedID).getCurrentLine();
				string operation = dataTable[currentLine].getOperation();
				int parameter = dataTable[currentLine].getParameter();

				if (operation == "INPUT"){
					inputQueue.push(returnedID);
				} else if (operation == "I/O"){

					if (parameter != 0){
						diskQueue.push(returnedID);
					}
					else if (parameter == 0) {
						// Get current and last line of process
						int currentLine = processTable.getProcess(returnedID).getCurrentLine();
						int lastLine = processTable.getProcess(returnedID).getLastLine();

						// If the main Process is not at its last step process
						if (currentLine != lastLine){
							// Set the freed process to READY and set its current line to the next CPU
							processTable.getProcess(returnedID).setStatus(READY);
							processTable.getProcess(returnedID).setCurrentLine(currentLine + 1);
							readyQueue.push(returnedID);
						} else{
							processTable.getProcess(returnedID).setStatus(TERMINATED);
							activeProcesses--;
							printReport(returnedID);
						}
					}
				} else {
					cout<<"ERROR. Program found an illegal operaton.\n";
					cout<<"Program will now terminate . . .";
					exit(EXIT_FAILURE);
				}

			}else{
				processTable.getProcess(returnedID).setStatus(TERMINATED);
				activeProcesses--;
				printReport(returnedID);
			}
		}// BIG IF
	}
}

void Laptop::freeDisk_AND_Input(){
	for (int i = 4; i < 6; i++){		  // disk & I/O
		int returnedID = attemptFreeDevice(deviceTable[i]);

		// Disk or I/O was freed
		if (returnedID != -1){
			// Get current and last line of process
			int currentLine = processTable.getProcess(returnedID).getCurrentLine();
			int lastLine = processTable.getProcess(returnedID).getLastLine();

			// Returned Process is not done yet
			if (currentLine != lastLine){
				// Put the MAIN process in the ready queue & get it ready for the next process step
				processTable.getProcess(returnedID).setStatus(READY);
				processTable.getProcess(returnedID).setCurrentLine(currentLine + 1);
				readyQueue.push(returnedID);
			} else {
				processTable.getProcess(returnedID).setStatus(TERMINATED);
				activeProcesses--;
				printReport(returnedID);
			}
		}
	}
}

void Laptop::fillCores(){
	for (int i = 0; i < 4; i++) {	// core0 - core3
		if (deviceTable[i].getFlag() == FREE) {
			if (!readyQueue.empty()) {
				// Pop first process ID from readyQueue
				int poppedProcess = readyQueue.front();
				readyQueue.pop();

				int currentLine = processTable.getProcess(poppedProcess).getCurrentLine();
				int requestTime = dataTable[currentLine].getParameter();
				/*
				// Update aggregate total CPU time and process total cpu time
				totalCPUTime += requestTime;
				processTable.getProcess(poppedProcess).incrementCPUTime(requestTime);
				*/
				// update core
				deviceTable[i].setFlag(BUSY);
				deviceTable[i].setProcessNumber(poppedProcess);
				deviceTable[i].setCompletionTime(currentTime + requestTime);

				processTable.getProcess(poppedProcess).setStatus(RUNNING);
			}
		}
	}
}

void Laptop::fillDisk_AND_Input(){
	// fill disk
	if (deviceTable[4].getFlag() == FREE) {
		if (!diskQueue.empty()) {
			// Pop first process ID from disk queue
			int poppedProcess = diskQueue.front();
			diskQueue.pop();

			int currentLine = processTable.getProcess(poppedProcess).getCurrentLine();
			int requestTime = dataTable[currentLine].getParameter();

			// update disk
			deviceTable[4].setFlag(BUSY);
			deviceTable[4].setProcessNumber(poppedProcess);
			deviceTable[4].setCompletionTime(currentTime + requestTime);

			processTable.getProcess(poppedProcess).setStatus(WAITING);
		}
	}
	// fill input
	if (deviceTable[5].getFlag() == FREE) {
		if (!inputQueue.empty()) {
			// Pop first process ID from input queue
			int poppedProcess = inputQueue.front();
			inputQueue.pop();

			int currentLine = processTable.getProcess(poppedProcess).getCurrentLine();
			int requestTime = dataTable[currentLine].getParameter();

			// update disk
			deviceTable[5].setFlag(BUSY);
			deviceTable[5].setProcessNumber(poppedProcess);
			deviceTable[5].setCompletionTime(currentTime + requestTime);

			processTable.getProcess(poppedProcess).setStatus(WAITING);
		}
	}
}

/**
* Returns a process ID if the process's time is up.
* Else, returns -1.
*/
int Laptop::attemptFreeDevice(Device& device){
	if (device.getCompletionTime() == currentTime && device.getFlag() == BUSY){
		int doneProcessID = device.getProcessNumber();

		device.setFlag(FREE);
		device.setProcessNumber(-1);
		device.setCompletionTime(-1);

		return doneProcessID;
	}// else
	return -1;
}

void Laptop::predictNextEvent(){
	int nextTime = nextProcessTime;

	for (int i = 0; i < 6; i++) {
		if (deviceTable[i].getCompletionTime() != -1){
			if (nextTime == -1)
				nextTime = deviceTable[i].getCompletionTime();
			else if (deviceTable[i].getCompletionTime() < nextTime)
				nextTime = deviceTable[i].getCompletionTime();
		}
	}

	if (nextTime < 0) {
		for (int i = 0; i < 4; i++) {
			if (deviceTable[i].getFlag() == BUSY) {
				int processID = deviceTable[i].getProcessNumber();
				processTable.getProcess(processID).incrementCPUTime();
				totalCPUTime += 1;
			}
		}
		currentTime++;
	}
	else {
		int timeSkip = nextTime - currentTime;

		for (int i = 0; i < 4; i++) {
			if (deviceTable[i].getFlag() == BUSY) {
				int processID = deviceTable[i].getProcessNumber();
				int pTime = processTable.getProcess(processID).getCPUTime();
				processTable.getProcess(processID).setCPUTime(pTime + timeSkip);
				totalCPUTime += timeSkip;
			}
		}
		currentTime = nextTime;
	}
}

void Laptop::printReport(int terminatedID){
	cout<<"Process "<< terminatedID <<" terminated at t = "<< currentTime <<"\n";
	for (int i = 0; i < 5; i++) {
		cout<< deviceTable[i].getName() <<" is ";
		if (deviceTable[i].getFlag() == FREE)
			cout<<"IDLE\n";
		else
			cout<<"BUSY - Used by Process "<<deviceTable[i].getProcessNumber()<<"\n";
	}
	cout<<"Average number of busy cores: ";
	printf("%.3f\n\n", (double)totalCPUTime / currentTime );
	printQueue(readyQueue, "Ready queue");
	printQueue(diskQueue, "Disk queue");
	cout<<"\n";
	processTable.print(terminatedID);
	cout<<"\n";
}

void Laptop::printQueue(queue<int> queue, string name){
	cout<< name <<" queue contains: ";
	int size = queue.size();
	for (int i = 0; i < size; i++) {
		cout<< queue.front() << " ";
		queue.pop();
	}
	cout<<"\n";
}
