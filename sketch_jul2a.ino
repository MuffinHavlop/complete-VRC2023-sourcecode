
#include <PS2X_lib.h>  
#include <Adafruit_PWMServoDriver.h> 
#include <Wire.h>

#define PS2_DAT 12
#define PS2_CMD 13
#define PS2_CLK 14
#define PS2_ATT 15

#define PWM_DC1A 14 
#define PWM_DC1B 15 
#define PWM_DC2A 8 
#define PWM_DC2B 9 
#define PWM_DC3A 10 
#define PWM_DC3B 11 
#define PWM_DC4A 12 
#define PWM_DC4B 13 

#define MOT_LEFT 1
#define MOT_RIGHT 2
#define SHOOTER_1 3

#define SPD_FAST 2047
#define SPD_MAX 4095

Adafruit_PWMServoDriver pwm;  
PS2X ps2;

void setup() { 
  Serial.begin(115200); 

  Serial.println("Mastodon's BanhMi Connected To PC. Proceeding to connect PS2 Controller");

  while (ps2.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT) != 0) {
    Serial.println("Err: PS2 Controller Not Connected");
    delay(500);
  } 

  Serial.println("Controller works normally");

  pwm.begin();                           
  pwm.setOscillatorFrequency(27000000);  
  pwm.setPWMFreq(50);   
             
}

void ctrl_dc(uint8_t motor, int16_t speed) {
  switch (motor) {
    case 1: 
      pwm.setPWM(PWM_DC1A, 0, ((speed > 0) ? speed : 0) );  
      pwm.setPWM(PWM_DC1B, 0, ((speed < 0) ? (-speed) : 0) );
      break;
    case 2: 
      pwm.setPWM(PWM_DC2A, 0, ((speed > 0) ? speed : 0) );
      pwm.setPWM(PWM_DC2B, 0, ((speed < 0) ? (-speed) : 0) ) ;
      break;
    case 3:  
      pwm.setPWM(PWM_DC3A, 0, ((speed > 0) ? speed : 0) );
      pwm.setPWM(PWM_DC3B, 0, ((speed < 0) ? (-speed) : 0) );
      break;
    case 4:
      pwm.setPWM(PWM_DC4A, 0, ((speed > 0) ? speed : 0) );
      pwm.setPWM(PWM_DC4B, 0, ((speed < 0) ? (-speed) : 0) );
      break;
  }
}

bool clockwise = false;
bool clockwise_off = false;
bool counter_clockwise = false;
bool counter_clockwise_off = false;

void loop() {

  ps2.read_gamepad(); 

  ctrl_dc(MOT_LEFT, map(ps2.Analog(PSS_LY), 0, 255, -SPD_FAST, SPD_FAST));
  ctrl_dc(MOT_RIGHT, map(ps2.Analog(PSS_RY), 0, 255, SPD_FAST, -SPD_FAST));

  if (ps2.Button(PSB_R1)) {
    ctrl_dc(SHOOTER_1, (true) ? 0 : SPD_MAX); 
    }

  if (ps2.Button(PSB_L1)) {
    ctrl_dc(SHOOTER_1, (true) ? 0 : -SPD_MAX); 
    }
  if (ps2.Button(PSB_R2)) {
    ctrl_dc(SHOOTER_1, (true) ? SPD_MAX : 0);
  }
  if (ps2.Button(PSB_L2)) {
    ctrl_dc(SHOOTER_1, (true) ? -SPD_MAX : 0);
  }
}
  