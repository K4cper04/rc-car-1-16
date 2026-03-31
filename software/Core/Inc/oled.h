/*
 * oled.h
 *
 *  Created on: 27 mar 2026
 *      Author: kacpe
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

//#include "stm32f4xx_hal.h"
#include "spi.h"

// Definicje komend SH1106
#define SET_PAGE_ADDR 0xB0
#define SET_COL_ADDR_LOW 0x02
#define SET_COL_ADDR_HIGH 0x10

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Fill(uint8_t pattern);
void OLED_DrawImage(const uint8_t* image);
void OLED_DrawTile(uint8_t x, uint8_t page, uint8_t width, uint8_t heigh, const uint8_t* tile);
void OLED_DisplayTime(uint8_t min, uint8_t sec);
void OLED_DisplaySpeed(uint8_t speed);
void OLED_Command(uint8_t cmd);
void OLED_Data(uint8_t data);

#endif /* INC_OLED_H_ */
