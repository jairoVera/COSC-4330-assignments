/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* process.h
* Header file for the Process class.
*/

#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
using std::ostream;

enum ProcessStatus {NEW, READY, RUNNING, WAITING, TERMINATED};

class Process{
public:
	Process(int id, ProcessStatus status, int startTime, int firstLine);
	Process();

	int getID() const;
	ProcessStatus getStatus() const;
	int getStartTime() const;
	int getFirstLine() const;
	int getLastLine() const;
	int getCurrentLine() const;
	int getCPUTime() const;
	int getTotalCPUTime() const;

	void setStatus(ProcessStatus status);
	void setLastLine(int lastLine);
	void setCurrentLine(int currentLine);
	void incrementCPUTime();
	void setCPUTime(int cpuTime);
	void setTotalCPUTime(int totalCPUTime);

	friend ostream& operator<< (ostream& out, const Process& process);

private:
	int id;
	ProcessStatus status;
	int startTime;
	int firstLine;
	int lastLine;
	int currentLine;
	int cpuTime;
	int totalCPUTime;
};

#endif	//PROCESS_H
