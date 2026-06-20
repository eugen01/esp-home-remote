
#include "ESP8266WiFi.h"

/**
 * @brief Configure the WiFi in STA mode and 
 */
void wifi_setup(void)
{

  WiFi.mode(WIFI_STA);

  WiFi.disconnect();
  delay(100);
  WiFi.setOutputPower(10.5);

  // Correct Arduino-compatible way to set global country channels
  wifi_country_t country;
  country.cc[0] = 'D';    // 
  country.cc[1] = 'E';
  country.cc[2] = '\0';
  country.schan = 1;   // Start at channel 1
  country.nchan = 13;  // Scan all the way up to channel 13
  country.policy = WIFI_COUNTRY_POLICY_MANUAL;

  if (wifi_set_country(&country)) {
    Serial.println("Country domain successfully set to Global (Channels 1-13)");
  } else {
    Serial.println("Failed to set country domain.");
  }
  
}

/**
 * @brief Establishes a connection to the WiFi AP
 * This method blocks until the connection has been established
 */
void wifi_connect(void)
{

  WiFi.begin(SECRET_SSID, SECRET_PSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    if (Serial)
    {
      Serial.println("waiting for wifi");
      Serial.println(WiFi.status());
    }
  }

  if (Serial)
  {
    Serial.println("");
    Serial.println("WiFi connection Successful");
    Serial.print("The IP Address of ESP8266 Module is: ");
    Serial.print(WiFi.localIP());// Print the IP address 
  }

}

/**
 * @brief Run a WiFi scan and print the available networks
 * Taken from https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiScan/WiFiScan.ino
 */
void wifi_scan(void)
{
  if (!Serial)
  {
    // The wifi scan only prints available networks.
    // It makes no sense to run this if the Serial is not started
    return;
  }

  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t *bssid;
  int32_t channel;
  bool hidden;
  int scanResult;

  Serial.println(F("Starting WiFi scan..."));

  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

  if (scanResult == 0) 
  {
    Serial.println(F("No networks found"));
  } 
  else if (scanResult > 0) 
  {
    Serial.printf(PSTR("%d networks found:\n"), scanResult);

    // Print unsorted scan results
    for (int8_t i = 0; i < scanResult; i++) 
    {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

      // get extra info
      const bss_info *bssInfo = WiFi.getScanInfoByIndex(i);
      String phyMode;
      const char *wps = "";
      if (bssInfo) 
      {
        phyMode.reserve(12);
        phyMode = F("802.11");
        String slash;
        if (bssInfo->phy_11b) {
          phyMode += 'b';
          slash = '/';
        }
        if (bssInfo->phy_11g) 
        {
          phyMode += slash + 'g';
          slash = '/';
        }
        if (bssInfo->phy_11n) 
        {
          phyMode += slash + 'n';
        }
        if (bssInfo->wps) 
        {
          wps = PSTR("WPS");
        }
      }
      Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %-11s %3S %s\n"), i, channel, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5],
                    rssi, (encryptionType == ENC_TYPE_NONE) ? ' ' : '*', hidden ? 'H' : 'V', phyMode.c_str(), wps, ssid.c_str());
      yield();
    }
  } 
  else 
  {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }

  // Wait a bit before scanning again
  delay(10000);
}
