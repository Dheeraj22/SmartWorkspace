/******************************************************************************
* File Name: mqtt_client_config.h
*
* Description: This file contains all the configuration macros used by the 
*              MQTT client in this example.
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

#ifndef MQTT_CLIENT_CONFIG_H_
#define MQTT_CLIENT_CONFIG_H_

#include "iot_mqtt.h"

/*******************************************************************************
* Macros
********************************************************************************/
/* MQTT Broker/Server address and port used for the MQTT connection. */
#define MQTT_BROKER_ADDRESS               "a2wez1ijy9ju6g-ats.iot.us-east-1.amazonaws.com"
#define MQTT_PORT                         8883

/* Set this macro to 'true' if the MQTT Broker being used is hosted by AWS IoT 
 * Core service, else 'false'.
 */
#define AWS_IOT_MQTT_MODE                 ( true )

/* Set this macro to 'true' if a secure connection to the MQTT Broker is  
 * required to be established, else 'false'.
 */
#define MQTT_SECURE_CONNECTION            ( true )

/* The MQTT topic on which the LED control messages will be published.
 */
#define MQTT_CUBICLE                        "smartworkspace/cubicle"
#define MQTT_EQUIPMENT                      "smartworkspace/equipment"
#define MQTT_ERROR                        	"smartworkspace/error"

/* The MQTT topic on which the user prefs will be received.
 */
#define MQTT_USER_PREFS                        "smartworkspace/user"

/* Configuration for the 'Will message' that will be published by the MQTT 
 * broker if the MQTT connection is unexpectedly closed. This configuration is 
 * sent to the MQTT broker during MQTT connect operation and the MQTT broker
 * will publish the Will message on the Will topic when it recognizes an 
 * unexpected disconnection from the client.
 */
#define MQTT_WILL_TOPIC_NAME              MQTT_ERROR "/will"
#define MQTT_WILL_MESSAGE                 ("MQTT client unexpectedly disconnected!")

/* Set the QoS that is associated with the MQTT publish, and subscribe messages.
 * Valid choices are 0, and 1. The MQTT library currently does not support 
 * QoS 2, and hence should not be used in this macro.
 */
#define MQTT_MESSAGES_QOS                 ( 1 )

/* Configure the MQTT username and password that can be used for AWS IoT  
 * Enhanced Custom Authentication.
 */
#define MQTT_USERNAME                     "User"
#define MQTT_PASSWORD                     ""

/* The timeout in milliseconds for MQTT operations in this example. */
#define MQTT_TIMEOUT_MS                   ( 5000 )

/* The keep-alive interval in seconds used for MQTT ping request. */
#define MQTT_KEEP_ALIVE_SECONDS           ( 60 )

/* MQTT client identifier prefix. */
#define MQTT_CLIENT_IDENTIFIER_PREFIX     "psoc6-mqtt-client"

/* The longest client identifier that an MQTT server must accept (as defined
 * by the MQTT 3.1.1 spec) is 23 characters. Add 1 to include the length of the
 * NULL terminator.
 */
#define MQTT_CLIENT_IDENTIFIER_MAX_LEN    ( 24 )

/* MQTT messages which are published and subscribed on the MQTT_TOPIC that
 * controls the device (user LED in this example) state.
 */
#define MQTT_DEVICE_ON_MESSAGE            "TURN ON"
#define MQTT_DEVICE_OFF_MESSAGE           "TURN OFF"

/* As per Internet Assigned Numbers Authority (IANA) the port numbers assigned 
 * for MQTT protocol are 1883 for non-secure connections and 8883 for secure
 * connections. In some cases there is a need to use other ports for MQTT like
 * port 443 (which is reserved for HTTPS). Application Layer Protocol 
 * Negotiation (ALPN) is an extension to TLS that allows many protocols to be 
 * used over a secure connection. The ALPN ProtocolNameList specifies the 
 * protocols that the client would like to use to communicate over TLS.
 * 
 * This macro specifies the ALPN Protocol Name to be used that is supported
 * by the MQTT broker in use.
 * Note: For AWS IoT, currently "x-amzn-mqtt-ca" is the only supported ALPN 
 *       ProtocolName and it is only supported on port 443.
 */
#define MQTT_ALPN_PROTOCOL_NAME           "x-amzn-mqtt-ca"

/* Configure the below credentials in case of a secure MQTT connection. */
/* PEM-encoded client certificate */
#define CLIENT_CERTIFICATE      "\
-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUJSyM97OJ1rV9+o8GFN6D0Kfgz5AwDQYJKoZIhvcNAQEL\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDUwMjA5MTcw\
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALcnq1/QyQhFu8h1Nz4b\
6XFuA/mHO6PbqMGs3Reo/HO/XAUZHgCBFHhB5GHcVD7mEnAA6wWcyKmRqM2kVgt9\
T6KMYrVK6VhALqV13mSmvdtaaqittm92AXuP/CJKBO+9wLuhs2mzUbI/COv50aVp\
UNvlvDqiY90XpjRd4uKFPEVXtuHa2Lbvt0G3MmfvwJRr+phWS0VhMpthjGpux1hu\
8JRSXtYD1WnhQC3dvJIWUDhu792Lyw2IYQggeH5Hu0gkDoCSwMA0E6i0d6EbrOkO\
gWRM2xTHYu/uIsW4vM+ZXeTbLe5nX7ZtlPsBokMqvkmL/thURYEpZI2IljNhkThT\
fkMCAwEAAaNgMF4wHwYDVR0jBBgwFoAU4yFOCiaTEOPd68C7EZW8qz62B8IwHQYD\
VR0OBBYEFC7hAlzlp5GCwgFRq1drZ4GH/e1SMAwGA1UdEwEB/wQCMAAwDgYDVR0P\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBUqeevQk2cGwnu22uMHrdp3K4r\
NSzMneE1CVfzdgv75TejkDlG2Yspni+q3BrDmZDzmVo7JqjMeZqNv/DQDc+b1MvM\
MbaDhbP4052c+LCG1J9w6/pRy6wcri33x7CEuwR75u7wgjyA3BSjoH4R8/asF6b/\
A0Q09jH3v4/IsJyfkJKA0Z+bQ56Ch0WauYMkZyB0uRlClMV89y1zDmmM0b9RXI+u\
2y7ubqSwZ0lqnB0Rz8KmZ659b7rxM25OpTSpNKmIAj+7cJMsiZh8IA9Zo7SjoWB5\
PU7A2IVYhQsOK0zbSyzpYifr1x0QEJ3Bel87UzgJDwEMUSbtpeO7HTlL/7sj\
\n\
-----END CERTIFICATE-----\n"

/* PEM-encoded client private key */
#define CLIENT_PRIVATE_KEY          "\
-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAtyerX9DJCEW7yHU3PhvpcW4D+Yc7o9uowazdF6j8c79cBRke\
AIEUeEHkYdxUPuYScADrBZzIqZGozaRWC31PooxitUrpWEAupXXeZKa921pqqK22\
b3YBe4/8IkoE773Au6GzabNRsj8I6/nRpWlQ2+W8OqJj3RemNF3i4oU8RVe24drY\
tu+3QbcyZ+/AlGv6mFZLRWEym2GMam7HWG7wlFJe1gPVaeFALd28khZQOG7v3YvL\
DYhhCCB4fke7SCQOgJLAwDQTqLR3oRus6Q6BZEzbFMdi7+4ixbi8z5ld5Nst7mdf\
tm2U+wGiQyq+SYv+2FRFgSlkjYiWM2GROFN+QwIDAQABAoIBAAmJEy8F27GGj+Q7\
1ghxw5GYPLS9CanaQcrziP82WQ5dpBjP2fQExGAObil6IxizObqDEblVFE1fu/4Y\
1DH88CxySqFNqSBjFwqePFnZJIFLD8qAYMMkzab1NBdE60TgCCy8mm6iW1qAVaKT\
ewq1lPQd3dLfqXtVQ93uFWXdsemaNMeZrgYDZMU7TobD9cQzX2gZZ4MO4O9cDTte\
ke5ABm6xXTPAxEgXro8JeVdA1WOOHcUe/VLOnPb6YZUoOJTo9gJsuiBIByZ8sjvN\
WaICEf/PkBJX1npWxQgn6J0a+P9VG3WV06FAsl6fF2hK5oRPRqT5jA9kLhsRsUX7\
OvNNH+ECgYEA8fldMGTWI8sxlb8hxFuFfDSEQ/zczxy35DvZ/OyMDEGbOs4vWiae\
TH8k5vcNxLkX2JyVIpyxMOm4H6GMx4UBGICog4JKMW9mUm0Q+BUWNgQs7PRya8Jv\
5ycnT2nbc8OzxZwPhirixaQB18moeHMYuV7r6RrtziJB+CI97RoZOmcCgYEAwcV+\
IsqkBXXqeDCSONV54XfeD/O0aG3IOXttE+N8gC4dyele1TvC5yx3+xDQ+Zx9GzZn\
BL35t/PXDF3yCxYnPl+/R/hsxwZL/YzoYqTfryxjKk6OhsVCQbPOSxuk2NmRcIXd\
6Thin19K99asFBPW/SCO/5e77Oi9cCxpf50r68UCgYEAkzT/SneuBuyQ5+b1Zf2/\
wYY5iRsGQkHYZdM7lLFbqgEnrKjPCetAlo0rKA/YontwVGk+GWZ2LamiSCwB90el\
GQD8g7T56gU5nJ+ZmTNK5eW4Zxd3nLMXXOrm8VsXFOBUN73ve9IPT+ms7VhTU6PB\
wNRMdO3bCnKw64H5ZotgGbUCgYBiGviS6uACd8H/mi/eII81zXebwfqcSMDwAYKn\
aCzXxQGGAebmtPwoCQayEcCMMGPTDEc6SmkNbqJ5e3MZk9zIord429lPJuwAcoAR\
uYIa8bE/cUiOCX9TPpL6ygM7W2RcgRpqgwbDw/5z3rd+7tCRlhNwasVV3DxVd9bu\
KohbNQKBgH4bOvq77JQTJzdeH5dWixbxq9GqBm8ocURbeehjT7DtFcssqTPPFc5z\
Guoyn45Xd3SoOu+DrvryRWtNcQ8c24e1wnCz9fG4zP/9Y5q/ycl8lw6ptAInP12Z\
kT1gBGQ+k7DTStZPFfdBS9HLA3HmY4c0QBkFowCrbukQ74S/jnU2\
\n\
-----END RSA PRIVATE KEY-----\n"

/* PEM-encoded Root CA certificate */
#define ROOT_CA_CERTIFICATE     "\
-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\
rqXRfboQnoZsG4q5WTP468SQvvG5\
\n\
-----END CERTIFICATE-----\n"

/******************************************************************************
* Global Variables
*******************************************************************************/
extern IotMqttNetworkInfo_t networkInfo;
extern IotMqttConnectInfo_t connectionInfo;

#endif /* MQTT_CLIENT_CONFIG_H_ */
