/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* dataTable.h
* The .h file for the DataTable class.
*/
#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include <vector>
#include "data.h"
using std::vector;

class DataTable{
public:
	void append(Data dataEntry);
	Data& operator[] (const int index);
	int getSize() const;
	friend ostream& operator<< (ostream& out, const DataTable& table);

private:
	vector<Data> dataTable;
};

#endif	//DATA_TABLE_H
