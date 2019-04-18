/*
 * @briefd FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOSConfig.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
xQueueHandle xQueue;
void vSenderTask(void *pvParameters)
{
	int8_t SendData = '0';
	portBASE_TYPE DataStatus = '0';

		SendData = (int8_t) pvParameters;
		for(int i=0; i<3; i++)
		{
		DataStatus = xQueueSendToBack(xQueue, &SendData, 0);

		if(DataStatus != pdPASS)
			{
				printf("unable to send to queue");
			}
		}
}

static void vReceiverTask(void *pvParameters)
{
	int8_t ReceiveData = '0';
		portTickType timeToWait = 100 / portTICK_RATE_MS;
		portBASE_TYPE DataStatus = '0';
		while(1)
		{
			DataStatus = xQueueReceive(xQueue, &ReceiveData, timeToWait);
			if(DataStatus == pdPASS)
			{
			Bool LedState = false;
				if(ReceiveData == 0)
				{
					Board_LED_Set(0, LedState);
					LedState = (bool) !LedState;

					/* About a 3Hz on/off toggle rate */
					vTaskDelay(configTICK_RATE_HZ / 1);
				}
				else if(ReceiveData == 1)
				{
					Board_LED_Set(1, LedState);
					LedState = (bool) !LedState;

					/* About a 3Hz on/off toggle rate */
					vTaskDelay(configTICK_RATE_HZ / 2);
				}
				else if(ReceiveData == 2)
				{
					Board_LED_Set(2, LedState);
					LedState = (bool) !LedState;

					/* About a 3Hz on/off toggle rate */
					vTaskDelay(configTICK_RATE_HZ / 3);
				}
			}
			else if(DataStatus == errQUEUE_EMPTY){}
		}

}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{

	xQueue = xQueueCreate(7, sizeof(int8_t));

	if(xQueue != NULL)
	{

	/* LED1 toggle thread */
	xTaskCreate(vSenderTask, (signed char *) "Sender", 1000, (void *) 100, 1, NULL);
	xTaskCreate(vReceiverTask, (signed char *) "Receiver", 1000, NULL, 2, NULL);

	/* Start the scheduler */
	vTaskStartScheduler();
	}

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
