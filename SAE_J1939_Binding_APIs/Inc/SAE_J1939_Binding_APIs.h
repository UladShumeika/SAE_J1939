/**
  ******************************************************************************
  * @file    SAE_J1939_Binding_APIs.h
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    03 May 2023
  * @brief   Header file of Binding APIs of SAE J1939.
  *
  ******************************************************************************
  */

//---------------------------------------------------------------------------
// Define to prevent recursive inclusion
//---------------------------------------------------------------------------
#ifndef __SAE_J1939_BINDING_APIS_H
#define __SAE_J1939_BINDING_APIS_H

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "stm32f4xx.h"
#include "SAE_J1939_21_Transport_Layer.h"

//---------------------------------------------------------------------------
// Structures and enumerations
//---------------------------------------------------------------------------

/**
 * @brief SAE J1939 protocol states enumeration.
 */
typedef enum
{
	J1939_STATE_UNINIT,
	J1939_STATE_NORMAL,
	J1939_STATE_TP_RECEIVING_BROADCAST,
	J1939_STATE_TP_SENDING_BROADCAST,
	J1939_STATE_TP_RECEIVING_PEER_TO_PEER,
	J1939_STATE_TP_SENDING_PEER_TO_PEER
} J1939_states;

//---------------------------------------------------------------------------
// External function prototypes
//---------------------------------------------------------------------------

/**
 * @brief 	This function is used to processing J1939 messages.
 * @param 	rxMessage - A pointer to the receiving message's data.
 * @param	data - A pointer to the receiving data.
 * @retval	None.
 */
void J1939_messagesProcessing(USH_CAN_rxHeaderTypeDef* rxMessage, uint8_t* data);

/**
 * @brief	This function is used to switch between SAE J1939 protocol operation
 * 			states
 * @retval 	None.
 */
void J1939_stateMachine(void);

/**
 * @brief 	This function is used to get J1939 state.
 * @retval	J1939 state.
 */
J1939_states J1939_getState(void);

/**
 * @brief 	This function is used to set J1939 state.
 * @param	J1939 state.
 * @retval	None.
 */
void J1939_setState(J1939_states state);

/**
 * @brief	This function is used to configuration CAN bus.
 * @retval	None.
 */
__WEAK void J1939_initCAN(void);

#endif /* __SAE_J1939_BINDING_APIS_H */
