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
#define J1939_MESSAGE_PACKET_FREQ				(50U) // from 50 to 200 ms
#define J1939_MESSAGE_DATA_TIMEOUT				(750U)
#define J1939_MESSAGE_CM_TIMEOUT				(1250U)

#define J1939_BROADCAST_ADDRESS					(255U)
#define J1939_USE_CURRENT_DA					(1U)

#define J1939_CONNECTION_MANAGEMENT				(0xECU)
#define J1939_DATA_TRANSFER						(0xEBU)

//---------------------------------------------------------------------------
// Structures and enumerations
//---------------------------------------------------------------------------

/**
 * @brief SAE J1939 protocol states enumeration.
 */
typedef enum
{
	J1939_STATE_UNINIT,					/* CAN bus and J1939 protocol isn't initialized	*/
	J1939_STATE_NORMAL,					/* For sending CAN message <= 8 bytes */
	J1939_STATE_TP_RX_BROADCAST,		/* Receiving data in transport protocol mode broadcast */
	J1939_STATE_TP_TX_BROADCAST,		/* Sending data in transport protocol mode broadcast */
	J1939_STATE_TP_RX_PTP_CTS,			/* Receiving data in transport protocol mode peer-to-peer.
	 	 	 	 	 	 	 	 	 	   CTS(Clear-to-send) waiting */
	J1939_STATE_TP_RX_PTP_DATA,			/* Receiving data in transport protocol mode peer-to-peer */
	J1939_STATE_TP_RX_PTP_EOM,			/* Receiving data in transport protocol mode peer-to-peer.
	 	 	 	 	 	 	 	 	 	   Sending EOM (End-of-message) */
	J1939_STATE_TP_TX_PTP_CTS,			/* Sending data in transport protocol mode peer-to-peer.
		 	 	 	 	 	 	 	 	   Sending CTS(Clear-to-send) message */
	J1939_STATE_TP_TX_PTP_DATA,			/* Sending data in transport protocol mode peer-to-peer */
	J1939_STATE_TP_TX_PTP_EOM,			/* Sending data in transport protocol mode peer-to-peer.
		 	 	 	 	 	 	 	 	   EOM (End-of-message) waiting */
} J1939_states;

/**
 * @brief J1939 status enumeration.
 */
typedef enum
{
	J1939_NO_STATUS,					/* Used to initialize a status variable */
	J1939_ERROR_BUSY,					/* Status to alert TP session already open */
	J1939_ERROR_MEMORY_ALLOCATION,		/* Status error memory allocation */
	J1939_ERROR_TOO_BIG_MESSAGE,		/* Status too big message */
	J1939_STATUS_GOT_BAM_MESSAGE,		/* Got BAM message */
	J1939_STATUS_GOT_RTS_MESSAGE,		/* Got RTS message */
	J1939_STATUS_GOT_CTS_MESSAGE,		/* Got CTS message */
	J1939_STATUS_GOT_EOM_MESSAGE,		/* Got EOM message */
	J1939_STATUS_GOT_ABORT_SESSION,		/* Got ABORT message */
	J1939_STATUS_DATA_FINISHED,			/* Sending/receiving data finished */
	J1939_STATUS_DATA_CONTINUE,			/* Sending/receiving data continue */
	J1939_STATUS_CTS					/* Used in PTP mode to notify that the number of packets specified
										   in the CTS message is completed and it's necessary to wait for the next */
} J1939_status;

/**
 * @brief J1939 transport protocol connection management message types.
 */
typedef enum
{
	J1939_TP_TYPE_BAM,      		/* Broadcast announce message */
	J1939_TP_TYPE_RTS,      		/* Request to send message */
	J1939_TP_TYPE_CTS,      		/* Clear to send message */
	J1939_TP_TYPE_ABORT,    		/* Abort message type */
	J1939_TP_TYPE_END_OF_MSG		/* End of message type */
} J1939_TPcmTypes;

/**
 * @brief SAE J1939 control bytes enumeration.
 */
typedef enum
{
	J1939_CONTROL_BYTE_TP_CM_RTS			= 16U,		/* Control byte for RTS (Request to Send) */
	J1939_CONTROL_BYTE_TP_CM_CTS			= 17U,		/* Control byte for CTS (Clear to Send) */
	J1939_CONTROL_BYTE_TP_CM_EndOfMsgACK	= 19U,		/* Control byte for EndOfMsgACK (End of Message Acknowledgment) */
	J1939_CONTROL_BYTE_TP_CM_BAM			= 32U,		/* Control byte for BAM (Broadcast Announce Message) */
	J1939_CONTROL_BYTE_TP_CM_Abort			= 255U		/* Control byte for Connection Abort */
} J1939_controlBytes;

/**
 * @brief SAE J1939 abort reasons enumeration.
 */
typedef enum
{
	J1939_REASON_BUSY						= 1,		/* Already in one or more connection managed sessions and
								   	   	   	   	   	   	   cannot support another */
	J1939_REASON_TIMEOUT 					= 3, 		/* A timeout occurred, and this is the connection abort
										   	   	   	   	   to close the session. */
	J1939_REASON_TOO_BIG_MESSAGE			= 9,		/* “Total Message Size” is greater than 1785 bytes */
	J1939_REASON_MEMORY_ALLOCATION_ERROR	= 250U,		/* Memory allocation error for receiving message */
} J1939_abortReasons;

/**
 * @brief PGN 0x00EC00 - Transport Protocol Connection Management.
 */
typedef struct
{
	J1939_controlBytes control_byte;				/* Type of messages	*/
	uint16_t message_size;							/* Total bytes of sending message - 9 to MAX_DT_SIZE */
	uint8_t total_number_of_packages;				/* Number of packages to send a message */
	uint32_t PGN_of_the_multipacket_message;		/* A PGN that activated the multi-packet transfer */

	uint8_t total_number_of_packages_in_CTS;		/* Max. number of packages in response to CTS. 0xFF - No limit. */
	uint8_t remaining_packages_from_CTS;			/* It remains to accept packets from the last CTS. */
	uint8_t next_package;							/* A next package. */
	uint8_t CTS_available_message;					/* 1 allows to process CTS messages, 0 - doesn't */

	uint8_t destination_address;					/* ECU address to send data to. 255 - broadcast */

	J1939_abortReasons abort_reason;				/* Connection abort reason */
	uint8_t destination_address_abort;				/* The address of the ECU to which the ABORT message must be sent */
} J1939_TP_CM;

/**
 * @brief PGN 0x00EB00 - Transport Protocol Data Transfer.
 */
typedef struct
{
	uint8_t sequence_number;		/* Sequence number - 1 to 255 */
	uint8_t* data;					/* A pointer to data */
	uint16_t data_size;				/* From 9 to MAX_DT_SIZE */
	uint16_t sent_bytes;			/* A flag of waiting message */
	uint16_t received_bytes;
	uint8_t memory_allocated;		/* 1 - memory allocated, 0 - no memory allocated */
} J1939_TP_DT;

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
 * @brief	This function is used to send transport protocol connection management messages.
 * @param	type - A type of the transport protocol connection management message.
 * @retval	None.
 */
void J1939_sendTP_connectionManagement(J1939_TPcmTypes type);

/**
 * @brief 	This function is used to read transport protocol data transfer messages.
 * @param	data - A pointer to the receiving data.
 * @retval	J1939 status.
 */
J1939_status J1939_readTP_dataTransfer(uint8_t* data);

/**
 * @brief	This function is used to send the data transfer packages.
 * @param 	destinationAddress - A destination address(255 for broadcast).
 * @return	J1939 status.
 */
J1939_status J1939_sendTP_dataTransfer(uint8_t destinationAddress);

/**
 * @brief	This function is used to abort the current multi-packet session.
 * @param	reason - A reason of aborting session.
 * @param   destinationAddress - A destination address(255 for broadcast).
 * @retval	None.
 */
void J1939_sendTP_connectionAbort(J1939_abortReasons reason, uint8_t destinationAddress);

/**
 * @brief	This function used to fill TP structures.
 * @param 	data - A pointer to the sending data.
 * @param 	dataSize - A size of the sending data.
 * @param 	PGN - A PGN of the multipacket message.
 * @param 	controlByte - A type of the control byte.
 * @retval	None.
 */
void J1939_fillTPstructures(uint8_t* data, uint16_t dataSize, uint32_t PGN, J1939_controlBytes controlByte);

/**
 * @brief	This function is used to clean TP structures.
 * @retval	None.
 */
void J1939_clearTPstructures(void);

/**
 * @brief 	This function is used to free allocated memory.
 * @retval	None.
 */
void J1939_freeAllocatedMemory(void);

/**
 * @brief 	This function is used to set the abort reason.
 * @param 	abortReason - The abort reason.
 * @param	abortAddress - The address of the ECU to which the ABORT message must be sent.
 * @retval	None.
 */
void J1939_setAbortReason(J1939_abortReasons abortReason, uint8_t abortAddress);

/**
 * @brief 	This function is used to get the pointer to received data.
 * @retval	A pointer to received data.
 */
uint8_t* J1939_getReceivedMessage(void);

#endif /* __SAE_J1939_21_TRANSPORT_LAYER_H */
