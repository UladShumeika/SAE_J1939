/**
  ******************************************************************************
  * @file    SAE_J1939_81_Network_Management_Layer.h
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    05 May 2023
  * @brief   Header file of SAE J1939-21 Network Management Layer.
  *
  ******************************************************************************
  */

//---------------------------------------------------------------------------
// Define to prevent recursive inclusion
//---------------------------------------------------------------------------
#ifndef __SAE_J1939_21_NETWORK_MANAGEMENT_LAYER_H
#define __SAE_J1939_21_NETWORK_MANAGEMENT_LAYER_H

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "stm32f4xx.h"

//---------------------------------------------------------------------------
// Structures and enumerations
//---------------------------------------------------------------------------

/**
 * @brief SAE J1939 the current ECU information.
 */
typedef struct
{
	uint8_t ECUaddress;
} J1939_informationECU;

//---------------------------------------------------------------------------
// External function prototypes
//---------------------------------------------------------------------------

/**
 * @brief 	This function is used to get the current ECU address.
 * @retval	ECU address.
 */
uint8_t J1939_getCurrentECUAddress(void);

/**
 * @brief 	This function is used to set the current ECU address.
 * @param	address - The current ECU address.
 * @retval	None.
 */
void J1939_setCurrentECUAddress(uint8_t address);

#endif /* __SAE_J1939_21_NETWORK_MANAGEMENT_LAYER_H */
