#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "staff";
const char* password = "pass123$";
const char* api = "https://www.google.com";

DHT dht(13, DHT11);
Adafruit_BMP085 bmp;

WiFiClient client;

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin();

  // if (!dht.begin()) {
  //   Serial.println("Could not find a valid DHT sensor, check wiring!");
  //   while (1) {}
  // }
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
    while (1) {}
  }

  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp1 = dht.readTemperature();
  float hum = dht.readHumidity();

  float temp2 = bmp.readTemperature();
  float press = bmp.readPressure();
  float alt = bmp.readAltitude();

  int aqi = map(analogRead(33), 0, 4095, 0, 1023);
  int cmo = map(analogRead(32), 0, 4095, 0, 1023);

  Serial.print("Temperature(DHT11) - ");
  Serial.print(temp1);
  Serial.println();
  Serial.print("Humidity - ");
  Serial.print(hum);
  Serial.println();
  Serial.print("Temperature(BMP180) - ");
  Serial.print(temp2);
  Serial.println();
  Serial.print("Pressure - ");
  Serial.print(press);
  Serial.println();
  Serial.print("Altitude - ");
  Serial.print(alt);
  Serial.println();
  Serial.print("AQI - ");
  Serial.print(aqi);
  Serial.println();
  Serial.print("Carbon monoxides - ");
  Serial.print(cmo);
  Serial.println();
  Serial.println();


  // if (WiFi.status() == WL_CONNECTED) {
  // HTTPClient http;
  // http.begin(api);
  // http.addHeader("Content-Type", "application/json");
  // String requestBody = "{\"location\":\"Rudraram\",\"aqi\":" + String(aqi) + ",\"co\":" + String(cmo) + ",\"temp\":" + String((temp1 + temp2) / 2) + ",\"ppm\": 000,\"pressure\":" + String(press) + ",\"humidity\":" + String(hum) + "}";
  // Serial.print(requestBody);
  // int httpResponseCode = http.POST(requestBody);
  // if (httpResponseCode > 0) {
  //   Serial.print("HTTP Response code: ");
  //   Serial.println(httpResponseCode);
  //   String response = http.getString();
  //   Serial.println(response);
  // } else {
  //   Serial.print("Error code: ");
  //   Serial.println(httpResponseCode);
  // }
  // http.end();

  // }

  Serial.println("Connect? " + String(client.connect(api, 80)));
  Serial.println("Connected? " + String(client.connected()));
  if (client.connect(api, 80)) {
    String url = "/feedData";
    url += "?location=";
    url += "Rudraram";
    url += "&aqi=";
    url += aqi;
    url += "&co=";
    url += cmo;
    url += "&temp=";
    url += (temp1 + temp2) / 2;
    url += "&ppm=";
    url += 000;
    url += "&pressure=";
    url += press;
    url += "&humidity=";
    url += hum;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + api + "\r\n" + "Connection: close\r\n\r\n");
  }

  delay(5000);
}
