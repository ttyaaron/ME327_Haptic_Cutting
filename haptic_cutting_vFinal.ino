#include <BTS7960.h>
#define L_EN 7
#define R_EN 6
#define L_PWM 9                             //pin 5 supports 980hz pwm frequency
#define R_PWM 10                             //pin 6 supports 980hz pwm frequency

BTS7960 motor1(L_EN, R_EN, L_PWM, R_PWM);
const int buttonPins[3] = {4, 5, 8};

enum Mode { NONE, MODE_A, MODE_B, MODE_C};
Mode currentMode = NONE;

const int ENC_A_PIN = 2;
const int ENC_B_PIN = 3;

//const long  PPR = 1024; // TBD
//const float RAD_PER_PULSE = 2.0f * PI / PPR; //TBD
const float RAD_PER_PULSE = 0.0031415;

//Default mango
float SPRING_ZONE_RAD = 3.4;   // rad
float K_TORQUE = 0.1;   // N·m / rad
float CONST_TORQUE = 0.34;   // N·m
float B_TORQUE = 0.001f;   // N·m·s / rad

/*
//meat
const float SPRING_ZONE_RAD = 3;   // rad
const float K_TORQUE = 0.135;   // N·m / rad
const float CONST_TORQUE = 0.5;   // N·m
const float B_TORQUE = 0;   // N·m·s / rad
*/

/*
//hard fruit
const float SPRING_ZONE_RAD = 3;   // rad
const float K_TORQUE = 0.15;   // N·m / rad
const float CONST_TORQUE = 0.25;   // N·m
const float B_TORQUE = 0;   // N·m·s / rad
*/






const float K_TORQUE2PWM = 250.0;  // TBD

volatile long encoderCount = 0;
volatile int lastEncoded = 0;

float maxAngle = 9;
long lastCount = 0;
unsigned long lastMicros = 0;
bool isCut = false;



void updateEncoder()
{
  int MSB = digitalRead(ENC_A_PIN);
  int LSB = digitalRead(ENC_B_PIN);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderCount++;
  else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderCount--;

  lastEncoded = encoded;
}

/*
//meat
float cuttingTorque(float theta, float omega)
{
  float delta = theta - maxAngle;
  float tau;
  //Serial.println(delta);
  if (delta <= 0.0f) {
    tau = 0;
  } else if (delta <= SPRING_ZONE_RAD) {
    tau = -K_TORQUE * delta - B_TORQUE * omega;
  } else {
    //tau = -K_TORQUE * SPRING_ZONE_RAD  - B_TORQUE * omega - CONST_TORQUE;
    tau = - CONST_TORQUE;
    maxAngle = theta - SPRING_ZONE_RAD;
    //tau = -K_TORQUE * delta;
  }

  return tau;
}
*/


//fruit
float cuttingTorque(float theta, float omega)
{
  float delta = theta - maxAngle;
  float tau;
  //Serial.println(delta);
  if (delta <= 0.0f) {
    tau = 0;
  } else if (delta <= SPRING_ZONE_RAD) {
    tau = -K_TORQUE * delta - B_TORQUE * omega;
  } else {
    //tau = -K_TORQUE * SPRING_ZONE_RAD  - B_TORQUE * omega - CONST_TORQUE;
    tau = - CONST_TORQUE;
    maxAngle = theta - SPRING_ZONE_RAD;
    //tau = -K_TORQUE * delta;
    isCut = true;
  }

  return tau;
}

//pepper
float cuttingTorquePepper(float theta, float omega)
{
  float delta = theta - maxAngle;
  float tau;
  float back = 18;

  if (!isCut) {
    if (delta <= 0.0f) {
      tau = 0;
    } else if (delta <= SPRING_ZONE_RAD) {
      tau = -K_TORQUE * delta - B_TORQUE * omega;
    } else {
      tau = -CONST_TORQUE;
      maxAngle = theta - SPRING_ZONE_RAD;
      isCut = true;
    }
  } else {
    tau = 0;
  }

  return tau;
}



/*
// hard fruit
float cuttingTorque(float theta, float omega)
{
  float delta = theta - maxAngle;
  float tau;
  //Serial.println(delta);
  if (delta <= 0.0f) {
    tau = 0;
  } else if (delta <= SPRING_ZONE_RAD) {
    tau = -K_TORQUE * delta - B_TORQUE * omega;
  } else {
    //tau = -K_TORQUE * SPRING_ZONE_RAD  - B_TORQUE * omega - CONST_TORQUE;
    tau = - CONST_TORQUE;
    maxAngle = theta - SPRING_ZONE_RAD;
    //tau = -K_TORQUE * delta;
    isCut = true;
  }

  return tau;
}
*/

/*
float cuttingTorque(float theta, float omega)
{
  float delta = theta - maxAngle;
  float tau;

  // Define blend width as 5% of the spring zone
  const float BLEND_WIDTH = 0.05f * SPRING_ZONE_RAD;
  const float BLEND_START = SPRING_ZONE_RAD - BLEND_WIDTH;

  // Torque in 3 zones: preload, spring, blended transition, constant
  if (delta <= 0.0f) {
    tau = 0;
  

  } else if (delta <= SPRING_ZONE_RAD) {
    tau = -K_TORQUE * delta - B_TORQUE * omega;


  } else if (delta < BLEND_END) {
    // Hermite cubic blend between spring torque and constant torque
    float x = (delta - BLEND_START) / BLEND_WIDTH;  // normalized in [0,1]
    float smoothstep = x * x * (3 - 2 * x);         // smooth blend weight

    float tau_spring = -K_TORQUE * delta;
    float tau_const  = -K_TORQUE * SPRING_ZONE_RAD - CONST_TORQUE;

    tau = (1.0f - smoothstep) * tau_spring + smoothstep * tau_const - B_TORQUE * omega;
  }
  else {
    tau = -K_TORQUE * SPRING_ZONE_RAD  - B_TORQUE * omega; //- CONST_TORQUE
  }

  // Update max angle if exceeded
  if (theta > maxAngle) maxAngle = theta;

  return tau;
}
*/

void setup()
{
  pinMode(ENC_A_PIN, INPUT_PULLUP);
  pinMode(ENC_B_PIN, INPUT_PULLUP);

  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B_PIN), updateEncoder, CHANGE);

  Serial.begin(9600);
  lastMicros = micros();
  TCCR1B = TCCR1B & 0b11111000 | 0x01;


  motor1.begin();
  motor1.enable();

  motor1.pwm = 0;
  motor1.back(); 
}

void switchMode(int buttonIndex) {
  if (buttonIndex == 0 && currentMode != MODE_A) {
    currentMode = MODE_A;
    Serial.println("Mode A activated");
  } 
  else if (buttonIndex == 1 && currentMode != MODE_B) {
    currentMode = MODE_B;
    Serial.println("Mode B activated");
  }
  else if (buttonIndex == 2 && currentMode != MODE_C) {
    currentMode = MODE_C;
    Serial.println("Mode C activated");
  }
}

void loop()
{

  //Serial.print("B0: ");
  //Serial.print(digitalRead(buttonPins[0]));
  //Serial.print(" | B1: ");
  //Serial.println(digitalRead(buttonPins[1]));
  //delay(200); // Slow it down

  
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      switchMode(i);
      delay(300); // crude debounce: prevent multiple triggers
    }
  }

  noInterrupts();
  long count = encoderCount;
  interrupts();

  float theta  = -(count * RAD_PER_PULSE); // rad
  //Serial.println(theta);
  unsigned long now = micros();
  float dt = (now - lastMicros) * 1e-6f; 
  if (dt <= 0) dt = 1e-6f;
  float omega = (count - lastCount) * RAD_PER_PULSE / dt;

  lastCount  = count;
  lastMicros = now;

  if (isCut and theta <= 0.0f){
      maxAngle = 9;
      isCut = false;
  }
  
  float T = 0;
  if (currentMode == MODE_A) {
    // soft fruit
    SPRING_ZONE_RAD = 3.4;   // rad
    K_TORQUE = 0.1;   // N·m / rad
    CONST_TORQUE = 0.34;   // N·m
    B_TORQUE = 0.001f;   // N·m·s / rad
    T = cuttingTorque(theta, omega);

  } else if (currentMode == MODE_B) {
    //hard fruit
    SPRING_ZONE_RAD = 3;   // rad
    K_TORQUE = 0.15;   // N·m / rad
    CONST_TORQUE = 0.25;   // N·m
    B_TORQUE = 0;   // N·m·s / rad
    T = cuttingTorque(theta, omega);

  } else if (currentMode == MODE_C) {
    //springy
    SPRING_ZONE_RAD = 5;   // rad
    K_TORQUE = 0.07;   // N·m / rad
    CONST_TORQUE = 0.25;   // N·m
    B_TORQUE = 0.005;   // N·m·s / rad
    T = cuttingTorquePepper(theta, omega);
  }

 // float T = cuttingTorque(float theta, float omega, float SPRING_ZONE_RAD, float CONST_TORQUE, float K_TORQUE, float B_TORQUE); // N·m
  //float T = cuttingTorque(theta, omega);


  bool dir = (T < 0);
  int  pwm = abs(T) * K_TORQUE2PWM;
  if (pwm > 255) pwm = 255;
  if (pwm == 0){
    motor1.disable();
  } else{
    motor1.enable();
  }

  Serial.println(currentMode);
  //Serial.println(pwm);
  motor1.pwm = pwm;
  motor1.back();

  //Serial.println(encoderCount);
}