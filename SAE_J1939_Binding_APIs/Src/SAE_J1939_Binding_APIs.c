/**
  ******************************************************************************
  * @file    SAE_J1939_Binding_APIs.c
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    03 May 2023
  * @brief	 This file contains the implementation of functions for binding APIs
  * 		 of SAE J1939
  *
  ******************************************************************************
  */

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "SAE_J1939_Binding_APIs.h"

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define J1939_PRIORITY_MASK					(0x03U)

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static J1939_states J1939_state = J1939_STATE_UNINIT;

//---------------------------------------------------------------------------
// Library Functions
//---------------------------------------------------------------------------

/**
 * @brief 	This function is used to processing J1939 messages.
 * @param 	rxMessage - A pointer to the receiving message's data.
 * @retval	None.
 */
void J1939_messagesProcessing(USH_CAN_rxHeaderTypeDef* rxMessage)
{
	uint8_t pages = (uint8_t)(rxMessage->ExtId >> 24U) & J1939_PRIORITY_MASK;
	uint8_t pduFormat = (uint8_t)(rxMessage->ExtId >> 16U);
	uint8_t destinAddress = (uint8_t)(rxMessage->ExtId >> 8U);
	uint8_t sourceAddress = (uint8_t)rxMessage->ExtId;

	uint8_t data[] = "Hello, my name is Ulad!";
}

/**
 * @brief	This function is used to switch between SAE J1939 protocol operation
 * 			states
 * @retval 	None.
 */
void J1939_stateMachine(void)
{
	switch(J1939_state)
	{
		case J1939_STATE_UNINIT:
			J1939_initCAN();
			J1939_state = J1939_STATE_NORMAL;
			break;

		case J1939_STATE_NORMAL:
			break;

		case J1939_STATE_TP_RECEIVING_BROADCAST:
			break;

		case J1939_STATE_TP_SENDING_BROADCAST:
			break;

		default:
			break;
	}
}

/**
 * @brief 	This function is used to get J1939 state.
 * @retval	J1939 state.
 */
J1939_states J1939_getState(void)
{
	return J1939_state;
}

/**
 * @brief 	This function is used to set J1939 state.
 * @param	J1939 state.
 * @retval	None.
 */
void J1939_setState(J1939_states state)
{
	J1939_state = state;
}

/**
 * @brief	This function is used to configuration CAN bus.
 * @retval	None.
 */
__WEAK void J1939_initCAN(void)
{

}
