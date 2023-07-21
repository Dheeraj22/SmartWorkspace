/******************************************************************************
 * File Name:   publisher_task.c
 *
 * Description: This file contains the task that initializes the user button
 *              GPIO, configures the ISR, and publishes MQTT messages on the
 *              topic 'MQTT_TOPIC' to control a device that is actuated by the
 *              subscriber task. The file also contains the ISR that notifies
 *              the publisher task about the new device state to be published.
 *
 * Related Document: See README.md
 *
 *******************************************************************************
 * (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress's integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 *******************************************************************************/

#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"

/* Task header files */
#include "publisher_task.h"
#include "mqtt_task.h"
#include "subscriber_task.h"

/* Configuration file for MQTT client */
#include "mqtt_client_config.h"

/* Middleware libraries */
#include "cy_retarget_io.h"
#include "iot_mqtt.h"

#include "../cJSON/cJSON.h"


/******************************************************************************
 * Macros
 ******************************************************************************/
/* Interrupt priority for User Button Input. */
#define USER_BTN_INTR_PRIORITY          (5)

/* The maximum number of times each PUBLISH in this example will be retried. */
#define PUBLISH_RETRY_LIMIT             (10)

/* A PUBLISH message is retried if no response is received within this 
 * time (in milliseconds).
 */
#define PUBLISH_RETRY_MS                (1000)

/******************************************************************************
 * Global Variables
 *******************************************************************************/
/* FreeRTOS task handle for this task. */
TaskHandle_t publisher_task_handle;

#define JSON_START
char *data = NULL;
static int print_preallocated(cJSON *root);

/* Structure to store publish message information. */
IotMqttPublishInfo_t publishCubicleInfo =
{
		.qos = (IotMqttQos_t) MQTT_MESSAGES_QOS,
		.pTopicName = MQTT_CUBICLE,
		.topicNameLength = (sizeof(MQTT_CUBICLE) - 1),
		.retryMs = PUBLISH_RETRY_MS,
		.retryLimit = PUBLISH_RETRY_LIMIT
};

/* Structure to store publish message information. */
IotMqttPublishInfo_t publishEquipmentInfo =
{
		.qos = (IotMqttQos_t) MQTT_MESSAGES_QOS,
		.pTopicName = MQTT_EQUIPMENT,
		.topicNameLength = (sizeof(MQTT_EQUIPMENT) - 1),
		.retryMs = PUBLISH_RETRY_MS,
		.retryLimit = PUBLISH_RETRY_LIMIT
};

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
void isr_button_press(void *callback_arg, cyhal_gpio_event_t event);

/******************************************************************************
 * Function Name: publisher_task
 ******************************************************************************
 * Summary:
 *  Task that handles initialization of the user button GPIO, configuration of
 *  ISR, and publishing of MQTT messages to control the device that is actuated
 *  by the subscriber task.
 *
 * Parameters:
 *  void *pvParameters : Task parameter defined during task creation (unused)
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void publisher_task(void *pvParameters)
{
	/* Status variable */
	int result;

	/* Variable to receive new device state from the user button ISR. */
	uint32_t publish_device_state;

	/* Status of MQTT publish operation that will be communicated to MQTT
	 * client task using a message queue in case of failure during publish.
	 */
	mqtt_result_t mqtt_publish_status = MQTT_PUBLISH_FAILURE;

	/* To avoid compiler warnings */
	(void)pvParameters;

	/* Initialize the user button GPIO and register interrupt on falling edge. */
	/* TODO: Change this to timer interrupt */
	cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT,
			CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
	cyhal_gpio_register_callback(CYBSP_USER_BTN, isr_button_press, NULL);
	cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL,
			USER_BTN_INTR_PRIORITY, true);

	printf("Press the user button (SW2) to publish on the topic '%s'...\n\n",
			publishCubicleInfo.pTopicName);

	while (true)
	{
		/* Wait for notification from the User Button ISR. */
		xTaskNotifyWait(0, 0, &publish_device_state, portMAX_DELAY);

		cJSON *root = NULL;

		cubicle_data_t cubicle_data;

		cubicle_data.temp = 0;
		cubicle_data.humidity = 1;
		cubicle_data.co2 = 2;

		//Create the object and send the data needed.
		root = cJSON_CreateObject();
		cJSON_AddNumberToObject(root, "temperature", cubicle_data.temp);
		cJSON_AddNumberToObject(root, "humidity", cubicle_data.humidity);
		cJSON_AddNumberToObject(root, "co2", cubicle_data.co2);

		//print_preallocated() used to convert to string before sending
		if (print_preallocated(root) != 0)
		{
			cJSON_Delete(root);
			printf("Failed to convert json to string. Can't send\n");
		}

		/* Assign the publish message payload according to received device state. */
		publishCubicleInfo.pPayload = data;

		publishCubicleInfo.payloadLength = strlen(data);

		printf("\r\n\nPublishing '%s' on the topic '%s'\n\n",
				(char *)publishCubicleInfo.pPayload,
				publishCubicleInfo.pTopicName);

		/* Publish the MQTT message with the configured settings. */
		result = IotMqtt_PublishSync(mqttConnection,
				&publishCubicleInfo,
				0,
				MQTT_TIMEOUT_MS);

		cJSON_Delete(root);

		if (result != IOT_MQTT_SUCCESS)
		{
			/* Inform the MQTT client task about the publish failure and suspend
			 * the task for it to be killed by the MQTT client task later.
			 */
			printf("MQTT Publish failed with error '%s'.\n\n",
					IotMqtt_strerror((IotMqttError_t) result));
			xQueueOverwrite(mqtt_status_q, &mqtt_publish_status);
			vTaskSuspend( NULL );
		}
	}
}

	/******************************************************************************
	 * Function Name: isr_button_press
	 ******************************************************************************
	 * Summary:
	 *  GPIO interrupt service routine. This function detects button
	 *  presses and sends task notifications to the publisher task about the new
	 *  device state that needs to be published.
	 *
	 * Parameters:
	 *  void *callback_arg : pointer to variable passed to the ISR (unused)
	 *  cyhal_gpio_event_t event : GPIO event type (unused)
	 *
	 * Return:
	 *  void
	 *
	 ******************************************************************************/
	void isr_button_press(void *callback_arg, cyhal_gpio_event_t event)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t new_device_state;

		/* To avoid compiler warnings */
		(void) callback_arg;
		(void) event;

		/* Toggle the device state. */
		if (current_device_state == DEVICE_ON_STATE)
		{
			new_device_state = DEVICE_OFF_STATE;
		}
		else
		{
			new_device_state = DEVICE_ON_STATE;
		}

		/* Notify the publisher task about the new state to be published. */
		xTaskNotifyFromISR(publisher_task_handle, new_device_state, eSetValueWithoutOverwrite,
				&xHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	/******************************************************************************
	 * Function Name: publisher_cleanup
	 ******************************************************************************
	 * Summary:
	 *  Cleanup function for the publisher task that disables the user button
	 *  interrupt. This operation needs to be necessarily performed before deleting
	 *  the publisher task.
	 *
	 * Parameters:
	 *  void
	 *
	 * Return:
	 *  void
	 *
	 ******************************************************************************/
	void publisher_cleanup(void)
	{
		/* Deregister the ISR and disable the interrupt on the user button. */
		cyhal_gpio_register_callback(CYBSP_USER_BTN, NULL, NULL);
		cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL,
				USER_BTN_INTR_PRIORITY, false);
	}


	static int print_preallocated(cJSON *root)
	{
		/* declarations */
		char *out = NULL;
		char *buf = NULL;
		char *buf_fail = NULL;
		size_t len = 0;
		size_t len_fail = 0;

		/* formatted print */
		out = cJSON_Print(root);

		/* create buffer to succeed */
		/* the extra 64 bytes are because of inaccuracies when reserving memory */
		len = strlen(out) + 64;
		printf("Length: %d", len);
		buf = (char*)malloc(len);
		if (buf == NULL)
		{
			printf("Failed to allocate memory.\n");
			exit(1);
		}

		/* create buffer to fail */
		len_fail = strlen(out);
		buf_fail = (char*)malloc(len_fail);
		if (buf_fail == NULL)
		{
			printf("Failed to allocate memory.\n");
			exit(1);
		}

		/* Print to buffer */
		if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
			printf("cJSON_PrintPreallocated failed!\n");
			if (strcmp(out, buf) != 0) {
				printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
				printf("%s", out);
				printf("%s", buf);
			}
			free(out);
			free(buf_fail);
			free(buf);
			return -1;
		}

		/* success */
		//printf("%s\n", buf);
		data = buf;
		strcat(data, ";");
		printf("%s", data);
		/* force it to fail */
		if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
			printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
			free(out);
			free(buf_fail);
			free(buf);
			return -1;
		}

		free(out);
		free(buf_fail);
		free(buf);
		return 0;
	}



	/* [] END OF FILE */
