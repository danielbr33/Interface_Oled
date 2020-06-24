#include "Interface1.h"

#ifndef INTERFACE1MANAGER_H_
#define INTERFACE1MANAGER_H_
#include <conio.h>

class Interface1_manager{
public:
	Interface1_manager() ;
    void print(Interface_Element::Action info) ;
private:
	Interface1 *Ssd_1306 ;
	Interface_Element::Button readKey() ;
};
#endif /* INTERFACE1MANAGER_H_ */
