import controlP5.*;
import processing.serial.*;

int buttonWidth, buttonHeight;

boolean largerUI = true;

Serial port;
String val = "Waiting for input";
Textlabel serialReaderLabel;

ControlP5 ui;



void setup()
{
  size(600,450);
  
  if (largerUI)
{
  buttonWidth = 200;
  buttonHeight = 100;
}
else//M2 needs fixing in smallUI mode
{
  buttonWidth = 100;
  buttonHeight = 50;
}

  port = new Serial(this, "COM11", 9600);
  
  ui = new ControlP5(this);
  
  ui.addButton("Forward").setPosition(buttonWidth, buttonHeight).setSize(buttonWidth, buttonHeight);//buttons position relative to their size, this maintains ui scale
  ui.addButton("Back").setPosition(buttonWidth, (2*buttonHeight)+4).setSize(buttonWidth, buttonHeight);
  ui.addButton("Stop").setPosition((buttonWidth/2)-4, (3*buttonHeight)+8).setSize((buttonWidth*2)+8, (buttonHeight/2));
  ui.addButton("Left").setPosition((buttonWidth/2)-4, buttonHeight).setSize(buttonWidth/2, (2*buttonHeight)+4);//plus 4 for white gap
  ui.addButton("Right").setPosition((2*buttonWidth)+4, buttonHeight).setSize(buttonWidth/2, (2*buttonHeight)+4);
  ui.addButton("M1").setPosition((buttonWidth/2)-4, 8).setSize((buttonWidth/2)+34, buttonHeight-12);
  ui.addButton("M2").setPosition((buttonWidth+34), 8).setSize((buttonWidth/2)+34, buttonHeight-12);
  ui.addButton("M3").setPosition((buttonWidth*2)-28, 8).setSize((buttonWidth/2)+32, buttonHeight-12);
  
  serialReaderLabel = ui.addTextlabel("serialReaderLabel")
    .setPosition(94, buttonHeight*3.62)
      .setSize(408, buttonHeight/2)
        .setColorValue(0)
          .setFont(createFont("arial", 20))
              .setText(val);
}



void draw()
{
  background(255, 255, 255);
  
  if (port.available() > 0)
  {
    val = port.readStringUntil('\n');
  }
}
  

void keyPressed()
{
  if (keyPressed)
  {
    switch (key)
    {
      case 'w':
      Forward();
      break;
      
      case 's':
      Back();
      break;
      
      case 'a':
      Left();
      break;
      
      case 'd':
      Right();
      break;
      
      case 'x':
      Stop();
      break;
      
      case '1':
      M1();
      break;
      
      case '2':
      M2();
      break;
      
      case '3':
      M3();
      break;
    }
  }
}
            

void Forward()
{
  port.write('w');
}

void Back()
{
  port.write('s');
}

void Left()
{
  port.write('a');
}

void Right()
{
  port.write('d');
}

void Stop()
{
  port.write('x');
}

void M1()
{
  port.write('1');
  
  serialReaderLabel.setText("Mode 1");
}

void M2()
{
  port.write('2');
  
  serialReaderLabel.setText("Mode 2");
}

void M3()
{
  port.write('3');
  
  serialReaderLabel.setText("Mode 3");
}
