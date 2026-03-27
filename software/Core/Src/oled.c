/*
 * oled.c
 *
 *  Created on: 27 mar 2026
 *      Author: kacpe
 */

#include "oled.h"
#include "spi.h"


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
    // Reset sprzętowy
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    // Inicjalizacja SH1106
    OLED_Command(0xAE); // Display OFF
    OLED_Command(0xA1); // Segment re-map
    OLED_Command(0xC8); // COM scan direction
    OLED_Command(0xA8); // Multiplex ratio
    OLED_Command(0x3F);
    OLED_Command(0xD3); // Display offset
    OLED_Command(0x00);
    OLED_Command(0x40); // Start line
    OLED_Command(0x81); // Contrast
    OLED_Command(0x7F);
    OLED_Command(0xA6); // Normal display
    OLED_Command(0xAF); // Display ON

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
