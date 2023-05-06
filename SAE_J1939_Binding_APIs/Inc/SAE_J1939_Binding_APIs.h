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
	J1939_STATE_TP_RECEIVE,
	J1939_STATE_TP_TRANSMITE
} J1939_states;

//---------------------------------------------------------------------------
// External function prototypes
//---------------------------------------------------------------------------

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
