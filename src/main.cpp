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

int currentDelay = 500;  // start at 500ms

// Servo pins
int bottom_scoop_sweep_pin = 27;
int bottom_scoop_tilt_pin  = 26;
int top_scoop_sweep_pin    = 25;
int top_scoop_tilt_pin     = 33;
int middle_scoop_pin       = 32;
int monster_tilt_pin       = 13;
int monster_lid_pin        = 12;

// Relay pins
int left_bicept_relay  = 23;
int right_bicept_relay = 22;
int left_tricept_relay = 21;
int right_tricept_relay= 19;
int left_front_delt_relay  = 18;
int right_front_delt_relay = 5;
int left_forearm_relay    = 4;
int right_forearm_relay   = 15;

// Servo objects
Servo bot_scoop_sweep_servo;
Servo bot_scoop_tilt_servo;
Servo top_scoop_sweep_servo;
Servo top_scoop_tilt_servo;
Servo middle_servo;
Servo monster_tilt_servo;
Servo monster_lid_servo;

// Muscle state flags
bool isRightForearm = false;
bool isLeftForearm = false;
bool isRightDelt = false;
bool isLeftDelt = false;

// FreeRTOS task handle for muscle sequence
TaskHandle_t muscleTaskHandle = NULL;

// Muscle sequence task
void muscleTask(void * parameter) {
  while(true) {
    MuscleContractionSequence();  // runs indefinitely
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Program Ready: sequences will start immediately");

  // Attach servos
  bot_scoop_sweep_servo.attach(bottom_scoop_sweep_pin);
  bot_scoop_tilt_servo.attach(bottom_scoop_tilt_pin);
  top_scoop_sweep_servo.attach(top_scoop_sweep_pin);
  top_scoop_tilt_servo.attach(top_scoop_tilt_pin);
  middle_servo.attach(middle_scoop_pin);
  monster_tilt_servo.attach(monster_tilt_pin);
  monster_lid_servo.attach(monster_lid_pin);

  // Initial servo positions
  bot_scoop_sweep_servo.write(90); delay(50);
  bot_scoop_tilt_servo.write(0); delay(50);
  top_scoop_sweep_servo.write(90); delay(50);
  top_scoop_tilt_servo.write(15); delay(50);
  middle_servo.write(70); delay(50);
  monster_tilt_servo.write(180); delay(50);
  monster_lid_servo.write(0); delay(50);

  // Set relays as output and HIGH
  int relays[] = {left_bicept_relay, right_bicept_relay, left_tricept_relay, right_tricept_relay,
                  left_front_delt_relay, right_front_delt_relay, left_forearm_relay, right_forearm_relay};
  for(int i=0;i<8;i++) pinMode(relays[i], OUTPUT), digitalWrite(relays[i], HIGH);

  // Start muscle sequence task immediately
  xTaskCreatePinnedToCore(
    muscleTask,        // Task function
    "MuscleTask",      // Task name
    4096,              // Stack size
    NULL,              // Parameters
    1,                 // Priority
    &muscleTaskHandle, // Task handle
    1                  // Run on core 1
  );

  // Run supplement sequence once immediately
  SupplementSequence();
}

void loop() {
  // Nothing needed in loop; everything runs automatically
}

// ---------- Sequences ----------
void MuscleContractionSequence() {
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

  if(currentDelay > 100){
    currentDelay -= 30;
    if(currentDelay < 200) currentDelay = 100;
  }
}

void SupplementSequence() {
  BotScoopSequence(); delay(500);
  TopScoopSequence(); delay(500);
  MonsterCanSequence(); delay(1000);

  middle_servo.write(180); delay(500);

  for(int i=0;i<10;i++){
    middle_servo.write(120); delay(250);
    middle_servo.write(180); delay(500);
  }

  middle_servo.write(70); delay(200);
}

void BotScoopSequence() {
  bot_scoop_sweep_servo.write(20); delay(500);
  bot_scoop_tilt_servo.write(180); delay(750);
  bot_scoop_tilt_servo.write(0); delay(500);
  bot_scoop_sweep_servo.write(90);
}

void TopScoopSequence() {
  top_scoop_sweep_servo.write(30); delay(500);
  top_scoop_tilt_servo.write(180); delay(750);
  top_scoop_tilt_servo.write(15); delay(500);
  top_scoop_sweep_servo.write(90);
}

void MonsterCanSequence() {
  OpenCan(); delay(2000);
  TiltCan(); delay(3000);
  UnTiltCan();
}

void OpenCan() { monster_lid_servo.write(170); }
void TiltCan() { monster_tilt_servo.write(50); }
void UnTiltCan() { monster_tilt_servo.write(180); }

// ---------- Muscle Functions ----------
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
  digitalWrite(side == left ? left_bicept_relay : right_bicept_relay, on ? LOW : HIGH);
}

void ToggleTricept(Side side, bool on) {
  digitalWrite(side == left ? left_tricept_relay : right_tricept_relay, on ? LOW : HIGH);
}

void ToggleForearm(Side side, bool on) {
  digitalWrite(side == left ? left_forearm_relay : right_forearm_relay, on ? LOW : HIGH);
}

void ToggleFrontDelt(Side side, bool on) {
  digitalWrite(side == left ? left_front_delt_relay : right_front_delt_relay, on ? LOW : HIGH);
}
