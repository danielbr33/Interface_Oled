/*
 * SSD1306.cpp
 *
 *  Created on: 22.03.2020
 *      Author: danie
 */

#include "SSD1306.h"

void SSD1306::Reset(void) {
	// CS = High (not selected)
	if(i2c_or_spi==SPI){
		HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);

		// Reset the OLED
		HAL_GPIO_WritePin(RESET_Port, RESET_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(RESET_Port, RESET_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
	}
}
// Send a byte to the command register
void SSD1306::WriteCommand() {
	if (i2c_or_spi==SPI){
		HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET); // select OLED
		HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_RESET); // command
		if (dma_status==SET_ON)
			HAL_SPI_Transmit_DMA(SPI_Port, lineCommands, 3);
		else
			HAL_SPI_Transmit(SPI_Port, lineCommands, 3, HAL_MAX_DELAY);
	}
	else {
		if (dma_status==SET_ON)
			HAL_I2C_Mem_Write_DMA(I2C_Port, I2C_ADDR, 0x00, 1, lineCommands, 3);
		else
			HAL_I2C_Mem_Write(I2C_Port, I2C_ADDR, 0x00, 1, lineCommands, 3, HAL_MAX_DELAY);
	}
}
// Send data
void SSD1306::WriteData() {
	if (i2c_or_spi==SPI){
		HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET); // select OLED
		HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_SET); // data
		if (dma_status==SET_ON)
			HAL_SPI_Transmit_DMA(SPI_Port, &SSD1306_Buffer[width*counter], width);
		else
			HAL_SPI_Transmit(SPI_Port, &SSD1306_Buffer[width*counter], width, HAL_MAX_DELAY);
	}
	else{
		if (dma_status==SET_ON)
			HAL_I2C_Mem_Write_DMA(I2C_Port, I2C_ADDR, 0x40, 1, &SSD1306_Buffer[width*counter], width);
		else
			HAL_I2C_Mem_Write(I2C_Port, I2C_ADDR, 0x40, 1, &SSD1306_Buffer[width*counter], width, HAL_MAX_DELAY);
	}
}


void SSD1306::SPI_Interrupt_DMA(){
	//if (dma_status == 1){
		if (status==2);
		else if (status==0){
			lineCommands[0]=SET_PAGE_START_ADDR + counter;
			lineCommands[1]=LOW_COLUMN_ADDR;
			lineCommands[2]=HIGH_COLUMN_ADDR;
			status=1;
			WriteCommand();
		}
		else{
			status=0;
			counter+=1;
			if (counter==8)
				counter=0;
			WriteData();
		}
	//}
}

void SSD1306::Init(void) {
	// Reset OLED
	Reset();
    // Wait for the screen to boot
    HAL_Delay(100);

    // Init OLED
    initCommands[0]=TURN_OFF;

    initCommands[1]=SET_MEMORY_ADDR_MODE;
    initCommands[2]=HORIZONTAL_ADDR_MODE;

    initCommands[3]=SET_PAGE_START_ADDR;

	if (mirror_vertical_status == SET_ON)
		initCommands[4]=MIRROR_VERTICAL;
	else
		initCommands[4]=COM_SCAN_DIRECTION;

    initCommands[5]=LOW_COLUMN_ADDR;
    initCommands[6]=HIGH_COLUMN_ADDR;

    initCommands[7]=SET_START_LINE_ADDR;

    initCommands[8]=SET_CONTRAST;
    initCommands[9]=CONTRAST;

	if (mirror_horizontal_status == SET_ON)
		initCommands[10]=MIRROR_HORIZONTAL;
	else
		initCommands[10]=SET_SEGMENT_REMAP;

	if (inversion_color_status == SET_ON)
		initCommands[11]=INVERSE_COLOR;
	else
		initCommands[11]=NORMAL_COLOR;

    initCommands[12]=SET_MULTIPLEX_RATIO;
	if (height == 32)
		initCommands[13]=RATIO_32;
	else if (height == 64)
		initCommands[13]=RATIO_64;

    initCommands[14]=OUT_FOLLOW_RAM_CONTENT;

    initCommands[15]=DISPLAY_OFFSET;
    initCommands[16]=DISPLAY_NOT_OFFSET;

    initCommands[17]=SET_CLOCK_DIVIDE_RATIO;
    initCommands[18]=DIVIDE_RATIO;

    initCommands[19]=SET_PRE_CHARGE_PERIOD;
    initCommands[20]=PRE_CHARGE_PERIOD;

    initCommands[21]=SET_COM_PIN;
	if (height == 32)
		initCommands[22]=COM_PIN_32;
	else if (height == 64)
		initCommands[22]=COM_PIN_64;

    initCommands[23]=SET_VCOMH;
    initCommands[24]=VOLTAGE_77;

    initCommands[25]=SET_DC_ENABLE;
    initCommands[26]=DC_ENABLE;
    initCommands[27]=TURN_ON;

    status=2;
    currentX = 0;
    currentY = 0;
    initialized = 1;

    Fill(WHITE);
    if (dma_status==SET_ON)
    	HAL_SPI_Transmit_DMA(SPI_Port, initCommands, 28);
    else
    	HAL_SPI_Transmit(SPI_Port, initCommands, 28, HAL_MAX_DELAY);
    status=0;
    SPI_Interrupt_DMA();
}

void SSD1306::process(){
	//components to display
	HAL_Delay(5);
}

// Fill the whole screen with the given color
void SSD1306::Fill(SSD1306_COLOR color) {
    /* Set memory */
    uint32_t i;

    for(i = 0; i < width * height /8; i++) {
        SSD1306_Buffer[i] = (color == BLACK) ? 0x00 : 0xFF;
    }
}

//    Draw one pixel in the screenbuffer
//    X => X Coordinate
//    Y => Y Coordinate
//    color => Pixel color
void SSD1306::DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= width || y >= height) {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if(inverted) {
        color = (SSD1306_COLOR)!color;
    }

    // Draw in the right color
    if(color == WHITE) {
        SSD1306_Buffer[x + (y / 8) * width] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * width] &= ~(1 << (y % 8));
    }
}

// Draw 1 char to the screen buffer
// ch       => char om weg te schrijven
// Font     => Font waarmee we gaan schrijven
// color    => BLACK or WHITE
char SSD1306::WriteChar(char ch, FontDef Font, SSD1306_COLOR color) {
    uint32_t i, b, j;

    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;

    // Check remaining space on current line
    if (width < (currentX + Font.FontWidth) ||
        height < (currentY + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Use the font to write
    for(i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for(j = 0; j < Font.FontWidth; j++) {
            if((b << j) & 0x8000)  {
                DrawPixel(currentX + j, (currentY + i), (SSD1306_COLOR) color);
            } else {
                DrawPixel(currentX + j, (currentY + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    currentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}

// Write full string to screenbuffer
char SSD1306::WriteString(char* str, FontDef Font, SSD1306_COLOR color) {
    // Write until null-byte
    while (*str) {
        if (WriteChar(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

// Position the cursor
void SSD1306::SetCursor(uint8_t x, uint8_t y) {
    currentX = x;
    currentY = y;
}

SSD1306::SSD1306(I2C_HandleTypeDef* i2c, int I2C_ADDRESS){
	this->I2C_Port=i2c;
	this->I2C_ADDR=I2C_ADDR;
	this->dma_status=SET_OFF;
	this->mirror_vertical_status = SET_OFF;
	this->mirror_horizontal_status = SET_OFF;
	this->inversion_color_status = SET_OFF;
	this->height=64;
	this->width=128;
	i2c_or_spi=I2C;
	counter=7;
	AllocBuffer();
}

void SSD1306::AllocBuffer(){
	this->SSD1306_Buffer=new uint8_t[width * height /8];
}

void SSD1306::ChangeDMA(state dma){
	dma_status=dma;
}

void SSD1306::ChangeMirrorHorizontal(state mirror){
	mirror_horizontal_status = mirror;
}

void SSD1306::ChangeMirrorVertical(state mirror){
	mirror_vertical_status = mirror;
}

void SSD1306::ChangeInversionColor(state inversion){
	inversion_color_status = inversion;
}

void SSD1306::ChangeHeight(uint8_t height){
	this->height=height;
}

void SSD1306::ChangeWidth(uint8_t width){
	this->width=width;
}

SSD1306::SSD1306(SPI_HandleTypeDef* hspi, gpio_struct reset, gpio_struct DC,
			gpio_struct CS) {
	this->SPI_Port = hspi;
	this->RESET_Port = reset.port;
	this->RESET_Pin = reset.pin;
	this->CS_Port = CS.port;
	this->CS_Pin = CS.pin;
	this->DC_Port = DC.port;
	this->DC_Pin = DC.pin;
	this->dma_status=SET_OFF;
	this->mirror_vertical_status = SET_OFF;
	this->mirror_horizontal_status = SET_OFF;
	this->inversion_color_status = SET_OFF;
	this->height=64;
	this->width=128;
	i2c_or_spi=SPI;
	counter=7;
	AllocBuffer();
}

SSD1306::~SSD1306() {
	// TODO Auto-generated destructor stub
}