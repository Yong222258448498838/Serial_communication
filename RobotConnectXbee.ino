float x;
float y;
char inchar = '0';
String inString = "";

// Motor control pins
const int motorLeftPWM = 9;    // PWM pin for left motor speed control
const int motorLeftDir = 24;   // Direction control pin for left motor
const int motorRightPWM = 10;  // PWM pin for right motor speed control
const int motorRightDir = 26;  // Direction control pin for right motor

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  // Set motor control pins as outputs
  pinMode(motorLeftPWM, OUTPUT);
  pinMode(motorLeftDir, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  pinMode(motorRightDir, OUTPUT);
  analogWrite(motorLeftPWM, 255);
  digitalWrite(motorLeftDir, LOW);
  analogWrite(motorRightPWM, 255);
  digitalWrite(motorRightDir, LOW);
}

void loop() {
  while (Serial1.available()) {
    char inChar = Serial1.read();
    inString += (char)inChar;
    if (inChar == '\n') {
      x = (inString.substring(inString.indexOf('X') + 1)).toFloat();
      y = (inString.substring(inString.indexOf('Y') + 1)).toFloat();
      inString = "";

      Serial.print("X: ");
      Serial.print(x);
      Serial.print("\t");
      Serial.print("Y: ");
      Serial.println(y);
      if (y > 160) {
        forward(y);
      } else if (y < 140) {
        back(y);
      }
      else if(x<120){
         left(x);
      }
      else if(x>150){
        rigth(x);
      }
       else {
        analogWrite(motorLeftPWM, 255);
        digitalWrite(motorLeftDir, LOW);
        analogWrite(motorRightPWM, 255);
        digitalWrite(motorRightDir, LOW);
      }
    }
  }
}
void left(int x) {
  int pwmValue3 = map(x, 135, 1, 255, 0);
  analogWrite(motorRightPWM, pwmValue3);
  digitalWrite(motorRightDir, HIGH);
  analogWrite(motorLeftPWM,pwmValue3);
  digitalWrite(motorLeftDir, HIGH);
}
void rigth(int x) {
  int pwmValue2 = map(x, 150, 255, 255, 0);
  analogWrite(motorRightPWM, pwmValue2);
  digitalWrite(motorRightDir, LOW);
  analogWrite(motorLeftPWM,pwmValue2);
  digitalWrite(motorLeftDir, LOW);
  
}
void forward(int y) {
  int pwmValue2 = map(y, 160, 255, 255, 0);
  analogWrite(motorLeftPWM, pwmValue2);
  digitalWrite(motorLeftDir, LOW);
  analogWrite(motorRightPWM, pwmValue2);
  digitalWrite(motorRightDir, HIGH);
}
void back(int y) {
  int pwmValue = map(y, 140, 0, 255, 0);
  analogWrite(motorRightPWM, pwmValue);
  digitalWrite(motorRightDir, LOW);  // Set direction for forward
  analogWrite(motorLeftPWM, pwmValue);
  digitalWrite(motorLeftDir, HIGH);
}