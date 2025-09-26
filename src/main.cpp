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
  Serial.println("Attaching all servos and setting to 90°...");

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
  bot_scoop_tilt_servo.write(90);
  top_scoop_sweep_servo.write(90);
  top_scoop_tilt_servo.write(90);
  middle_servo.write(90);
  monster_tilt_servo.write(90);
  monster_lid_servo.write(90);
}

void loop() {
  // Nothing for now - servos stay at 90°
}
