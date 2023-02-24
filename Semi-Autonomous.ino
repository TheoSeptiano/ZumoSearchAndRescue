#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
//Zumo32U4ButtonC buttonC;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;

const uint16_t TOLERANCE = 50;
const uint8_t NUM_SENSORS = 3;
const uint16_t LEFT_MOTOR_SPEED = 100, RIGHT_MOTOR_SPEED = 100;//between -400 and 400
uint16_t lineSensorValues[NUM_SENSORS];

uint16_t getSensorValues()
{
  return lineSensors.readLine(lineSensorValues);
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

  if (NUM_SENSORS == 3)
    lineSensors.initThreeSensors();
  else
    lineSensors.initFiveSensors();

  calibrateSensors();
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonAPressed = false;
  bool buttonBPressed = false;
  uint16_t position;  

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
    // position 2000 is central, 1000 centre left, 0 furthest left and inverse for right
    //getSensorValues();

    //lcd.print(lineSensorValues[0]);

    // if (position < 2000 && position > 0)//line on the left
    // {
    //   motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
    // }
    // else if (position > 2000 && position < 4000)//line on the right
    // {
    //   motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
    // }
    // else
    // {
    //   motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
    
    getSensorValues();

    if (NUM_SENSORS == 5)
    {
        if (lineSensorValues[4]>TOLERANCE && lineSensorValues[0]>TOLERANCE)//line in center
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);//reverse
        delay(300);  
      }
      else if (lineSensorValues[0]>TOLERANCE)//line on left most sensor
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[1]>TOLERANCE)//line on second-to-left most sensor
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[3]>TOLERANCE)//line on second-to-right most sensor
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[4]>TOLERANCE)//line on right most sensor
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(100);
      }
      else if (lineSensorValues[4]<TOLERANCE && lineSensorValues[3]<TOLERANCE && lineSensorValues[1]<TOLERANCE && lineSensorValues[0]<TOLERANCE)
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(50);
      }      
    }
    else
    {
      if (lineSensorValues[2]>TOLERANCE && lineSensorValues[0]>TOLERANCE)//line in center
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);//reverse
        delay(300);  
      }
      else if (lineSensorValues[0]>TOLERANCE)//line on left most sensor
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        delay(100);      
      }
      else if (lineSensorValues[2]>TOLERANCE)//line on right most sensor
      {
        motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(100);
      }
      else if (lineSensorValues[0]<TOLERANCE && lineSensorValues[2]<TOLERANCE)
      {
        motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        delay(50);
      }
    }

    
    // lineSensors.read(*lineSensorValues);

    // delay(100);
  } 
}