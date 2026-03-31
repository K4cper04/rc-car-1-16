/*
 * oled.c
 *
 *  Created on: 27 mar 2026
 *      Author: kacpe
 */

#include "oled.h"
#include "spi.h"
#include "images.h"

void OLED_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET); // DC na 0 = Komenda
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET); // CS na 0 = Wybór układu
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);   // CS na 1 = Koniec
}

void OLED_Data(uint8_t data)
{
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);   // DC na 1 = Dane
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void OLED_Init(void) {
	// https://www.waveshare.com/w/upload/e/e3/1.3inch-SH1106-OLED.pdf
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    OLED_Command(0xAE); // Display OFF
    OLED_Command(0x40); // start line
    OLED_Command(0x81); // contrast
    OLED_Command(0x7F); //
    OLED_Command(0xA1); // Segment re-map
    OLED_Command(0xA6); // normal display
    OLED_Command(0xA8); // multiplex ratio
    OLED_Command(0x3F); // duty = 1/64
    OLED_Command(0xC8); // com scan direction
    OLED_Command(0xD3); // display offset
    OLED_Command(0x00);	// 0x20
    OLED_Command(0xAF); // display ON

    OLED_Clear(); // Wyczyść śmieci z RAMu ekranu
}

void OLED_Clear(void) {
    OLED_Fill(0x00);
}

void OLED_Fill(uint8_t pattern) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_Command(SET_PAGE_ADDR + i);
        OLED_Command(SET_COL_ADDR_LOW);
        OLED_Command(SET_COL_ADDR_HIGH);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_Data(pattern);
        }
    }
}

void OLED_DrawImage(const uint8_t* image) {
    for (uint8_t p = 0; p < 8; p++) {
        OLED_Command(0xB0 + p);
        OLED_Command(0x02);
        OLED_Command(0x10);
        for (uint16_t i = 0; i < 128; i++) {
            OLED_Data(image[p * 128 + i]);
        }
    }
}

void OLED_DrawTile(uint8_t x, uint8_t page, uint8_t width, uint8_t height, const uint8_t* tile)
{
	uint8_t rows = height / 8;

	for (uint8_t i = 0; i < rows; i++)
	{
		OLED_Command(0xB0 + page + i);
		OLED_Command(0x02 + (x & 0x0F));
		OLED_Command(0x10 + ((x & 0xF0) >> 4));

		for (uint8_t j = 0; j < width; j++)
		{
			OLED_Data(tile[i * width + j]);
		}
	}
}

void OLED_DisplayTime(uint8_t min, uint8_t sec)
{
	uint8_t m1 = min / 10;
	uint8_t m2 = min % 10;
	uint8_t s1 = sec / 10;
	uint8_t s2 = sec % 10;

	OLED_DrawTile(103, 6, 4, 8, font_8[m1]);
	OLED_DrawTile(108, 6, 4, 8, font_8[m2]);
	OLED_DrawTile(113, 6, 4, 8, colon);
	OLED_DrawTile(116, 6, 4, 8, font_8[s1]);
	OLED_DrawTile(121, 6, 4, 8, font_8[s2]);
}

void OLED_DisplaySpeed(uint8_t speed)
{
	uint8_t tens = speed / 10;
	uint8_t units = speed % 10;

	OLED_DrawTile(51, 2, 10, 24, font_24[tens]);
	OLED_DrawTile(64, 2, 10, 24, font_24[units]);
	OLED_DrawTile(76, 2, 10, 24, ms);

}
