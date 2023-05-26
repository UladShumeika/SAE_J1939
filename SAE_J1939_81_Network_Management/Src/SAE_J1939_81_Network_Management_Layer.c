/**
  ******************************************************************************
  * @file    SAE_J1939_81_Network_Management_Layer.h
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    05 May 2023
  * @brief	 This file contains the implementation of functions for a network
  * 		 management layer of SAE J1939
  *
  ******************************************************************************
  */

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "SAE_J1939_81_Network_Management_Layer.h"

//---------------------------------------------------------------------------
// Structure definitions
//---------------------------------------------------------------------------
static J1939_informationECU currentECU = {0};

//---------------------------------------------------------------------------
// Library Functions
//---------------------------------------------------------------------------

/**
 * @brief 	This function is used to get the current ECU address.
 * @retval	ECU address.
 */
uint8_t J1939_getCurrentECUAddress(void)
{
	return currentECU.ECUaddress;
}

/**
 * @brief 	This function is used to set the current ECU address.
 * @param	address - The current ECU address.
 * @retval	None.
 */
void J1939_setCurrentECUAddress(uint8_t address)
{
	currentECU.ECUaddress = address;
}
