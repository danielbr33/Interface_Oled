#include "Interface.h"

#ifndef INTERFACEMANAGER_H_
#define INTERFACEMANAGER_H_
#include <conio.h>
#include <ctime>
#include <chrono>

class Interface_manager{
public:
	Interface_manager() ;
private:
	Interface *Ssd_1306 ;
	Interface_Element::Button readKey() ;
	void display() ;
};
#endif /* INTERFACEMANAGER_H_ */
