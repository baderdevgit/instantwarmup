#include <Arduino.h>
#include <ESP32Servo.h>

enum Side {
  left,
  right
};

void BotScoopSequence();
void TopScoopSequence();
void MonsterCanSequence();
void OpenCan();
void TiltCan();
void UnTiltCan();
void SupplementSequence();
void MuscleContractionSequence();
void CurlArm(Side side);
void StraightenArm(Side side);
void ToggleBicept(Side side, bool on);
void ToggleTricept(Side side, bool on);
void ToggleForearm(Side side, bool on);
void ToggleFrontDelt(Side side, bool on);

void TestRelays(int relayPins[], int numRelays);

int currentDelay = 500;  // start at 1 second

// Assign PWM-capable pins for each servo
int bottom_scoop_sweep_pin = 27;
int bottom_scoop_tilt_pin  = 26;
int top_scoop_sweep_pin    = 25;
int top_scoop_tilt_pin     = 33;
int middle_scoop_pin       = 32;
int monster_tilt_pin       = 13;
int monster_lid_pin        = 12;

int left_bicept_relay  = 23;
int right_bicept_relay = 22;
int left_tricept_relay = 21;
int right_tricept_relay= 19;
int left_front_delt_relay  = 18;
int right_front_delt_relay = 5;
int left_forearm_relay    = 4;
int right_forearm_relay   = 15;

int relayArray[] = {23, 22, 21, 19, 18, 5, 4, 15};

// Servo objects
Servo bot_scoop_sweep_servo;
Servo bot_scoop_tilt_servo;
Servo top_scoop_sweep_servo;
Servo top_scoop_tilt_servo;
Servo middle_servo;
Servo monster_tilt_servo;
Servo monster_lid_servo; // fixed typo: should be a Servo, not int

void setup() {
  Serial.begin(115200);
  Serial.println("Program Ready, Press D to trigger");

  // Attach all servos to their pins
  bot_scoop_sweep_servo.attach(bottom_scoop_sweep_pin);
  bot_scoop_tilt_servo.attach(bottom_scoop_tilt_pin);
  top_scoop_sweep_servo.attach(top_scoop_sweep_pin);
  top_scoop_tilt_servo.attach(top_scoop_tilt_pin);
  middle_servo.attach(middle_scoop_pin);
  monster_tilt_servo.attach(monster_tilt_pin);
  monster_lid_servo.attach(monster_lid_pin);

  // Set all servos to 90°
  bot_scoop_sweep_servo.write(90);
  delay(50);
  bot_scoop_tilt_servo.write(0);
  delay(50);
  top_scoop_sweep_servo.write(90);
  delay(50);
  top_scoop_tilt_servo.write(15);
  delay(50);
  middle_servo.write(70);
  delay(50);
  monster_tilt_servo.write(180);
  delay(50);
  monster_lid_servo.write(0);
  delay(50);

  pinMode(left_bicept_relay, OUTPUT);
  pinMode(right_bicept_relay, OUTPUT);
  pinMode(left_tricept_relay, OUTPUT);
  pinMode(right_tricept_relay, OUTPUT);
  pinMode(left_front_delt_relay, OUTPUT);
  pinMode(right_front_delt_relay, OUTPUT);
  pinMode(left_forearm_relay, OUTPUT);
  pinMode(right_forearm_relay, OUTPUT);

  digitalWrite(left_bicept_relay, HIGH);
  digitalWrite(right_bicept_relay, HIGH);
  digitalWrite(left_tricept_relay, HIGH);
  digitalWrite(right_tricept_relay, HIGH);
  digitalWrite(left_front_delt_relay, HIGH);
  digitalWrite(right_front_delt_relay, HIGH);
  digitalWrite(left_forearm_relay, HIGH);
  digitalWrite(right_forearm_relay, HIGH);
}

void loop() {

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 'd') {
      Serial.println("D key pressed");

      SupplementSequence();
      MuscleContractionSequence();
    }
  }

}

void TestRelays(int relayPins[], int numRelays = 8) {
  for (int i = 0; i < numRelays; i++) {
    digitalWrite(relayPins[i], HIGH);  // Turn relay ON
    delay(500);
    digitalWrite(relayPins[i], LOW);   // Turn relay OFF
    delay(200);
  }
}

bool isRightForearm = false;
bool isLeftForearm = false;
bool isRightDelt = false;
bool isLeftDelt = false;

void MuscleContractionSequence() {
  // Pick a random function (0-3)
  int choice = random(4);
  switch (choice) {
    case 0: CurlArm(right); break;
    case 1: StraightenArm(right); break;
    case 2: ToggleForearm(right, !isRightForearm); isRightForearm = !isRightForearm; break;
    case 3: ToggleFrontDelt(right, !isRightDelt); isRightDelt = !isRightDelt; break;
  }

  int choiceL = random(4);
  switch (choiceL) {
    case 0: CurlArm(left); break;
    case 1: StraightenArm(left); break;
    case 2: ToggleForearm(left, !isLeftForearm); isLeftForearm = !isLeftForearm; break;
    case 3: ToggleFrontDelt(left, !isLeftDelt); isLeftDelt = !isLeftDelt; break;
  }

  delay(currentDelay);

  // Linearly reduce delay until we hit 100 ms
  if (currentDelay > 100) {
    currentDelay -= 45;  // 1000 -> 910 -> ... -> 100
    if (currentDelay < 100) currentDelay = 100;
  }
}

void SupplementSequence() {
      BotScoopSequence();
      delay(500);
      TopScoopSequence();
      delay(500);
      MonsterCanSequence();
      delay(1000);

      middle_servo.write(180); //not sure if 0 or 180

      delay(500);
      for(int i = 0; i<10; i++) {
        middle_servo.write(120); //not sure if 0 or 180
        delay(250);
        middle_servo.write(180); //not sure if 0 or 180
        delay(500);
      }

      middle_servo.write(70); //not sure if 0 or 180

      delay(200);
}

void BotScoopSequence() {
  //move bottom scoop
  bot_scoop_sweep_servo.write(20);
  delay(500);
  //turn it
  bot_scoop_tilt_servo.write(180);
  delay(750);
  bot_scoop_tilt_servo.write(0);
  delay(500);
  //move bottom scoop back
  bot_scoop_sweep_servo.write(90);
}

void TopScoopSequence() {
  //move bottom scoop
  top_scoop_sweep_servo.write(30);
  delay(500);
  //turn it
  top_scoop_tilt_servo.write(180);
  delay(750);
  top_scoop_tilt_servo.write(15);
  delay(500);
  //move bottom scoop back
  top_scoop_sweep_servo.write(90);
}

void MonsterCanSequence() {
  OpenCan();
  delay(2000);
  TiltCan();
  delay(3000);
  UnTiltCan();
}

void OpenCan() {
  monster_lid_servo.write(170);
}

void TiltCan() {
  monster_tilt_servo.write(50);
}

void UnTiltCan() {
    monster_tilt_servo.write(180);
}


void CurlArm(Side side) {
  ToggleBicept(side, true);
  ToggleTricept(side, false);
  ToggleForearm(side, false);
}

void StraightenArm(Side side) {
  ToggleBicept(side, false);
  ToggleTricept(side, true);
}

void ToggleBicept(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_bicept_relay, on ? LOW : HIGH);
  } 
  else {
      digitalWrite(right_bicept_relay, on ? LOW : HIGH);
  }
}

void ToggleTricept(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_tricept_relay, on ? LOW : HIGH);
  } 
  else {
      digitalWrite(right_tricept_relay, on ? LOW : HIGH);
  }
}

void ToggleForearm(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_forearm_relay, on ? LOW : HIGH);
  } 
  else {
      digitalWrite(right_forearm_relay, on ? LOW : HIGH);
  }
}

void ToggleFrontDelt(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_front_delt_relay, on ? LOW : HIGH);
  } 
  else {
      digitalWrite(right_front_delt_relay, on ? LOW : HIGH);
  }
}