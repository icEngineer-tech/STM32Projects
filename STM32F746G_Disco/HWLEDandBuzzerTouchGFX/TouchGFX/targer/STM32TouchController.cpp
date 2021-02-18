/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include <TouchGFXHAL.hpp>
#include <stm32f7xx_hal.h>
#include <touchgfx/hal/OSWrappers.hpp>
#include "../../Drivers/BSP/Components/ft5336/ft5336.h"

static TS_DrvTypeDef* tsDriver;
extern I2C_HandleTypeDef hi2c3;

void STM32TouchController::init()
{
	/* Initialize the TS driver structure */
	    tsDriver = &ft5336_ts_drv;

	    /* Initialize the TS driver */
	    tsDriver->Start(TS_I2C_ADDRESS);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
	/* Checking if the screen has been touched */

	    if (tsDriver)
	    {
	        if (tsDriver->DetectTouch(TS_I2C_ADDRESS))
	        {
	            /* Get each touch coordinates */
	            tsDriver->GetXY(TS_I2C_ADDRESS, (uint16_t*)&y, (uint16_t*)&x);
	            return true;
	        }
	    }
	    return false;
}

static void I2Cx_Error(I2C_HandleTypeDef* i2c_handler, uint8_t Addr)
{
    /* De-initialize the I2C communication bus */
    HAL_I2C_DeInit(i2c_handler);

    /* Re-Initialize the I2C communication bus */
    //I2Cx_Init(i2c_handler);
}

/**
  * @brief  Reads multiple data.
  * @param  i2c_handler : I2C handler
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  MemAddress: Memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
static HAL_StatusTypeDef I2Cx_ReadMultiple(I2C_HandleTypeDef* i2c_handler,
                                           uint8_t Addr,
                                           uint16_t Reg,
                                           uint16_t MemAddress,
                                           uint8_t* Buffer,
                                           uint16_t Length)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Read(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

    /* Check the communication status */
    if (status != HAL_OK)
    {
        /* I2C error occurred */
        I2Cx_Error(i2c_handler, Addr);
    }
    return status;
}

/**
  * @brief  Writes a value in a register of the device through BUS in using DMA mode.
  * @param  i2c_handler : I2C handler
  * @param  Addr: Device address on BUS Bus.
  * @param  Reg: The target register address to write
  * @param  MemAddress: Memory address
  * @param  Buffer: The target register value to be written
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_WriteMultiple(I2C_HandleTypeDef* i2c_handler,
                                            uint8_t Addr,
                                            uint16_t Reg,
                                            uint16_t MemAddress,
                                            uint8_t* Buffer,
                                            uint16_t Length)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

    /* Check the communication status */
    if (status != HAL_OK)
    {
        /* Re-Initiaize the I2C Bus */
        I2Cx_Error(i2c_handler, Addr);
    }
    return status;
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  * @retval None
  */

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
