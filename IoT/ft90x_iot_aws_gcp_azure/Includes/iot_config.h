#ifndef _IOT_CONFIG_H_
#define _IOT_CONFIG_H_

#include <ft900.h>
#include <iot/iot_config_brokers.h>



///////////////////////////////////////////////////////////////////////////////////
// Choose your MQTT IoT Cloud Broker
//   MQTT_BROKER_UNKNOWN
//   MQTT_BROKER_AWS_IOT
//   MQTT_BROKER_GCP_IOT
//   MQTT_BROKER_MAZ_IOT
//   MQTT_BROKER_AWS_GREENGRASS
//   MQTT_BROKER_GCP_EDGE        - not yet supported
//   MQTT_BROKER_MAZ_EDGE        - not yet supported
//   MQTT_BROKER_ADAFRUITIO
//   MQTT_BROKER_UBIDOTS
//   MQTT_BROKER_THINGSPEAK
//   MQTT_BROKER_LOCAL           - for RabbitMQ/Mosquitto local broker
//   Refer to iot_config_brokers.h
///////////////////////////////////////////////////////////////////////////////////

#define USE_MQTT_BROKER               MQTT_BROKER_LOCAL

///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// Enable/disable for optimization purposes
///////////////////////////////////////////////////////////////////////////////////

#if (USE_MQTT_BROKER == MQTT_BROKER_AWS_IOT) || (USE_MQTT_BROKER == MQTT_BROKER_AWS_GREENGRASS)
    #define USE_ROOT_CA               1
    #define USE_MBEDTLS_MAX_SIZES     2 // if using ATS endpoint
    //#define USE_MBEDTLS_MAX_SIZES   0 // if using non-ATS endpoint
    #define USE_DEVICE_CERT           1
#elif (USE_MQTT_BROKER == MQTT_BROKER_GCP_IOT)
    // Google IoT does not need a root CA
    #define USE_ROOT_CA               0 // memory footprint optimization
    #define USE_MBEDTLS_MAX_SIZES     0 // memory footprint optimization
    #define USE_DEVICE_CERT           1
#elif (USE_MQTT_BROKER == MQTT_BROKER_MAZ_IOT)
    // Microsoft IoT requires a root CA
    #define USE_ROOT_CA               1
    #define USE_DEVICE_CERT           1
#elif (USE_MQTT_BROKER == MQTT_BROKER_ADAFRUITIO)
    #define USE_ROOT_CA               0
    #define USE_MBEDTLS_MAX_SIZES     2
    #define USE_DEVICE_CERT           0
#elif (USE_MQTT_BROKER == MQTT_BROKER_UBIDOTS)
    #define USE_ROOT_CA               0
    #define USE_MBEDTLS_MAX_SIZES     2
    #define USE_DEVICE_CERT           0
#elif (USE_MQTT_BROKER == MQTT_BROKER_THINGSPEAK)
    #define USE_ROOT_CA               0
    #define USE_MBEDTLS_MAX_SIZES     3
    #define USE_DEVICE_CERT           0
#else
    #define USE_ROOT_CA               1
    #define USE_MBEDTLS_MAX_SIZES     0
    #define USE_DEVICE_CERT           1
#endif

///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// Select the device to use
///////////////////////////////////////////////////////////////////////////////////

// This demo application provides 3 sets of device certificates
// Each one will work for Amazon AWS, Google Cloud and Microsoft Azure
#define SAMPLE_DEVICE_1               1 // corresponds to ft900device1_cert.pem
#define SAMPLE_DEVICE_2               2 // corresponds to ft900device2_cert.pem
#define SAMPLE_DEVICE_3               3 // corresponds to ft900device3_cert.pem
#define USE_MQTT_DEVICE               SAMPLE_DEVICE_1

///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
//
// MQTT CREDENTIALS
//   Default:
//     MQTT_BROKER
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME
//     MQTT_CLIENT_USER
//     MQTT_CLIENT_PASS
//   Amazon AWS IoT
//     MQTT_BROKER = �IDENTIFIER.iot.REGION.amazonaws.com�
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = DEVICE_ID or THING_NAME
//     MQTT_CLIENT_USER = NULL // not needed
//     MQTT_CLIENT_PASS = NULL // not needed
//   Amazon AWS Greengrass
//     MQTT_BROKER = IP address or host name of local Greengrass device
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = DEVICE_ID or THING_NAME
//     MQTT_CLIENT_USER = NULL // not needed
//     MQTT_CLIENT_PASS = NULL // not needed
//   Google Cloud IoT
//     MQTT_BROKER = �mqtt.googleapis.com�
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = �projects/PROJECT_ID/locations/LOCATION_ID/registries/REGISTRY_ID/devices/DEVICE_ID�
//     MQTT_CLIENT_USER = � � // any
//     MQTT_CLIENT_PASS = JWT security token (generated with private key)
//   Microsoft Azure IoT (SAS security token authentication)
//     MQTT_BROKER = �HUB_NAME.azure-devices.net�
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = DEVICE_ID
//     MQTT_CLIENT_USER = �HUB_NAME.azure-devices.net/DEVICE_ID/api-version=2016-11-14�
//     MQTT_CLIENT_PASS = SAS security token (generated with shared access key)
//   Microsoft Azure IoT (TLS certificate authentication)
//     MQTT_BROKER = �HUB_NAME.azure-devices.net�
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = DEVICE_ID
//     MQTT_CLIENT_USER = �HUB_NAME.azure-devices.net/DEVICE_ID/api-version=2016-11-14�
//     MQTT_CLIENT_PASS = NULL // not needed
//   AdafruitIO
//     MQTT_BROKER = "io.adafruit.com"
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = DEVICE_ID // any
//     MQTT_CLIENT_USER = Username  // Check AdafruitIO dashboard
//     MQTT_CLIENT_PASS = AIO Key   // Check AdafruitIO dashboard
//   Ubidots
//     MQTT_BROKER = �things.ubidots.com�
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = ANY
//     MQTT_CLIENT_USER = TOKEN    // Check Ubidots dashboard
//     MQTT_CLIENT_PASS = ANY
//   ThingSpeak
//     MQTT_BROKER = "mqtt.thingspeak.com"
//     MQTT_BROKER_PORT = 8883
//     MQTT_CLIENT_NAME = ANY
//     MQTT_CLIENT_USER = ANY
//     MQTT_CLIENT_PASS = MQTT API KEY   // Check ThingSpeak dashboard
//
// TLS CERTIFICATES
// Authentication with Amazon AWS, Google Cloud and Microsoft Azure
//   Sample for Amazon AWS IoT (using TLS certificate authentication)
//     Rootca.pem - self-signed ca certificate
//     Rootca_aws_ats.pem - server certificate if connecting to ATS endpoint
//     Rootca_aws_ver.pem - server certificate if not connecting to ATS endpoint
//     Ft900device1_cert.pem
//     Ft900device1_pkey.pem
//   Sample for Amazon AWS Greengrass (using TLS certificate authentication)
//     Rootca_gg.pem
//     Ft900device1_cert.pem
//     Ft900device1_pkey.pem
//   Sample for Google Cloud IoT (using TLS certificate/JWT security token authentication)
//     Ft900device1_cert.pem // not used by device, registered in cloud only
//     Ft900device1_pkey.pem // used to generate the JWT security token
//   Sample for Microsoft Azure IoT (using SAS security token authentication)
//     Rootca_azure.pem
//     Ft900device1_sas_azure.pem // used to generate SAS security token
//   Sample for Microsoft Azure IoT (using TLS certificate authentication)
//     Rootca_azure.pem
//     Ft900device1_cert.pem
//     Ft900device1_pkey.pem
//   Sample for AdafruitIO
//     Not needed
//   Sample for Ubidots
//     Not needed
//   Sample for ThingSpeak
//     Not needed
//
///////////////////////////////////////////////////////////////////////////////////

// Load the configuration from a file for easier modification by users
#if (USE_MQTT_BROKER == MQTT_BROKER_AWS_IOT) || (USE_MQTT_BROKER == MQTT_BROKER_AWS_GREENGRASS)
    #include <iot_config_aws.h>
#elif (USE_MQTT_BROKER == MQTT_BROKER_GCP_IOT)
    #include <iot_config_gcp.h>
#elif (USE_MQTT_BROKER == MQTT_BROKER_MAZ_IOT)
    #include <iot_config_azure.h>
#elif (USE_MQTT_BROKER == MQTT_BROKER_ADAFRUITIO)
    #include <iot_config_adafruitio.h>
#elif (USE_MQTT_BROKER == MQTT_BROKER_UBIDOTS)
    #include <iot_config_ubidots.h>
#elif (USE_MQTT_BROKER == MQTT_BROKER_THINGSPEAK)
    #include <iot_config_thingspeak.h>
#else
    // Set values here for generic MQTT brokers
    #define USE_DEVICE_ID             "deviceid"      // replace me
    #define MQTT_BROKER_PORT          MQTT_TLS_PORT
    #define MQTT_BROKER               "192.168.100.5" // replace me
    #define MQTT_CLIENT_NAME          USE_DEVICE_ID
    #define MQTT_CLIENT_USER          "guest"         // replace me
    #define MQTT_CLIENT_PASS          "guest"         // replace me

    // This certificate refers to rootca.pem
    extern __flash__ uint8_t ca_data[]        asm("rootca_pem");
    extern __flash__ uint8_t ca_data_end[]    asm("rootca_pem_end");
    // This certificate refers to ft900device1_cert.pem
    extern __flash__ uint8_t cert_data[]      asm("ft900device1_cert_pem");
    extern __flash__ uint8_t cert_data_end[]  asm("ft900device1_cert_pem_end");
    // This private key refers to ft900device1_pkey.pem
    extern __flash__ uint8_t pkey_data[]      asm("ft900device1_pkey_pem");
    extern __flash__ uint8_t pkey_data_end[]  asm("ft900device1_pkey_pem_end");
#endif

///////////////////////////////////////////////////////////////////////////////////



#include <iot/iot_config_defaults.h>

#endif /* _IOT_CONFIG_H_ */
