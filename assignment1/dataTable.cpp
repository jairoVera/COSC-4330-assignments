/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* dataTable.cpp
* The .cpp file for the DataTable class. The class encapsulates & handles
* access to the data table.
*/
#include <iostream>
#include <cassert>
#include "dataTable.h"

void DataTable::append(Data dataEntry){
	dataTable.push_back(dataEntry);
}

Data& DataTable::operator[] (const int index){
	assert (index >= 0 && index < getSize());
	return dataTable[index];
}

int DataTable::getSize() const {
	return static_cast<int>(dataTable.size());
}

ostream& operator<< (ostream& out, const DataTable& table){
	for (int i = 0; i < table.getSize(); i++)
		std::cout<< table.dataTable[i];
	return out;
}
