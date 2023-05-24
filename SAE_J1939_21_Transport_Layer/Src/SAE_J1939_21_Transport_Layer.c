/**
  ******************************************************************************
  * @file    SAE_J1939_21_Transport_Layer.c
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    05 May 2023
  * @brief	 This file contains the implementation of functions for a transport
  * 		 layer of SAE J1939
  *
  ******************************************************************************
  */

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "SAE_J1939_21_Transport_Layer.h"
#include "SAE_J1939_81_Network_Management_Layer.h"

// for NULL and pvPortMalloc function
#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include <stddef.h>
#include "portable.h"

//---------------------------------------------------------------------------
// Configuration section
//---------------------------------------------------------------------------
#if defined(STM32F429xx)
	#define CAN_USED							(CAN1)
#elif defined(STM32F407xx)
	#define CAN_USED							(CAN2)
#endif

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define J1939_MAX_LENGTH_TP_MODE_PACKAGE		(7U)
#define J1939_MAX_NUMBER_PACKAGES_IN_CTS		(4U)
#define J1939_STANDART_NUMBER_PACKAGES_IN_CTS	(2U)

#define J1939_PGN_PRIOTITY_POS					(26U)
#define J1939_PDU_FORMAT_POS					(16U)
#define J1939_PDU_SPECIFIC_POS					(8U)

#define J1939_EDP_0								(0U)
#define J1939_EDP_1								(1 << 25U)
#define J1939_DP_0								(0U)
#define J1939_DP_1								(1 << 24U)

#define J1939_MAX_LENGTH_MESSAGE				(1785U)

//---------------------------------------------------------------------------
// Structure definitions
//---------------------------------------------------------------------------
static J1939_TP_CM connectManagement 	= {0};
static J1939_TP_DT dataTransfer 		= {0};

//---------------------------------------------------------------------------
// Library Functions
//---------------------------------------------------------------------------

/**
 * @brief 	This function is used to read transport protocol connection management messages.
 * @param	data - A pointer to the receiving data.
 * @retval	J1939 status.
 */
J1939_status J1939_readTP_connectionManagement(uint8_t* data)
{
	J1939_status status = J1939_STATUS_OK;

	// Read the multi-packet message's parameters
	connectManagement.control_byte 						= data[0];
	connectManagement.message_size 						= ((uint16_t)data[2] << 8U) | data[1];
	connectManagement.total_number_of_packages 			= data[3];
	connectManagement.PGN_of_the_multipacket_message 	= (((uint32_t)data[7] << 16U) | \
														   ((uint32_t)data[6] << 8U) | data[5]);
	// Check the control byte
	switch(connectManagement.control_byte)
	{
		case J1939_CONTROL_BYTE_TP_CM_BAM:

			// Memory allocation for the message (used from FreeRTOS)
			dataTransfer.data = (uint8_t*)pvPortMalloc(connectManagement.message_size * sizeof(uint8_t));
			dataTransfer.data_size = connectManagement.message_size;

			// Check memory allocation
			if(dataTransfer.data == NULL) status = J1939_STATUS_ERROR;
			break;

		case J1939_CONTROL_BYTE_TP_CM_Abort:
			status = J1939_STATUS_DATA_ABORT;
			break;

		default:
			break;
	}

	return status;
}

/**
 * @brief	This function is used to send transport protocol connection management messages.
 * @param	type - A type of the transport protocol connection management message.
 * @retval	None.
 */
void J1939_sendTP_connectionManagement(J1939_TPcmTypes type)
{
	USH_CAN_txHeaderTypeDef txMessage = {0};
	uint8_t currentECUAddress = J1939_getCurrentECUAddress();
	uint8_t data[8] = {0};

	// Fill in CAN ID
	if(type == J1939_TP_TYPE_ABORT)
	{
		txMessage.ExtId = (((uint32_t)7U << J1939_PGN_PRIOTITY_POS) | \
							(J1939_CONNECTION_MANAGEMENT << J1939_PDU_FORMAT_POS) | \
							(connectManagement.destination_address_abort << J1939_PDU_SPECIFIC_POS) | currentECUAddress);
	} else
	{
		txMessage.ExtId = (((uint32_t)7U << J1939_PGN_PRIOTITY_POS) | \
							(J1939_CONNECTION_MANAGEMENT << J1939_PDU_FORMAT_POS) | \
							(connectManagement.destination_address << J1939_PDU_SPECIFIC_POS) | currentECUAddress);
	}

	txMessage.IDE 	= CAN_ID_EXT;
	txMessage.RTR 	= CAN_RTR_DATA;
	txMessage.DLC 	= 8U;

	// Fill in bytes that are the same for all messages
	data[5] = (uint8_t)connectManagement.PGN_of_the_multipacket_message;
	data[6] = (uint8_t)(connectManagement.PGN_of_the_multipacket_message >> 8U);
	data[7] = (uint8_t)(connectManagement.PGN_of_the_multipacket_message >> 16U);

	// Fill in the rest of the bytes according to the message type
	switch(type)
	{
		case J1939_TP_TYPE_BAM:
			data[0] = J1939_CONTROL_BYTE_TP_CM_BAM;
			data[1] = (uint8_t)connectManagement.message_size;
			data[2] = (uint8_t)(connectManagement.message_size >> 8U);
			data[3] = connectManagement.total_number_of_packages;
			data[4] = 0xFFU;
			break;

		case J1939_TP_TYPE_RTS:
			data[0] = J1939_CONTROL_BYTE_TP_CM_RTS;
			data[1] = (uint8_t)connectManagement.message_size;
			data[2] = (uint8_t)(connectManagement.message_size >> 8U);
			data[3] = connectManagement.total_number_of_packages;
			data[4] = connectManagement.total_number_of_packages_in_CTS;

			connectManagement.CTS_available_message = 1U;
			break;

		case J1939_TP_TYPE_END_OF_MSG:
			data[0] = J1939_CONTROL_BYTE_TP_CM_EndOfMsgACK;
			data[1] = (uint8_t)connectManagement.message_size;
			data[2] = (uint8_t)(connectManagement.message_size >> 8U);
			data[3] = connectManagement.total_number_of_packages;
			data[4] = 0xFFU;
			break;

		case J1939_TP_TYPE_CTS:
			data[0] = J1939_CONTROL_BYTE_TP_CM_CTS;
			data[1] = ((connectManagement.total_number_of_packages - connectManagement.next_package) >= J1939_STANDART_NUMBER_PACKAGES_IN_CTS) ? \
					  (J1939_STANDART_NUMBER_PACKAGES_IN_CTS) : ((connectManagement.total_number_of_packages - connectManagement.next_package) + 1U);
			data[2] = connectManagement.next_package;
			data[3] = 0xFFU;
			data[4] = 0xFFU;
			break;

		case J1939_TP_TYPE_ABORT:
			data[0] = J1939_CONTROL_BYTE_TP_CM_Abort;
			data[1] = (uint8_t)connectManagement.abort_reason;
			data[2] = 0xFFU;
			data[3] = 0xFFU;
			data[4] = 0xFFU;

			connectManagement.destination_address_abort 	= 0U;
			connectManagement.abort_reason 					= 0U;
			break;

		default:
			break;
	}

	CAN_addTxMessage(CAN_USED, &txMessage, data);
}

/**
 * @brief 	This function is used to read transport protocol data transfer messages.
 * @param	data - A pointer to the receiving data.
 * @retval	J1939 status.
 */
J1939_status J1939_readTP_dataTransfer(uint8_t* data)
{
	J1939_status status = J1939_STATUS_DATA_CONTINUE;
	uint8_t index = data[0] - 1;

	// Read the multi-packet message
	dataTransfer.sequence_number = data[0];

	for(uint8_t i = 1; i <= 7; i++)
	{
		dataTransfer.data[(index * 7) + (i - 1)] = data[i];
	}

	// Check the last package
	if(connectManagement.total_number_of_packages == dataTransfer.sequence_number)
	{
		status = J1939_STATUS_DATA_FINISHED;
	}

	return status;
}

/**
 * @brief	This function is used to send the data transfer packages.
 * @return	J1939 status.
 */
J1939_status J1939_sendTP_dataTransfer(void)
{
	J1939_status status = J1939_STATUS_DATA_CONTINUE;
	USH_CAN_txHeaderTypeDef txMessage = {0};
	uint8_t currentECUAddress = J1939_getCurrentECUAddress();
	uint8_t data[8] = {0};

	// Build CAN ID FRAME, where 7 is the default priority
	txMessage.ExtId		= (((uint32_t)7U << J1939_PGN_PRIOTITY_POS) | J1939_EDP_0 | J1939_DP_0 | \
		      	  	  	   (J1939_DATA_TRANSFER << J1939_PDU_FORMAT_POS) | \
						   (connectManagement.destination_address << J1939_PDU_SPECIFIC_POS) | currentECUAddress);
	txMessage.IDE		= CAN_ID_EXT;
	txMessage.RTR		= CAN_RTR_DATA;
	txMessage.DLC		= 8U;

	// Fill in the data field of the sent message, taking into account the type of transfer
	if(connectManagement.destination_address == J1939_BROADCAST_ADDRESS)
	{
		data[0] = ++dataTransfer.sequence_number;

		for(uint8_t i = 1U; i <= J1939_MAX_LENGTH_TP_MODE_PACKAGE; i++)
		{
			(dataTransfer.sent_bytes < dataTransfer.data_size) ? (data[i] = dataTransfer.data[dataTransfer.sent_bytes++]) : \
																		    (data[i] = 0xFFU);
		}
	} else
	{
		dataTransfer.sequence_number 	= connectManagement.next_package;
		dataTransfer.sent_bytes 		= (connectManagement.next_package - 1U) * J1939_MAX_LENGTH_TP_MODE_PACKAGE;

		data[0] = connectManagement.next_package++;

		for(uint8_t i = 1U; i <= J1939_MAX_LENGTH_TP_MODE_PACKAGE; i++)
		{
			(dataTransfer.sent_bytes < connectManagement.message_size) ? (data[i] = dataTransfer.data[dataTransfer.sent_bytes++]) : \
																		 (data[i] = 0xFFU);
		}

		if((--connectManagement.remaining_packages_from_CTS) == 0U)
		{
			status = J1939_STATUS_CTS;
			connectManagement.CTS_available_message = 1U;
		}
	}

	// Send the data package
	CAN_addTxMessage(CAN_USED, &txMessage, data);

	// Check if the message has been sent
	if(dataTransfer.sent_bytes >= dataTransfer.data_size) status = J1939_STATUS_DATA_FINISHED;

	return status;
}

/**
 * @brief	This function used to fill TP structures.
 * @param 	data - A pointer to the sending data.
 * @param 	dataSize - A size of the sending data.
 * @param 	PGN - A PGN of the multipacket message.
 * @param 	destinationAddress - ECU address to send data to.
 * @retval	None.
 */
void J1939_fillTPstructures(uint8_t* data, uint16_t dataSize, uint32_t PGN, uint8_t destinationAddress)
{
	uint8_t remainder = dataSize % J1939_MAX_LENGTH_TP_MODE_PACKAGE;

	// Fill the connection management structure
	if(destinationAddress != J1939_BROADCAST_ADDRESS)
	{
		connectManagement.total_number_of_packages_in_CTS  	= J1939_MAX_NUMBER_PACKAGES_IN_CTS;
		connectManagement.next_package						= 1U;
	}

	connectManagement.message_size 						= dataSize;
	connectManagement.total_number_of_packages			= (remainder > 0U) ? ((dataSize / J1939_MAX_LENGTH_TP_MODE_PACKAGE) + 1) : \
																			  (dataSize / J1939_MAX_LENGTH_TP_MODE_PACKAGE);
	connectManagement.PGN_of_the_multipacket_message	= PGN;
	connectManagement.destination_address				= destinationAddress;

	// Fill the data transfer structure
	dataTransfer.data 									= data;
	dataTransfer.data_size 								= dataSize;
}

/**
 * @brief	This function is used to clean TP structures.
 * @retval	None.
 */
void J1939_clearTPstructures(void)
{
	// Clean the connection management structure
	connectManagement.control_byte						= 0U;
	connectManagement.message_size						= 0U;
	connectManagement.total_number_of_packages			= 0U;
	connectManagement.total_number_of_packages_in_CTS	= 0U;
	connectManagement.next_package						= 0U;
	connectManagement.PGN_of_the_multipacket_message	= 0U;
	connectManagement.destination_address				= 0U;
	connectManagement.PGN_of_the_multipacket_message	= 0U;

	// Clean the data transfer structure
	dataTransfer.sequence_number						= 0U;
	dataTransfer.data									= 0U;
	dataTransfer.data_size								= 0U;
	dataTransfer.sent_bytes								= 0U;
	dataTransfer.memory_allocated						= 0U;
}

/**
 * @brief 	This function is used to free allocated memory.
 * @retval	None.
 */
void J1939_freeAllocatedMemory(void)
{
	vPortFree(dataTransfer.data);
}

/**
 * @brief 	This function is used to set the abort reason.
 * @param 	abortReason - The abort reason.
 * @param	abortAddress - The address of the ECU to which the ABORT message must be sent.
 * @retval	None.
 */
void J1939_setAbortReason(J1939_abortReasons abortReason, uint8_t abortAddress)
{
	connectManagement.abort_reason = abortReason;

	if(abortAddress == J1939_USE_CURRENT_DA)
	{
		connectManagement.destination_address_abort = connectManagement.destination_address;
	} else
	{
		connectManagement.destination_address_abort = abortAddress;
	}
}

/**
 * @brief 	This function is used to get the pointer to received data.
 * @retval	A pointer to received data.
 */
uint8_t* J1939_getReceivedMessage(void)
{
	return dataTransfer.data;
}
