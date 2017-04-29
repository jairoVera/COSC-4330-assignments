/**
* Name:   	Jairo Vera
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* data.h
* Header file for data.cpp
*/
#ifndef DATA_H
#define DATA_H

#include <ostream>
using std::string;
using std::ostream;

class Data{
public:
    Data(string operation, int parameter);
    Data();

    string getOperation() const;
    int getParameter() const;

	friend ostream& operator<< (ostream& out, const Data& data);

private:
    string operation;
    int parameter;
};

#endif //DATA_H
