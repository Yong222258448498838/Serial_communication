
#include <HardwareSerial.h>// Joystick pins (connected to ESP32 analog pins)
const int joystickX = 34;  // First joystick X-axis analog pin
const int joystickY = 35;  // First joystick Y-axis analog pin
const int joystickZ = 25;  // Second joystick (Z-axis) analog pin
const int Led = 12;
String transmitData;

HardwareSerial MySerial(2);
// Moving Average Filter settings
const int numSamples = 10;  // Number of ADC samples for averaging

// Exponential Moving Average (EMA) settings
float alpha = 0.15;  // Lower alpha for smoother response
float smoothedX = 100.0;
float smoothedY = 100.0;
float smoothedZ = 100.0;  // Second joystick's smoothed value

// Deadzone settings
const int deadzone = 1;  // Reduce to 1 for better small-step changes

// Function to read and average ADC values
int readSmoothADC(int pin) {
    long sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum += analogRead(pin);
    }
    return sum / numSamples;
}

void setup() {
    Serial.begin(115200);
    pinMode(Led, OUTPUT);
    Serial.begin(115200);     // Serial Monitor
    MySerial.begin(115200, SERIAL_8N1, 16, 17); // UART1: TX=17, RX=16
  
}

void loop() {
    // Read and average joystick values
    int rawX = readSmoothADC(joystickX);
    int rawY = readSmoothADC(joystickY);
    int rawZ = readSmoothADC(joystickZ);

    // Constrain values to avoid ESP32 ADC unstable ranges
    rawX = constrain(rawX, 300, 3500);
    rawY = constrain(rawY, 300, 3500);
    rawZ = constrain(rawZ, 300, 3500);

    // Map joystick values from 300-3500 to 0-200
    int mappedX = map(rawX, 300, 3500, 0, 255);
    int mappedY = map(rawY, 300, 3500, 0, 255);
    int mappedZ = map(rawZ, 300, 3500, 0, 255);

    // Apply Exponential Moving Average with Deadzone
    if (abs(mappedX - (int)smoothedX) > deadzone) {
        smoothedX = alpha * mappedX + (1.0 - alpha) * smoothedX;
    }
    if (abs(mappedY - (int)smoothedY) > deadzone) {
        smoothedY = alpha * mappedY + (1.0 - alpha) * smoothedY;
    }
    if (abs(mappedZ - (int)smoothedZ) > deadzone) {
        smoothedZ = alpha * mappedZ + (1.0 - alpha) * smoothedZ;
    }

    // Ensure X value doesn't fluctuate too much around 94
//    if ((int)smoothedX >= 90 && (int)smoothedX <= 95) {
//        smoothedX = 94;
//    }

    // Print smoothed values for both joysticks
    transmitData=String("X")+smoothedX+String("Y")+smoothedY;
    MySerial.println(transmitData);
    Serial.println(transmitData);
    delay(4);  // Small delay for stability
}
