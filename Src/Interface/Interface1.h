#include "List.h"

#ifndef INTERFACE1_H_
#define INTERFACE1_H_

#include <conio.h>

class Interface1{
private :
	Parameter *battery_voltage ;
	Parameter *work_time ;
	Parameter *distance ;
	Parameter *energy_consumed ;
	Parameter *temperature ;

	void get_Value() ;

	List *list_of_elements ;
public:
	Interface1() ;
	Interface_Element::Action getAction(Interface_Element::Button) ;
	void display() ;
	void displayError();
};

#endif /* INTERFACE1_H_ */
