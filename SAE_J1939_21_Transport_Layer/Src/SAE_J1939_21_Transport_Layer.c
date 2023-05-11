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
// Includes
//---------------------------------------------------------------------------
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
 * @brief 	This function is used to read transport protocol data transfer messages.
 * @param	data - A pointer to the receiving data.
 * @retval	None.
 */
void J1939_readTP_dataTransfer(uint8_t* data)
{
	uint8_t index = data[0] - 1;

	dataTransfer.sequence_number = data[0];

	for(uint8_t i = 1; i <= 7; i++)
	{
		dataTransfer.data[(index * 7) + (i - 1)] = data[i];
	}

	//dataTransfer.flag_wait_message = false;

	if(connectManagement.total_number_of_packages == dataTransfer.sequence_number)
	{
		//J1939_setState(J1939_STATE_NORMAL);
	}
}
