#include "Interface.h"
#include "stm32f3xx_hal.h"
#include "SSD1306.h"

#ifndef INTERFACEMANAGER_H_
#define INTERFACEMANAGER_H_

extern SSD1306* oled;

class Interface_manager{
public:
	Interface_manager(UART_HandleTypeDef* huart) ;
	void interrupt();
	void init();
private:
	Interface *Ssd_1306 ;
	Interface_Element::Button readKey() ;
	void display() ;
	uint8_t button;
	UART_HandleTypeDef* uart_interface;
};
#endif /* INTERFACEMANAGER_H_ */
