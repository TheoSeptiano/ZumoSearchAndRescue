#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
//Zumo32U4ButtonC buttonC;
Zumo32U4Buzzer buzzer;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
const uint16_t LINE_SENSOR_TOLERANCE = 200;//50 worked in 93.. keep adjusting this
const uint16_t FRONT_PROX_SENSOR_TOLERANCE = 5;
const uint16_t SIDE_PROX_SENSOR_TOLERANCE = 5;
const uint8_t NUM_LINE_SENSORS = 3;
const uint16_t LEFT_MOTOR_SPEED = 100, RIGHT_MOTOR_SPEED = 100;//between -400 and 400
const uint8_t NUM_PEOPLE = 3;
uint16_t lineSensorValues[NUM_LINE_SENSORS];

void getSensorValues()//gets values for both line and prox sensors
{
  lineSensors.readLine(lineSensorValues);
  proxSensors.read();
}

void calibrateSensors()
{
  lcd.clear();

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void setup() {
  // put your setup code here, to run once:
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print("Press A");

  if (NUM_LINE_SENSORS == 3)
    lineSensors.initThreeSensors();
  else
    lineSensors.initFiveSensors();

  proxSensors.initThreeSensors();

  calibrateSensors();
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonAPressed = false;
  bool buttonBPressed = false;
  uint16_t position, leftFrontValue, rightFrontValue, leftValue, rightValue;
  uint8_t personFoundCooldown = 0;

  motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);

  while (!buttonBPressed)//B aborts the mission
  {
    buttonAPressed = buttonA.getSingleDebouncedPress();
    buttonBPressed = buttonB.getSingleDebouncedPress();
    // if (buttonAPressed)//go
    // {
    //   lcd.clear();
    //   //lcd.print("A was pressed!");
    //   lcd.print(position);
    // }
    
    getSensorValues();
    leftFrontValue = proxSensors.countsFrontWithLeftLeds();
    rightFrontValue = proxSensors.countsFrontWithRightLeds();
    leftValue = proxSensors.countsLeftWithLeftLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    
    if (personFoundCooldown > 0)
      personFoundCooldown -= 1;

    if (leftFrontValue >= FRONT_PROX_SENSOR_TOLERANCE || rightFrontValue >= FRONT_PROX_SENSOR_TOLERANCE) {//object in front
      motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
      delay(500);      
    }
    else if (leftValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the left
    {//TODO: MAKE LIST THAT ADDS EACH TURN TO IT
      motors.setSpeeds(0,0);
      lcd.println("Person left");
      delay(300);
      lcd.clear();
      personFoundCooldown = 50;
    }
    else if (rightValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the right
    {
      motors.setSpeeds(0,0);
      lcd.print("Person right");
      delay(300);
      lcd.clear();
      personFoundCooldown = 50;
    }

    if (NUM_LINE_SENSORS == 5)
    {
        if (lineSensorValues[4]>LINE_SENSOR_TOLERANCE && lineSensorValues[0]>LINE_SENSOR_TOLERANCE)//line in center
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);//reverse
        delay(300);  
      }
      else if (lineSensorValues[0]>LINE_SENSOR_TOLERANCE)//line on left most sensor
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[1]>LINE_SENSOR_TOLERANCE)//line on second-to-left most sensor
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[3]>LINE_SENSOR_TOLERANCE)//line on second-to-right most sensor
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[4]>LINE_SENSOR_TOLERANCE)//line on right most sensor
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(100);
      }
      else if (lineSensorValues[4]<LINE_SENSOR_TOLERANCE && lineSensorValues[3]<LINE_SENSOR_TOLERANCE && lineSensorValues[1]<LINE_SENSOR_TOLERANCE && lineSensorValues[0]<LINE_SENSOR_TOLERANCE)
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(50);
      }      
    }
    else //NUM_SENSORS is 3
    {
      if (lineSensorValues[2]>LINE_SENSOR_TOLERANCE && lineSensorValues[0]>LINE_SENSOR_TOLERANCE)//line in center
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);//turn around
        delay(800);  
      }
      else if (lineSensorValues[0]>LINE_SENSOR_TOLERANCE)//line on left most sensor
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[2]>LINE_SENSOR_TOLERANCE)//line on right most sensor
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(100);
      }
      else if (lineSensorValues[0]<LINE_SENSOR_TOLERANCE && lineSensorValues[2]<LINE_SENSOR_TOLERANCE)
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(50);
      }
    }
  } 
}