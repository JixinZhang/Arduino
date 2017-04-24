/*
 *
 *
 *
 *
 *
 */

#include <ESP8266WiFi.h>

const char *ssid     = "ziroom001";
const char *password = "ziroomer002";

const char *host = "http://api.yeelink.net";
const int httpPort = 80;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  //start to connecting to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println("WiFi.localIP()");
}

void loop() {
  // put your main code here, to run repeatedly:
  // delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;

  if (!client.connect(host,httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/v1.0/device/345323/sensor/384355/datapoints";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET") + url + "HTTP/1.1\r\n" + 
               "host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout!");
      client.stop();
      return;
    }
  }

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}
