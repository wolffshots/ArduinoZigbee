/*
  Zigbee HA Dimmable Light example

  This example creates a zigbee device following the HA(Home Automation) profile.
  It uses the onboard RGB led of an Arduino Nano 33 BLE/Sense to simulate two
  dimmable lights.

  The circuit:
  - Arduino Nano 33 BLE/Sense

  This example code is in the public domain.
*/

#include <ArduinoZigbee.h>

void dimLightCallBack1(const uint8_t brightness_level) {
  analogWrite(LEDR, 255 - brightness_level);
  Serial.print("set R to ");
  Serial.println(brightness_level);
}

void dimLightCallBack2(const uint8_t brightness_level) {
  analogWrite(LEDG, 255 - brightness_level);
  Serial.print("set G to ");
  Serial.println(brightness_level);
}

void dimLightCallBack3(const uint8_t brightness_level) {
  analogWrite(LEDB, 255 - brightness_level);
  Serial.print("set B to ");
  Serial.println(brightness_level);
}

ZigbeeDimmableLight light1("Red", dimLightCallBack1);
ZigbeeDimmableLight light2("Green", dimLightCallBack2);
ZigbeeDimmableLight light3("Blue", dimLightCallBack3);

void setup() {
  Serial.begin(9600);
  while ((!Serial) && (millis() < 10000))
    ; // wait until serial is set up or 10s passes (whichever comes first) - this is only really for debugging, in production you'd not want to delay boot like this
  ZIGBEE.addEndpoint(light1);// add red light
  ZIGBEE.addEndpoint(light2);// add green light
  ZIGBEE.addEndpoint(light3);// add blue light
#if 1
  // If you want to explicitly set the Trust Centre Link Key enable this section
  // uint8_t zb_zll_tc_key[] = {};  // insert TC_KEY as HEX if known (0xFF, 0xFF, ...)
  // ZIGBEE.setTrustCenterKey(zb_zll_tc_key);
  ZIGBEE.setZigbeeRevisionLevelCompatibility(20); // a revision level <21 means that TC link key exchange is disabled which means the device is not compatible with Zigbee R21+
  Serial.println("set trust centre link stuff");
#endif
  ZIGBEE.setDeviceName("dimmablelight");
  Serial.println("added endpoints");
  int start_result = ZIGBEE.begin();// start the zigbee communication on all channels by default, pass in a list of channels to only run on certain channels
  light1.setBrightness(255);// not necessary to start up but an example of how to programatically interact with the endpoint
  light2.setBrightness(255);
  light3.setBrightness(255);
  Serial.println("set brightnesses");
  if (start_result == 1) {
    Serial.println("began zigbee");
  } else {
    while (true) {// a loop to listen in on serial if zigbee set up fails
      delay(1000);
      Serial.println("zigbee setup failed");
      Serial.print("result was ");
      Serial.println(start_result);
    }
  }
  Serial.println("setup complete");
}

void loop() {
  ZIGBEE.poll();// this is how the device monitors the endpoints for updates and communicates with the network
}
