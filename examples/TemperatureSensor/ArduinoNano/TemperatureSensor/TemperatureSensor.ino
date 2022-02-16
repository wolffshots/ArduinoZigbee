/*
  Zigbee HA Temperature Sensor example

  This example creates a zigbee device following the HA(Home Automation) profile.
  It uses the onboard temperature sensor of an Arduino Nano 33 BLE Sense to read 
  room temperature

  The circuit:
  - Arduino Nano 33 BLE Sense

  This example code is in the public domain.
*/

#include <Arduino_HTS221.h>
#include <ArduinoZigbee.h>

float tempSensorCB_1() {
  // Put sensor read here
  return HTS.readTemperature(); 
}

ZigbeeTemperatureSensor temperature_sensor(tempSensorCB_1);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started!");

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  Serial.println("Sensors initialized!");

  ZIGBEE.addEndpoint(temperature_sensor);

  ZIGBEE.begin(11);
}

void loop() {
  ZIGBEE.poll();
}