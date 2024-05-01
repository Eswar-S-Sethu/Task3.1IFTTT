#include <WiFiNINA.h>
#include <Keyboard.h>

char ssid[] = "WiFi-13EA2A";
char pass[] = "43038930";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/intensity_change/with/key/eEDyViFanvA7P_kMsUnxsRSI6w5O4nw1iROgjUAK4Pw";
String queryString = "?value1="; // Value1 will be replaced with light intensity value from the sensor

const int light_sensor = 7;

void setup() {
  WiFi.begin(ssid, pass);
  Keyboard.begin();
  Serial.begin(9600);
  while (!Serial);

  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100);
  Serial.print("Light level: ");
  Serial.println(light);

  if (light > 30) { // Adjust this threshold as needed
    sendRequest(light);
    delay(2000); // Delay to prevent multiple requests within a short time
  }
}

void sendRequest(int lightIntensity) {
  client.println("GET " + PATH_NAME + queryString + lightIntensity + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println();
  
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  
  client.stop();
  Serial.println();
  Serial.println("Disconnected");
}
