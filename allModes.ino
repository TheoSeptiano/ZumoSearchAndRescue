#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4Buzzer buzzer;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
const uint16_t LINE_SENSOR_TOLERANCE = 150;//50 worked in 9331, but 150 seems to be most consistent
const uint16_t FRONT_PROX_SENSOR_TOLERANCE = 5;
const uint16_t SIDE_PROX_SENSOR_TOLERANCE = 5;
const uint8_t NUM_LINE_SENSORS = 3;
const uint16_t LEFT_MOTOR_SPEED = 100, RIGHT_MOTOR_SPEED = 100;//between -400 and 400
const uint8_t NUM_PEOPLE = 3;
uint16_t lineSensorValues[NUM_LINE_SENSORS];
bool buttonAPressed = false;
uint16_t leftFrontValue, rightFrontValue, leftValue, rightValue;
uint8_t personFoundCooldown = 0;

void calibrateSensors()//taken from the Pololu Zumo examples
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

void modeOne()//manual
{
  boolean stop = false;
  boolean toggled = false;
  while (!stop)
  {
    proxSensors.read();
    buttonAPressed = buttonA.getSingleDebouncedPress();
    if (buttonAPressed)
    {
      stop=true;
    }
    leftFrontValue = proxSensors.countsFrontWithLeftLeds();
    rightFrontValue = proxSensors.countsFrontWithRightLeds();
    leftValue = proxSensors.countsLeftWithLeftLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    
    if (personFoundCooldown > 0)
      personFoundCooldown -= 1;

    if (leftValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the left
    {
      lcd.print("Person left");//prints to the LCD screen when a person has been found
      delay(500);
      lcd.clear();
      personFoundCooldown = 50;
    }
    else if (rightValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the right
    {
      lcd.print("Person right");
      delay(500);
      lcd.clear();
      personFoundCooldown = 50;
    }

  if (Serial.available()) {

    char val = Serial.read();

    if (val == 'w')//the processing gui sends characters to be read and used here
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
    {
        motors.setSpeeds(0, 0);
        stop = true;
    }
  }
}
}

void modeTwo()//semi-autonomous
{
  calibrateSensors();
  boolean stop = false;
  motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
  while (!stop)
  {
    lineSensors.readLine(lineSensorValues);
    proxSensors.read();
    leftFrontValue = proxSensors.countsFrontWithLeftLeds();
    rightFrontValue = proxSensors.countsFrontWithRightLeds();
    leftValue = proxSensors.countsLeftWithLeftLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    

    if (personFoundCooldown > 0)
      personFoundCooldown -= 1;

    if (leftValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the left
    {
      motors.setSpeeds(0,0);
      lcd.println("Person left");
      delay(300);
      lcd.clear();
      modeTwoUserInput();
      personFoundCooldown = 50;
    }
    else if (rightValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the right
    {
      motors.setSpeeds(0,0);
      lcd.print("Person right");
      delay(300);
      lcd.clear();
      modeTwoUserInput();
      personFoundCooldown = 50;
    }
  }
}

void modeTwoUserInput()//mode two switches to mode 1-esque user input, but only allows one command before going back to autonomous mode
{
    lcd.print("Command needed");
    boolean toggled = false;
    boolean stop = false;

    while (!stop)
    {
      if (Serial.available())
      {

        char val = Serial.read();

        if (val == 'w')
        {
          stop = true;    
            motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        }
        else if (val == 's')
        {
          stop=true;    
            motors.setSpeeds(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        }
        else if (val == 'a')
        {
          stop=true;    
            motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        }
        else if (val == 'd')
        {
          stop=true;  
            motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        }
        else if (val == 'x')
        {
            stop=true;
            motors.setSpeeds(0, 0);
        }
      }
    }
    lcd.clear();
}

void modeThree()//autonomous
{
  calibrateSensors();
  boolean stop = false;
  while (!stop)
  {
    lineSensors.readLine(lineSensorValues);//This section reads the sensor (line and proximity values)
    proxSensors.read();
    leftFrontValue = proxSensors.countsFrontWithLeftLeds();
    rightFrontValue = proxSensors.countsFrontWithRightLeds();
    leftValue = proxSensors.countsLeftWithLeftLeds();
    rightValue = proxSensors.countsRightWithRightLeds();
    
    if (personFoundCooldown > 0)//personFoundCooldown was added as a condition for seeing a person to prevent the Zumo from endlessly 'finding' the same pereson
      personFoundCooldown -= 1;

    if (leftFrontValue >= FRONT_PROX_SENSOR_TOLERANCE || rightFrontValue >= FRONT_PROX_SENSOR_TOLERANCE) {//object in front
      motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
      delay(500);      
    }
    if (leftValue >= SIDE_PROX_SENSOR_TOLERANCE && personFoundCooldown == 0)//object to the left
    {
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
    if(Serial.available())
    {
      char val = Serial.read();      
      if (val == 'x')//when x is read, the mode stops
      {
        stop=true;
      }
    }
  }
}
  

void setup() {
  lcd.clear();
  lcd.gotoXY(0,0);
  Serial.begin(9600);

  if (NUM_LINE_SENSORS == 3)
    lineSensors.initThreeSensors();
  else
    lineSensors.initFiveSensors();

  proxSensors.initThreeSensors();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    char mode = Serial.read();

      if (mode=='1')
        modeOne();
      else if (mode=='2')
        modeTwo();
      else if (mode=='3')
        modeThree();

      
  }
}