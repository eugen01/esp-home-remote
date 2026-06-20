

#include "secrets.h"
#include "wifi_utils.h"

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Gree.h>


// IRRemote configuration. Taken from this example: https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/TurnOnGreeAC/TurnOnGreeAC.ino
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRGreeAC ac(kIrLed);  // Set the GPIO to be used for sending messages.


void printState() {
  // Display the settings.
  Serial.println("GREE A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());

  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kGreeStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}




void setup(void)
{
  ac.begin();
  Serial.begin(115200);
  delay(200);

  wifi_setup();

  wifi_connect();


  // Set up what we want to send. See ir_Gree.cpp for all the options.
  // Most things default to off.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
  ac.on();
  ac.setFan(1);
  // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
  ac.setMode(kGreeCool);
  ac.setTemp(20);  // 16-30C
  ac.setSwingVertical(true, kGreeSwingAuto);
  ac.setXFan(false);
  ac.setLight(false);
  ac.setSleep(false);
  ac.setTurbo(false);

}

void loop()
{
  //ac.send();

  Serial.print(WiFi.localIP());

  delay(500);
}
