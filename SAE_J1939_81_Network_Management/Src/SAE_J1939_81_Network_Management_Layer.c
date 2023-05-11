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
 * @param	data - A pointer to the receiving data.
 * @retval	J1939 status.
 */
uint8_t J1939_getCurrentECUAddress(void)
{
	return currentECU.ECUaddress;
}
