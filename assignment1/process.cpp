/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* process.cpp
* CPP file for the Process class. This class is an entry to the
* Process Table class.
*/
#include <cstdlib>
#include <string>
#include "process.h"

Process::Process(int id, ProcessStatus status, int startTime, int firstLine){
	this->id = id;
	this->status = status;
	this->startTime = startTime;
	this->firstLine = firstLine;
	this->cpuTime = 0;
}

Process::Process(){
	std::cout<<"ERROR. Program called Default Process Constructor.\n";
	std::cout<<"Program will now terminate . . . \n";
	exit(EXIT_FAILURE);
}

int Process::getID() const {
	return id;
}

ProcessStatus Process::getStatus() const {
	return status;
}

int Process::getStartTime() const {
	return startTime;
}

int Process::getFirstLine() const {
	return firstLine;
}

int Process::getLastLine() const {
	return lastLine;
}

int Process::getCurrentLine() const {
	return currentLine;
}

int Process::getCPUTime() const {
	return cpuTime;
}

int Process::getTotalCPUTime() const {
	return totalCPUTime;
}

void Process::setStatus(ProcessStatus status){
	this->status = status;
}

void Process::setLastLine(int lastLine){
	this->lastLine = lastLine;
}

void Process::setCurrentLine(int currentLine){
	this->currentLine = currentLine;
}

void Process::incrementCPUTime() {
	cpuTime++;
}

void Process::setCPUTime(int cpuTime){
	this->cpuTime = cpuTime;
}
void Process::setTotalCPUTime(int totalCPUTime) {
	this->totalCPUTime = totalCPUTime;
}

ostream& operator<< (ostream& out, const Process& process){
	std::string stringStatus;
	if (process.status == NEW)
		stringStatus = "NEW";
	else if (process.status == READY)
		stringStatus = "READY";
	else if (process.status == RUNNING)
		stringStatus = "RUNNING";
	else if (process.status == WAITING)
		stringStatus = "WAITING";
	else if (process.status == TERMINATED)
		stringStatus = "TERMINATED";

	out<< process.id <<"\t\t"<< process.startTime <<"\t\t"<< process.cpuTime <<"\t\t"<< stringStatus <<"\n";
	return out;
}
