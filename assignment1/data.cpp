/**
* Name:   	Jairo Vera
* PS ID:  	1170501
* Course: 	COSC 4330
* Semester: Spring 2015
* Classification: Junior (Undergraduate)
*
* data.cpp
* The .cpp file for the Data class, which is an entry for the DataTable class.
*/
#include <iostream>
#include <cstdlib>
#include "data.h"

Data::Data(string operation, int parameter){
    this->operation = operation;
    this->parameter = parameter;
}

Data::Data(){
    std::cout<<"Error. Program called Default Data Constructor.\n";
    std::cout<<"Program will terminate . . .\n";
    exit(EXIT_FAILURE);
}

string Data::getOperation() const {
    return operation;
}

int Data::getParameter() const {
    return parameter;
}

ostream& operator<< (ostream& out, const Data& data){
	out<< data.operation <<"\t"<< data.parameter <<"\n";
	return out;
}
