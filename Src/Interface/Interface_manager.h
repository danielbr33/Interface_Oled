#include "Interface.h"
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f3xx_hal.h"
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
	uint8_t button;
};
#endif /* INTERFACEMANAGER_H_ */
