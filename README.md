# FT900 IoT/Smart Home

This contains the <b>IoT framework</b>, <b>Amazon Alexa demo</b> and <b>AWS/GCP/Azure IoT end-to-end demo</b> applications for FTDI/Bridgetek's FT900 series of memory-constrained microcontrollers.


# FT900 Alexa Demo
- This demonstrates using the <b>FT900 microcontroller [or ESP32 microcontroller] as an Amazon Echo Dot device</b>, where users can interact with Alexa via an edge gateway hub.
- This integrates Alexa AVS SDK on smart hub device running on Raspberry PI platform and adds <b>Alexa Virtualization feature</b> enabling multiple in-house FT900 MCUs [or ESP32 MCUs] to access Alexa using different Alexa instances and Amazon accounts.
- This will be integrated to <b>PanL Smart Home Automation</b> (PanL Hub and PanL Display).
- [RPI Alexa Gateway](https://github.com/richmondu/FT900/tree/master/Alexa/Amazon%20Alexa%20Gateway), [FT900 Alexa Client](https://github.com/richmondu/FT900/tree/master/Alexa/Amazon%20Alexa%20Client), [FT900 Alexa Client Simulator](https://github.com/richmondu/FT900/tree/master/Alexa/Amazon%20Alexa%20Client%20Simulator), [ESP32 Alexa Client](https://github.com/richmondu/FT900/tree/master/Alexa/Amazon%20Alexa%20Client%20ESP32)

<img src="https://github.com/richmondu/FT900/blob/master/Alexa/Amazon%20Alexa%20Client/docs/images/system_diagram.jpg" width="623"/>

<img src="https://github.com/richmondu/FT900/blob/master/Alexa/Amazon%20Alexa%20Client/docs/images/system_diagram2.jpg" width="623"/>



# FT900 AWS IoT Demo
- This demonstrates secure MQTT connectivity to Amazon AWS IoT Core and AWS Greengrass using MQTT library from Amazon FreeRTOS aka AWS IoT SDK.
- It contains end-to-end demo including lambda functions for backend cloud and local gateway (Greengrass). 
- This was made after I successfully integrated mbedTLS open-source library with optimal settings for memory footprint.
- This was presented in Electronica 2018 https://brtchip.com/Brochures/FT900%20AWS%20IoT%20Demosheet.pdf
  https://github.com/richmondu/FT900/tree/master/IoT/aws_demos_ft_greengrass_lwip_mbedtls

<img src="https://github.com/richmondu/FT900/blob/master/IoT/aws_demos_ft_greengrass_lwip_mbedtls/doc/FT900%20AWS%20IoT%20-%20System%20Architecture.jpeg" width="623"/>



# FT900 AWS/GCP/Azure IoT Demo [MQTT]
- This demonstrates <b>secure MQTT connectivity</b> to popular IoT cloud services:
  Amazon AWS IoT Core [using X.509 certificate authentication] 
  Google Cloud IoT Core [using JWT authentication]
  Microsoft Azure IoT Hub [using SAS authentication and X.509 certificate authentication] 
- These were presented in Embedded World 2019 https://www.ftdichip.com/Embedded%20World%202019.htm
  https://github.com/richmondu/FT900/tree/master/IoT/ft90x_iot_aws_gcp_azure
- These utilize the IoT library I created which includes AWS/GCP/Azure IoT cloud connectivity, mbedTLS SSL library integration, X.509 certificate handling/authentication, JWT/SAS access token generation, ciphersuite security-memory footprint tradeoffs, LWIP open-source bug fix contributions, and IoT Project Templates (for FT900 Eclipse IDE).



# FT900 Amazon SNS/Lambda/IoT Client Demo [HTTPS]
- These demonstrate <b>secure HTTPS connectivity</b> to Amazon services and SigV4 Signature generation for HTTP POST request.
- This consists of the following demo applications
  FT900 Amazon SNS Client - sending of text/SMS or email messages
  https://github.com/richmondu/FT900/tree/master/IoT/ft90x_amazon_sns_httpclient
  FT900 Amazon Lambda Client - invoking serverless cloud function
  https://github.com/richmondu/FT900/tree/master/IoT/ft90x_amazon_lambda_httpclient
  FT900 Amazon IoT Client - publishing sensor data
  https://github.com/richmondu/FT900/tree/master/IoT/ft90x_amazon_iot_httpclient
