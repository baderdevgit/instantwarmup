#include <Arduino.h>
#include <ESP32Servo.h>

// Assign PWM-capable pins for each servo
int bottom_scoop_sweep_pin = 27;
int bottom_scoop_tilt_pin  = 26;
int top_scoop_sweep_pin    = 25;
int top_scoop_tilt_pin     = 33;
int middle_scoop_pin       = 32;
int monster_tilt_pin       = 19;
int monster_lid_pin        = 18;

int left_bicept_relay  = 4;
int right_bicept_relay = 5;
int left_tricept_relay = 16;
int right_tricept_relay= 17;
int left_front_delt_relay    = 18;
int right_front_delt_relay   = 19;
int left_side_delt_relay    = 21;
int right_side_delt_relay   = 22;

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
  bot_scoop_tilt_servo.write(0);
  top_scoop_sweep_servo.write(90);
  top_scoop_tilt_servo.write(0);
  middle_servo.write(90);
  monster_tilt_servo.write(0);
  monster_lid_servo.write(0);

  digitalWrite(left_bicept_relay, LOW);
  digitalWrite(right_bicept_relay, LOW);
  digitalWrite(left_tricept_relay, LOW);
  digitalWrite(right_tricept_relay, LOW);
  digitalWrite(left_front_delt_relay, LOW);
  digitalWrite(right_front_delt_relay, LOW);
  digitalWrite(left_side_delt_relay, LOW);
  digitalWrite(right_side_delt_relay, LOW);
}

void loop() {

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 'd') {
      BotScoopSequence();
    }
  }

}

void SupplementSequence() {

  BotScoopSequence();

  delay(250);

  TopScoopSequence();

  delay(500);

  //open the monster
  MonsterCanSequence();
  
  delay(500);

  //move main scoop up
  middle_servo.write(0); //not sure if 0 or 180

  //back and forth of main scoop
}

void BotScoopSequence() {
  //move bottom scoop
  bot_scoop_sweep_servo.write(45); //might be 135 idk
  delay(250);
  //turn it
  bot_scoop_tilt_servo.write(180);
  delay(500);
  bot_scoop_tilt_servo.write(0);
  delay(250);
  //move bottom scoop back
  bot_scoop_sweep_servo.write(90);
}

void TopScoopSequence() {
  //move bottom scoop
  top_scoop_sweep_servo.write(45); //might be 135 idk
  delay(250);
  //turn it
  top_scoop_tilt_servo.write(180);
  delay(500);
  top_scoop_tilt_servo.write(0);
  delay(250);
  //move bottom scoop back
  top_scoop_sweep_servo.write(90);
}

void MonsterCanSequence() {
  OpenCan();
  delay(500);
  TiltCan();
  delay(500);
  UnTiltCan();
}

void OpenCan() {
  monster_lid_servo.write(180);
}

void TiltCan() {
  monster_tilt_servo.write(120);
}

void UnTiltCan() {
    monster_tilt_servo.write(0);
}

enum Side {
  left,
  right
};

void ToggleBicept(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_bicept_relay, on ? HIGH : LOW);
  } 
  else {
      digitalWrite(right_bicept_relay, on ? HIGH : LOW);
  }
}

void ToggleTricept(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_tricept_relay, on ? HIGH : LOW);
  } 
  else {
      digitalWrite(right_tricept_relay, on ? HIGH : LOW);
  }
}

void ToggleSideDelt(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_side_delt_relay, on ? HIGH : LOW);
  } 
  else {
      digitalWrite(right_side_delt_relay, on ? HIGH : LOW);
  }
}

void ToggleFrontDelt(Side side, bool on) {
  if (side == left) {
      digitalWrite(left_front_delt_relay, on ? HIGH : LOW);
  } 
  else {
      digitalWrite(right_front_delt_relay, on ? HIGH : LOW);
  }
}