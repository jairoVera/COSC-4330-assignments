/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* device.h
* The .h file for the Device class.
*/
#ifndef DEVICE_H
#define DEVICE_H

#include <string>
using std::string;
using std::ostream;

enum Flag {BUSY, FREE};

class Device {
public:
	Device(string name);
	Device();

	string getName() const;
	Flag getFlag() const;
	int getProcessNumber() const;
	int getCompletionTime() const;

	void setFlag(Flag flag);
	void setProcessNumber(int processNumber);
	void setCompletionTime(int completionTime);

	friend ostream& operator<< (ostream& out, const Device& device);

private:
	string name;
	Flag flag;
	int processNumber;
	int completionTime;
};

#endif	//DEVICE_H
