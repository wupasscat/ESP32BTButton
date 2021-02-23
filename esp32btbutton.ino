#include <BleKeyboard.h>

BleKeyboard bleKeyboard; // Sets Device name

const int key1 = 23; // Key 1 is on pin 23
const int key2 = 24; // Key 2 is on pin 24
const int key3 = 19; // Key 3 is on pin 19
const int key4 = 17; // Key 4 is on pin 17
const int redLed = 11; // Pin for red anode of LED
const int greenLed = 10; // Pin for green anode of LED
float maxVoltage = 4.20; // Highest battery voltage value

void setup() {
  // Configuring pinModes
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(19, INPUT);
  pinMode(17, INPUT);
  pinMode(11, OUTPUT); // Controls Red LED
  pinMode(10, OUTPUT); // Controls Green LED
  pinMode(16, INPUT); // Voltage divider input
  // Configuring ADC
  analogReadResolution(10); // Sets the resolution of the ADC to 10 bits
  // Starts serial console for debug
  Serial.begin(115200);
  Serial.println("Starting Bluetooth Keypad!");
  //bleKeyboard.begin();
}

void loop() {
  // External button presses trigger key presses
  if (bleKeyboard.isConnected()) {
    Serial.println("BT Connected");
    // Blinks LED green 5 times when bluetooth connects
    for (int i = 0; i < 5; i++) {
      digitalWrite(greenLed, HIGH);
      delay(1000);
      digitalWrite(greenLed, LOW);
      delay(1000);
    }
  }
  // Send F24 to device
  if (digitalRead(key1) == HIGH) {
    Serial.println("Sending F24 key");
    bleKeyboard.write(KEY_F24);
  }
  // Send F23 to device
  if (digitalRead(key2) == HIGH) {
    Serial.println("Sending F23 Key");
    bleKeyboard.write(KEY_F23);
  }
  // Send F22 to device
  if (digitalRead(key3) == HIGH) {
    Serial.println("Sending F22 key");
    bleKeyboard.write(KEY_F22);
  }
  // Send Ctrl+d macro to device
  if (digitalRead(key4) == HIGH) {
    Serial.println("Sending Ctrl+d macro");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(0x07); // Hex for "d" (I think)
  }
  // Releases keys after key4 is low, this could be done better.
  if digitalRead(key4) == LOW) {
    Serial.println("Releasing keys")
    delay(50);
    bleKeyboard.releaseAll();
  }

  // Battery voltage monitoring
  int sensorValue = analogRead(16); // Read analog voltage from volage divider on pin 16
  float voltage = sensorValue * (5.00 / 1023.00) * 2; // Convert value to true voltage
  if (voltage <= 3.40) { // 3.4v minimum battery voltage, 
    // Flash red LED when battery is low
    digitalWrite(redLed, HIGH);
    delay(1000);
    digitalWrite(redLed, LOW);
    delay(1000);
  }
  // Battery voltage reporting (Now reference to the 3.40v minimum)
  float batteryPercent = map(voltage, 3.40, 4.20, 0, 100); // I hope this works
  bleKeyboard.setBatteryLevel(batteryPercent); 
}
