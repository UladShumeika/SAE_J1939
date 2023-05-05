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
// Static variables
//---------------------------------------------------------------------------
static J1939_states J1939_state = J1939_STATE_UNINIT;

//---------------------------------------------------------------------------
// Library Functions
//---------------------------------------------------------------------------

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
