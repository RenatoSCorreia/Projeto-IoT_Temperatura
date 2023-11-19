#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Renato";
const char *password = "thai6805";

// MQTT Broker
const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("temperature");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temperature = readTemperature();
  
  Serial.print("Timestamp: ");
  Serial.print(millis());
  Serial.print(" - Temperature: ");
  Serial.println(temperature);

  char tempStr[10];
  dtostrf(temperature, 4, 2, tempStr);
  client.publish("temperature", tempStr);

  delay(5000);
}

float readTemperature() {
  int sensorValue = analogRead(A0);
  float temperature = (sensorValue * 0.0048828125 * 100);

  return temperature;
}


