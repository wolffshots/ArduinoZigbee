/*
  Zigbee HA Temperature Sensor example

  This example creates a zigbee device following the HA(Home Automation) profile.
  It uses a simulated temperature sensor

  The circuit:
  - Arduino Nano 33 BLE/Sense

  The line `const zb_uint32_t DEFAULT_TEMPERATURE_MEASUREMENT_PERIOD = 30000;` in zigbee_temperature_sensor_implementation.cpp is what 

  This example code is in the public domain.
*/
#include <ArduinoZigbee.h>

float tempSensorCB() {
  // Put sensor read here
  return float(random(15.0,22.0)); 
}

ZigbeeTemperatureSensor temperature_sensor(tempSensorCB);

void setup() {
  Serial.begin(9600);
  while ((!Serial) && (millis() < 10000))
    ;  // wait until serial is set up or 10s passes (whichever comes first) - this is only really for debugging, in production you'd not want to delay boot like this
  ZIGBEE.addEndpoint(temperature_sensor);
  
  Serial.println("added endpoint");
#if 1
  // If you want to explicitly set the Trust Centre Link Key enable this section
  // uint8_t zb_zll_tc_key[] = {};  // insert TC_KEY as HEX if known (0xFF, 0xFF, ...)
  // ZIGBEE.setTrustCenterKey(zb_zll_tc_key);
  ZIGBEE.setZigbeeRevisionLevelCompatibility(20);  // a revision level <21 means that TC link key exchange is disabled which means the device is not compatible with Zigbee R21+
  Serial.println("set trust centre link stuff");
#endif
  ZIGBEE.setDeviceName("tempsensor");
  int start_result = ZIGBEE.begin();  // start the zigbee communication on all channels by default, pass in a list of channels to only run on certain channels
  if (start_result == 1) {
    Serial.println("began zigbee");
  } else {
    while (true) {  // a loop to listen in on serial if zigbee set up fails
      delay(1000);
      Serial.println("zigbee setup failed");
      Serial.print("result was ");
      Serial.println(start_result);
    }
  }
  Serial.println("setup complete");
} 

void loop() {
  ZIGBEE.poll();
}
