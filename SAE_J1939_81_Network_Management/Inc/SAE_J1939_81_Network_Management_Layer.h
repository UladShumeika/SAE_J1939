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

#endif /* __SAE_J1939_21_NETWORK_MANAGEMENT_LAYER_H */
