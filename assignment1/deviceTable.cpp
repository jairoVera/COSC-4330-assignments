/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* deviceTable.cpp
* The .cpp file for the DeviceTable class. The class encapsulates & handles
* access to the device table.
*/
#include <iostream>
#include <cassert>
#include "deviceTable.h"

void DeviceTable::append(Device device){
	deviceTable.push_back(device);
}

Device& DeviceTable::operator[] (const int index){
	assert (index >= 0 && index < deviceTable.size());
	return deviceTable[index];
}

int DeviceTable::getSize() const {
	return static_cast<int>(deviceTable.size());
}

ostream& operator<< (ostream& out, const DeviceTable& table){
	for (int i = 0; i < table.getSize(); i++)
		std::cout<< table.deviceTable[i];
	return out;
}
