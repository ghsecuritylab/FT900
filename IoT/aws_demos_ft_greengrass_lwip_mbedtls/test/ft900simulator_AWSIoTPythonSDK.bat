:: AWS IoT broker
set AWS_IOT_CORE=amasgua12bmkv.iot.us-east-1.amazonaws.com
set AWS_IOT_CACERT=../certificates/ca.crt

:: AWS Greengrass broker
set GREENGRASS_CORE=192.168.22.16
set GREENGRASS_GROUP_CACERT=../certificates/ggca.crt
set GREENGRASS_CACERT_FILEPATH=../certificates/
set GREENGRASS_CACERT_FILENAME=ggca.crt

:: Device certificates
set GREENGRASS_DEVICE_CERT=../certificates/cert.crt
set GREENGRASS_DEVICE_KEY=../certificates/cert.key
set GREENGRASS_DEVICE_CLIENTID=HelloWorld_Publisher

:: MODES: demo, publish, subscribe, both (publish, subscribe)
:: TOPIC and MESSAGE will not be used when MODE is demo
set MODE=demo
set TOPIC=""
set MESSAGE=""

:: Connect to AWS Greengrass
::python ft900simulator_AWSIoTPythonSDK.py --endpoint %GREENGRASS_CORE% --rootCA %GREENGRASS_GROUP_CACERT% --cert %GREENGRASS_DEVICE_CERT% --key %GREENGRASS_DEVICE_KEY% --thingName %GREENGRASS_DEVICE_CLIENTID% --mode %MODE% --topic %TOPIC% --message %MESSAGE%

:: Connect to AWS IoT
python ft900simulator_AWSIoTPythonSDK.py --endpoint %AWS_IOT_CORE% --rootCA %AWS_IOT_CACERT% --cert %GREENGRASS_DEVICE_CERT% --key %GREENGRASS_DEVICE_KEY% --thingName %GREENGRASS_DEVICE_CLIENTID% --mode %MODE% --topic %TOPIC% --message %MESSAGE%

pause