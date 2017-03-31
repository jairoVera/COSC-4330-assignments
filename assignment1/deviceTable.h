/**
* Name:   	Jairo Vera
* PS ID:  	1170501
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* deviceTable.h
* The .h file for the DeviceTable class.
*/

#ifndef DEVICE_TABLE_H
#define DEVICE_TABLE_H

#include<vector>
#include "device.h"
using std::vector;
using std::ostream;

class DeviceTable{
public:
	void append(Device device);
	Device& operator[] (const int index);
	int getSize() const;
	friend ostream& operator<< (ostream& out, const DeviceTable& table);
	
private:
	vector<Device> deviceTable;
};

#endif	//DEVICE_TABLE_H
