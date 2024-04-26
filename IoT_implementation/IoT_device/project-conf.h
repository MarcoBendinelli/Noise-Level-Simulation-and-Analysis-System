/*---------------------------------------------------------------------------*/
/**
 * \file
 * Project specific configuration defines for the IoT-device
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
/* Enable TCP */
#define UIP_CONF_TCP 1
/*---------------------------------------------------------------------------*/
/* User configuration */
/*---------------------------------------------------------------------------*/
#define MQTT_CLIENT_STATUS_LED  LEDS_GREEN
#define MQTT_CLIENT_TRIGGER_LED LEDS_RED
#define MQTT_CLIENT_READ_LED LEDS_BLUE
#define MQTT_CLIENT_PUBLISH_TRIGGER &button_left_sensor

#define MQTT_CLIENT_PUBLISH_TOPIC   "iot/italy/noiseLevels/readings"
#define MQTT_CLIENT_SUB_TOPIC       "iot/italy/noiseLevels/settingThreshold"

#define MQTT_CLIENT_BROKER_IP_ADDR "fd00::1"
//*---------------------------------------------------------------------------*/
#define WINDOW_SIZE                6
#define K                          30
#define FILE_NAME                 "NoiseLevels"
//*---------------------------------------------------------------------------*/
#define IEEE802154_CONF_DEFAULT_CHANNEL      21
//*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
/** @} */
