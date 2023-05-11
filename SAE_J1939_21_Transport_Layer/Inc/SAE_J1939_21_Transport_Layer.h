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
// Defines
//---------------------------------------------------------------------------
#define MAX_DT_SIZE					(1785U)

//---------------------------------------------------------------------------
// Structures and enumerations
//---------------------------------------------------------------------------

/**
 * @brief PGN 0x00EC00 - Transport Protocol Connection Management.
 */
typedef struct
{
	uint8_t control_byte;							/* Type of messages	*/
	uint16_t message_size;							/* Total bytes of sending message - 9 to MAX_DT_SIZE */
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

/**
 * @brief SAE J1939 control bytes enumeration.
 */
typedef enum
{
	J1939_CONTROL_BYTE_TP_CM_RTS			= 16U,
	J1939_CONTROL_BYTE_TP_CM_CTS			= 17U,
	J1939_CONTROL_BYTE_TP_CM_EndOfMsgACK	= 19U,
	J1939_CONTROL_BYTE_TP_CM_BAM			= 32U,
	J1939_CONTROL_BYTE_TP_CM_Abort			= 255U
} J1939_controlBytes;

/**
 * @brief SAE J1939 abort reasons enumeration.
 */
typedef enum
{
	J1939_REASON_BUSY		= 1,		/* Already in one or more connection managed sessions and
								   	   	   cannot support another */

	J1939_REASON_TIMEOUT 	= 3, 		/* A timeout occurred, and this is the connection abort
										   to close the session. */
} J1939_abortReasons;

/**
 * @brief J1939 status enumeration.
 */
typedef enum
{
	J1939_STATUS_OK				= 0,
	J1939_STATUS_ERROR,
	J1939_STATUS_DATA_FINISHED,
	J1939_STATUS_DATA_CONTINUE
} J1939_status;

//---------------------------------------------------------------------------
// External function prototypes
//---------------------------------------------------------------------------

/**
 * @brief 	This function is used to read transport protocol connection management messages.
 * @param	data - A pointer to the receiving data.
 * @retval	J1939 status.
 */
J1939_status J1939_readTP_connectionManagement(uint8_t* data);

/**
 * @brief 	This function is used to read transport protocol data transfer messages.
 * @param	data - A pointer to the receiving data.
 * @retval	None.
 */
void J1939_sendTP_connectionAbort(J1939_abortReasons reason);

#endif /* __SAE_J1939_21_TRANSPORT_LAYER_H */
