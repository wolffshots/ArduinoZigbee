/*
  Zigbee HA On Off Output/Light example

  The circuit:
  - Arduino Nano 33 BLE

*/

#include <ArduinoZigbee.h>

void toggleCallBack1(const bool state) {
  if(state == 0){
    digitalWrite(LEDR, HIGH);
    Serial.println("turned R on");
  }else{
    digitalWrite(LEDR, LOW);
    Serial.println("turned R off");
  }
}

void toggleCallBack2(const bool state) {
  if(state == 0){
    digitalWrite(LEDG, HIGH);
    Serial.println("turned G on");
  }else{
    digitalWrite(LEDG, LOW);
    Serial.println("turned G off");
  }
}

void toggleCallBack3(const bool state) {
  if(state == 0){
    digitalWrite(LEDB, HIGH);
    Serial.println("turned B on");
  }else{
    digitalWrite(LEDB, LOW);
    Serial.println("turned B off");
  }
}

ZigbeeOnOffOutput light1("Red", toggleCallBack1);
ZigbeeOnOffOutput light2("Green", toggleCallBack2);
ZigbeeOnOffOutput light3("Blue", toggleCallBack3);

void setup() {
  Serial.begin(9600);
  while ((!Serial) && (millis() < 10000))
    ; // wait until serial is set up or 10s passes (whichever comes first) - this is only really for debugging, in production you'd not want to delay boot like this
  ZIGBEE.addEndpoint(light1);// add red light
  ZIGBEE.addEndpoint(light2);// add green light
  ZIGBEE.addEndpoint(light3);// add blue light
  Serial.println("added endpoints");
  // If you want to explicitly set the Trust Centre Link Key enable this section
  // uint8_t zb_zll_tc_key[] = {};  // insert TC_KEY as HEX if known (0xFF, 0xFF, ...)
  // ZIGBEE.setTrustCenterKey(zb_zll_tc_key);
  ZIGBEE.setZigbeeRevisionLevelCompatibility(20); // a revision level <21 means that TC link key exchange is disabled which means the device is not compatible with Zigbee R21+
  Serial.println("set trust centre link stuff");
  int start_result = ZIGBEE.begin();// start the zigbee communication on all channels by default, pass in a list of channels to only run on certain channels
  Serial.print("zigbee start result was ");
  Serial.println(start_result);
  Serial.println("setup complete");
}

void loop() {
  ZIGBEE.poll();// this is how the device monitors the endpoints for updates and communicates with the network
}
