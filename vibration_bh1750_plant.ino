const int xPin = A0; // X-axis pin
const int yPin = A1; // Y-axis pin
const int zPin = A2; // Z-axis pin
const int buzzerPin = 8; // Buzzer pin

float sensitivity = 0.0033; // ADXL335 sensitivity (V/g)

#include <Wire.h>
#include <BH1750.h>
#include <Servo.h>

BH1750 lightMeter;
Servo myServo;
Servo myServo1;
const int threshold = 300; // Illuminance threshold in lux
const int servoPin = 9;
const int servoPin1 = 10;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  Serial.begin(9600);

  Wire.begin();
  lightMeter.begin();
  myServo.attach(servoPin); myServo1.attach(servoPin1);
  myServo.write(0); // Initial position
  myServo1.write(0);
  Serial.begin(9600);

}

void loop() {
  int xRaw = analogRead(xPin);
  int yRaw = analogRead(yPin);
  int zRaw = analogRead(zPin);

  // Convert analog readings to g-force
  float xG = (xRaw * 5.0 / 1023.0 - 1.65) / sensitivity;
  float yG = (yRaw * 5.0 / 1023.0 - 1.65) / sensitivity;
  float zG = (zRaw * 5.0 / 1023.0 - 1.65) / sensitivity;

  // Calculate vibration magnitude
  float magnitude = sqrt(xG * xG + yG * yG + zG * zG);
  Serial.println(magnitude);

  // Trigger buzzer if vibration detected
  if (magnitude > 120) {
    digitalWrite(buzzerPin, HIGH);
    delay(5000);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
  delay(100);

  float lux = lightMeter.readLightLevel();
  Serial.print("Illuminance: ");
  Serial.print(lux);
  Serial.println(" lux");

  if (lux > threshold) {
    myServo.write(90); // Rotate 90 degrees
    myServo1.write(90);
    delay(1000);
  } else {
    myServo.write(0); // Return to initial position
    myServo1.write(0);
  }
  delay(500);
}
