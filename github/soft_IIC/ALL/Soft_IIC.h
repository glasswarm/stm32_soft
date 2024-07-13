#ifndef __SOFT_IIC_H
#define __SOFT_IIC_H

#include "stm32f1xx_hal.h"

#define Port_SCL GPIOB
#define Port_SDA GPIOB
#define IO_SCL GPIO_PIN_0
#define IO_SDA GPIO_PIN_1

#define	SET_SCL()  HAL_GPIO_WritePin(Port_SCL,IO_SCL,BitValue);
#define	SET_SDA()  HAL_GPIO_WritePin(Port_SDA,IO_SDA,BitValue);

#define	Read_SDA() HAL_GPIO_ReadPin(Port_SDA,IO_SDA);

void MYI2C_Init();
void MyI2C_Start();
void MyI2C_Stop();
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_RecByte();
void MyI2C_SendACK(uint8_t ACKBit);
uint8_t MyI2C_RecvACK();

#endif
