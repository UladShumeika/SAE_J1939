/**
  ******************************************************************************
  * @file    SAE_J1939_21_Transport_Layer.h
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    05 May 2023
  * @brief   Header file of SAE J1939-21 Transport Layer.
  *
  ******************************************************************************
  */

//---------------------------------------------------------------------------
// Define to prevent recursive inclusion
//---------------------------------------------------------------------------
#ifndef __SAE_J1939_21_TRANSPORT_LAYER_H
#define __SAE_J1939_21_TRANSPORT_LAYER_H

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "stm32f4xx.h"

//---------------------------------------------------------------------------
// Structures and enumerations
//---------------------------------------------------------------------------

/**
 * @brief J1939 control bytes enumeration.
 */
typedef enum
{
	CB_TP_CM_RTS 				= 16U,
	CB_TP_CM_CTS 				= 17U,
	CB_TP_CM_END_OF_MSG_ACK 	= 19U,
	CB_TP_CONN_ABORT 			= 255U
} J1939_controlBytes;

#endif /* __SAE_J1939_21_TRANSPORT_LAYER_H */
