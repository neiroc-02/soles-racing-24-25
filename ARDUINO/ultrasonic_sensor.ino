#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h> 
#include <ESPping.h>
#include "max6675.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>
#define ONE_WIRE_BUS 7
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ========== TEMPERATURE SENSOR ================================
int numberSensors = 0;
float temperature;

// ========== THERMOCOUPLE SENSOR ===============================
int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// ========== ULTRASONIC SENSOR CONSTANTS =======================
// Note: D_ corresponds to the pin number, A0 is 16, A1 is 17, etc
const int trigPin = 8;
const int echoPin = 9;
long duration;
float distanceCm;
float distanceInch;
// ==============================================================

// =========== MQTT CONSTANTS ===================================
const int BUFFER_SIZE = 10;
float distanceBuffer[BUFFER_SIZE] = {0};  // Initialize all to 0
int bufferIndex = 0;  // Tracks the position to insert the next value

const char WIFI_SSID[] = "SOLES_RACING";                // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "goodlife";        // CHANGE TO YOUR WIFI PASSWORD
const char MQTT_BROKER_ADRRESS[] = "192.168.8.117";  // CHANGE TO MQTT BROKER'S ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "soles";      // CHANGE IT AS YOU DESIRE
const char MQTT_USERNAME[] = "";                          // CHANGE IT IF REQUIRED, empty if not required
const char MQTT_PASSWORD[] = "";                          // CHANGE IT IF REQUIRED, empty if not required
// The MQTT topics that ESP32 should publish/subscribe
const char PUBLISH_TOPIC[] = "soles";    // CHANGE IT AS YOU DESIRE
const char SUBSCRIBE_TOPIC[] = "soles";  // CHANGE IT AS YOU DESIRE
const int PUBLISH_INTERVAL = 1000;  // 1 second
WiFiClient network;
MQTTClient mqtt = MQTTClient(256);
unsigned long lastPublishTime = 0;
// ==============================================================

void setup() {
  // =========== SETUP FOR ULTRASONIC SENSOR ====================
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  // ============================================================
  Serial.println(WiFi.localIP());
  
  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Arduino Nano ESP32 - Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  connectToMQTT();
}

void loop() {
  mqtt.loop();

  if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
    sendToMQTT();
    lastPublishTime = millis();

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED/2;
    distanceInch = distanceCm * CM_TO_INCH;
  
    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    Serial.print("Distance (inch): ");
    Serial.println(distanceInch);
  
    delay(1000);
  }
}

void connectToMQTT() { 
  Serial.println("Pinging MQTT Broker...");
  IPAddress ip (192, 168, 8, 117); // The remote ip to ping
  bool ret = Ping.ping(ip);
  if (ret) {
      Serial.println("MQTT Broker is reachable");
  } else {
      Serial.println("Cannot reach the MQTT Broker");
  }
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  // Create a handler for incoming messages
  mqtt.onMessage(messageHandler);

  Serial.print("Arduino Nano ESP32 - Connecting to MQTT broker");
  unsigned long startTime = millis();
  const unsigned long timeout = 10000;

  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(500);
    if (millis() - startTime > timeout) {
      Serial.println("MQTT Connection Timeout!");
      return;
    }
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("Arduino Nano ESP32 - MQTT broker Timeout!");
    return;
  }

  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("Arduino Nano ESP32 - Subscribed to the topic: ");
  else
    Serial.print("Arduino Nano ESP32 - Failed to subscribe to the topic: ");

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("Arduino Nano ESP32 - MQTT broker Connected!");
}

void sendToMQTT() {
  // Publish timestamp
  char timestampBuffer[20];
  snprintf(timestampBuffer, sizeof(timestampBuffer), "%lu", millis());
  mqtt.publish("/car/timestamp", timestampBuffer);

  char tempBuffer[10] = "NaN";
  double tempF = NAN;

  // Read temperature from an analog pin
  double val = analogRead(A1);
  double fenya = (val / 1023.0) * 5;

  if (fenya > 0.01 && fenya < 4.99) {
    double r = (5 - fenya) / fenya * 4700;
    double tempC = 1 / (log(r / 10000.0) / 3950 + 1 / (25 + 273.15)) - 273.15;

    // Optional: sanity check on temperature range
    if (tempC > -40 && tempC < 125) {
      tempF = tempC * 9.0 / 5.0 + 32;
      dtostrf(tempF, 6, 2, tempBuffer);
      mqtt.publish("/car/temperature", tempBuffer);
    }
  }

  // Read voltage from an analog pin
  int rawADC = analogRead(A0);  
  float voltage = rawADC * (3.3 / 4095.0);

  // Publish voltage
  char voltageBuffer[10];
  dtostrf(voltage, 6, 2, voltageBuffer);
  mqtt.publish("/car/voltage", voltageBuffer);

  Serial.println("Sent to MQTT:");
  Serial.print("Timestamp: ");
  Serial.println(timestampBuffer);
  Serial.print("Temperature (F): ");
  Serial.println(tempBuffer);
  Serial.print("Voltage: ");
  Serial.println(voltageBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("Arduino Nano ESP32 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:" + payload);
  if (topic == PUBLISH_TOPIC) {
      Serial.println("Message received on the loopback topic!");
  } else {
      Serial.println("⚠️ Message received on an unexpected topic.");
  }
}