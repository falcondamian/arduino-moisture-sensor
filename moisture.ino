#include <dht.h>
#include <WiFiNINA.h>
#define dht_pin A1
dht DHT;

const char ssid[] = "MIWIFI_bxYH";
const char pass[] = "YybfKhQ4";
int status = WL_IDLE_STATUS;

char server[] = "sensor-api-gateway.herokuapp.com";

WiFiClient client;

const int AirValue = 680;
const int WaterValue = 280;

void setup() {
  
  Serial.begin(9600);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
}

void loop() {

  int soilMoisturePercentage = map(analogRead(A0), WaterValue, AirValue, 100, 0); 
  
  DHT.read11(dht_pin);

  String postData = "{\"sensor\":\"tomatoes\",\"airHumidity\":" + String(DHT.humidity) + ",\"airTemperature\":" + String(DHT.temperature) + ",\"soilHumidity\":" + String(soilMoisturePercentage) + "}";
  
  if (client.connect(server, 80)) {
            
    client.println("POST /collect HTTP/1.1");
    client.println("Host: sensor-api-gateway.herokuapp.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
  }

  if (client.connected()) {
    client.stop();
  }
    
  delay(900000);
}
