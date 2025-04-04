#include <Wire.h>
#include "Adafruit_HTU21DF.h"

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// Pin definitions
const int soilMoisturePin = A0;
const int pumpRelayPin = 4;
const int tempRelayPin = 5;
const int humidRelayPin = 6;

// Thresholds (Adjust as needed)
const int soilDryThreshold = 500;       // Lower values = wetter
const float tempThreshold = 30.0;       // Degrees Celsius
const float humidityThreshold = 70.0;   // Percent

void setup() {
  Serial.begin(9600);

  // Initialize relay pins
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(tempRelayPin, OUTPUT);
  pinMode(humidRelayPin, OUTPUT);

  digitalWrite(pumpRelayPin, HIGH);  // Relay OFF (assuming active LOW)
  digitalWrite(tempRelayPin, HIGH);
  digitalWrite(humidRelayPin, HIGH);

  // Initialize HTU21D
  if (!htu.begin()) {
    Serial.println("Couldn't find HTU21D sensor!");
    while (1);
  }

  Serial.println("HTU21D sensor initialized.");
}

void loop() {
  // ---- Soil Moisture ----
  int soilValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);

  if (soilValue > soilDryThreshold) {
    digitalWrite(pumpRelayPin, LOW); // Turn ON pump
    Serial.println("Soil is dry. Pump ON.");
  } else {
    digitalWrite(pumpRelayPin, HIGH); // Turn OFF pump
    Serial.println("Soil is moist. Pump OFF.");
  }

  // ---- Temperature and Humidity ----
  float temp = htu.readTemperature();
  float humidity = htu.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Temperature control
  if (temp > tempThreshold) {
    digitalWrite(tempRelayPin, LOW);  // Turn ON relay
    Serial.println("Temperature high. Temp Relay ON.");
  } else {
    digitalWrite(tempRelayPin, HIGH); // Turn OFF
    Serial.println("Temperature normal. Temp Relay OFF.");
  }

  // Humidity control
  if (humidity > humidityThreshold) {
    digitalWrite(humidRelayPin, LOW);  // Turn ON
    Serial.println("Humidity high. Humidity Relay ON.");
  } else {
    digitalWrite(humidRelayPin, HIGH); // Turn OFF
    Serial.println("Humidity normal. Humidity Relay OFF.");
  }

  delay(3000); // Wait before next reading
}
