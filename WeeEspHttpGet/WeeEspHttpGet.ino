/*  To use SoftwareSerial, mySerial will be used if you create an object (named wifi) of class ESP8266 in your code like this:
    #include "ESP8266.h"
    #include <SoftwareSerial.h>

    SoftwareSerial mySerial(3, 2);  RX:D3, TX:D2
    ESP8266 wifi(mySerial);

    The connection should be like these:
    ESP8266_TX->RX(D3)
    ESP8266_RX->TX(D2)
    ESP8266_CH_PD->3.3V
    ESP8266_VCC->3.3V
    ESP8266_GND->GND
*/

// http://api.yeelink.net/v1.0/device/345323/sensor/384355/datapoint

#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID      "ziroom101"
#define PASSWORD  "ziroomer002"
#define HOST_NAME "api.yeelink.net"
#define HOST_PORT (80)

SoftwareSerial mySerial(3, 2);
ESP8266 wifi(mySerial);

char response[50] = "{";

void setup(void) {
  Serial.begin(9600);
  Serial.print("setup begin \r\n");

  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok \r\n");
  } else {
    Serial.print("to station + softap error \r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");

    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.println("Join AP failure");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single error\r\n");
  }

  Serial.print("setup end\r\n");
    Serial.println(response);
}

void loop(void) {
  uint8_t buffer[1024] = {0};

  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print("create tcp ok \r\n");
  } else {
    Serial.print("create tcp failure\r\n");
  }
  String url = "/v1.0/device/345323/sensor/384355/datapoints";
  String request = String("GET") + url  + "HTTP/1.1\r\n" +
                   "host: " + HOST_NAME + "\r\n" +
                   "Connection: close\r\n\r\n";

  char *hello = "GET /v1.0/device/345323/sensor/384355/datapoints HTTP/1.1\r\nHost:api.yeelink.net\r\nConnection: close\r\n\r\n";
  wifi.send((const uint8_t*)hello, strlen(hello));

  uint32_t len = wifi.recv(buffer, sizeof(buffer), 1000);
  if (len > 0) {
    Serial.print("Received:[");
    for (uint32_t i = 0; i < len; i++) {
      response[i] = (char)buffer[i];
      Serial.print((char)buffer[i]);
    }
    Serial.print("]\r\n");
    Serial.println(response);
  }

  if (wifi.releaseTCP()) {
    Serial.print("release tcp ok\r\n");
  } else {
    Serial.print("release top error\r\n");
  }

  while (1);

}

int getData() {
  int status = 0;
  for (uint32_t index = 0; index < 50; index++) {
    if (response[index] == 'v' &&
        response[index + 1] == 'a' &&
        response[index + 2] == 'l')
    {
      if (response[index + 7] == '1')
      {
        status = 1;
      } else if (response[index + 7] == '0') {
        status = 0;
      } else {
        status = -1;
      }
      break;
    }
  }
  return status;
}
