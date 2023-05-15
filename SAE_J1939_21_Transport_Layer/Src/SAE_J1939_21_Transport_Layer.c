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
#define J1939_CONNECTION_MANAGEMENT				(0xECU)
#define J1939_DATA_TRANSFER						(0xEBU)

#define J1939_BROADCAST_ADDRESS					(255U)
#define J1939_MAX_LENGTH_TP_MODE_PACKAGE		(7U)

#define J1939_PGN_PRIOTITY_POS					(26U)
#define J1939_PDU_FORMAT_POS					(16U)
#define J1939_PDU_SPECIFIC_POS					(8U)

#define J1939_EDP_0								(0U)
#define J1939_EDP_1								(1 << 25U)
#define J1939_DP_0								(0U)
#define J1939_DP_1								(1 << 24U)

//---------------------------------------------------------------------------
// Structure definitions
//---------------------------------------------------------------------------
static J1939_TP_CM connectManagement = {0};
static J1939_TP_DT dataTransfer = {0};

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
	if(connectManagement.control_byte == J1939_CONTROL_BYTE_TP_CM_BAM)
	{
		// Memory allocation for the message (used from FreeRTOS)
		dataTransfer.data = (uint8_t*)pvPortMalloc(connectManagement.message_size * sizeof(uint8_t));
		dataTransfer.data_size = connectManagement.message_size;

		// Check memory allocation
		if(dataTransfer.data == NULL) status = J1939_STATUS_ERROR;
	}

	return status;
}

/**
 * @brief	This function is used to send transport protocol connection management messages.
 * @param	destinationAddress - A destination address(255 for broadcast).
 * @retval	None.
 */
void J1939_sendTP_connectionManagement(uint8_t destinationAddress)
{
	USH_CAN_txHeaderTypeDef txMessage = {0};
	uint8_t currentECUAddress = J1939_getCurrentECUAddress();
	uint8_t data[8] = {0};

	txMessage.ExtId = (((uint32_t)7U << J1939_PGN_PRIOTITY_POS) | \
					   (J1939_CONNECTION_MANAGEMENT << J1939_PDU_FORMAT_POS) | \
					   (destinationAddress << J1939_PDU_SPECIFIC_POS) | \
					   currentECUAddress);
	txMessage.IDE 	= CAN_ID_EXT;
	txMessage.RTR 	= CAN_RTR_DATA;
	txMessage.DLC 	= 8U;

	if(destinationAddress == J1939_BROADCAST_ADDRESS)
	{
		data[0] = connectManagement.control_byte;
		data[1] = (uint8_t)connectManagement.message_size;
		data[2] = (uint8_t)(connectManagement.message_size >> 8U);
		data[3] = connectManagement.total_number_of_packages;
		data[4] = 0xFFU;
		data[5] = (uint8_t)connectManagement.PGN_of_the_multipacket_message;
		data[6] = (uint8_t)(connectManagement.PGN_of_the_multipacket_message >> 8U);
		data[7] = (uint8_t)(connectManagement.PGN_of_the_multipacket_message >> 16U);
	} else
	{
		// peer-to-peer connection
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
 * @brief	This function used to fill connection management structure.
 * @param 	data - A pointer to the sending data.
 * @param 	dataSize - A size of the sending data.
 * @param 	PGN - A PGN of the multipacket message.
 * @param 	controlByte - A type of the control byte.
 * @retval	None.
 */
void J1939_fillConnectionManagement(uint8_t* data, uint16_t dataSize, uint32_t PGN, J1939_controlBytes controlByte)
{
	uint8_t remainder = dataSize % J1939_MAX_LENGTH_TP_MODE_PACKAGE;

	connectManagement.control_byte 						= controlByte;
	connectManagement.message_size 						= dataSize;
	connectManagement.total_number_of_packages 			= (remainder > 0U) ? ((dataSize / J1939_MAX_LENGTH_TP_MODE_PACKAGE) + 1) : \
																			 (dataSize / J1939_MAX_LENGTH_TP_MODE_PACKAGE);
	connectManagement.PGN_of_the_multipacket_message 	= PGN;
}
