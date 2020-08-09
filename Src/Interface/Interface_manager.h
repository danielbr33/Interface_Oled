#include "Interface.h"
#include "usart.h"
#include "SSD1306.h"

#ifndef INTERFACEMANAGER_H_
#define INTERFACEMANAGER_H_

extern SSD1306* oled;

class Interface_manager{
public:
	Interface_manager() ;
	void interrupt();
private:
	Interface *Ssd_1306 ;
	Interface_Element::Button readKey() ;
	void display() ;
	uint16_t button;
};
#endif /* INTERFACEMANAGER_H_ */
