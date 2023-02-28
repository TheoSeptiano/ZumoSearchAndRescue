#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4LCD lcd;
const uint16_t LEFT_MOTOR_SPEED = 100, RIGHT_MOTOR_SPEED = 100;//between -400 and 400
// const uint16_t FRONT_PROX_SENSOR_TOLERANCE = 5;
const uint16_t SIDE_PROX_SENSOR_TOLERANCE = 5;
boolean toggled = false;
uint8_t personFoundCooldown = 0;
uint16_t leftFrontValue, rightFrontValue, leftValue, rightValue;


void setup() 
{
  Serial.begin(9600);//Start serial communication @9600 bps
  lcd.clear();
  proxSensors.initThreeSensors();
}

void loop()
{
    proxSensors.read();

    leftFrontValue = proxSensors.countsFrontWithLeftLeds();
    rightFrontValue = proxSensors.countsFrontWithRightLeds();
    leftValue = proxSensors.countsLeftWithLeftLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    
    if (personFoundCooldown > 0)
      personFoundCooldown -= 1;

    if (leftValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the left
    {
      lcd.println("Person left");
      delay(1000);
      lcd.clear();
      toggled = false;
      personFoundCooldown = 50;
    }
    else if (rightValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the right
    {
      lcd.print("Person right");
      delay(1000);
      lcd.clear();
      toggled = false;
      personFoundCooldown = 50;
    }

  if (Serial.available()) {

    char val = Serial.read();

    if (val == 'w')
    {    
      if (toggled == false)
      {
        toggled = true;
        motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
      }
      else
      {
        toggled = false;
        motors.setSpeeds(0, 0);
      }
    }
    else if (val == 's')
    {    
      if (toggled == false)
      {
        toggled = true;
        motors.setSpeeds(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
      }
      else
      {
        toggled = false;
        motors.setSpeeds(0, 0);
      }
    }
    else if (val == 'a')
    {    
      if (toggled == false)
      {
        toggled = true;
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
      }
      else
      {
        toggled = false;
        motors.setSpeeds(0, 0);
      }
    }
    else if (val == 'd')
    {    
      if (toggled == false)
      {
        toggled = true;
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
      }
      else
      {
        toggled = false;
        motors.setSpeeds(0, 0);
      }
    }
    else if (val == 'x')
        motors.setSpeeds(0, 0);
    }
  }