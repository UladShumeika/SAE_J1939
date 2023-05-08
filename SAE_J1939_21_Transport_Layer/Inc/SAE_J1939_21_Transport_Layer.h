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
 * @brief PGN 0x00EC00 - Transport Protocol Connection Management.
 */
typedef struct
{
	uint8_t control_byte;							/* Type of messages	*/
	uint16_t message_size;							/* Total bytes of sending message - 9 to 1785 */
	uint8_t total_number_of_packages;				/* Number of packages to send a message */
	uint32_t PGN_of_the_multipacket_message;		/* A PGN that activated the multi-packet transfer */
} J1939_TP_CM;

/**
 * @brief PGN 0x00EB00 - Transport Protocol Data Transfer.
 */
typedef struct
{
	uint8_t sequence_number;		/* Sequence number - 1 to 255 */
	uint8_t* data;					/* A pointer to data */
	uint16_t data_size;				/* From 9 to MAX_DT_SIZE */
	uint8_t flag_wait_message;		/* A flag of waiting message */
} J1939_TP_DT;

typedef enum
{
	CB_TP_CM_RTS 				= 16U,
	CB_TP_CM_CTS 				= 17U,
	CB_TP_CM_END_OF_MSG_ACK 	= 19U,
	CB_TP_CONN_ABORT 			= 255U
} J1939_controlBytes;

#endif /* __SAE_J1939_21_TRANSPORT_LAYER_H */
