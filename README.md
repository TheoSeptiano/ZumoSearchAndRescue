# ZumoSearchAndRescue

This project makes the Zumo search a white corridor with black walls for objects (representing people.)

This is achieved using 3 modes:

* Mode 1: Manual Control, wherein the user sends commands via a USB cable using the ManualGUI.pde file to move the Zumo.

* Mode 2: Semi-autonomous Control. In this mode, the Zumo moves forwards on its own until it senses an object, at which point it will output "Command" to the LCD screen attached to the Zumo and await the user to input a command manually using the GUI.

* Mode 3: Autonomous Control. In this mode, the Zumo will traverse the corridors scanning for objects without user intervention.

In any mode the LCD screen will display "Person l" or "Person r" when an object is detected to the left or right respectively.

The AllModes.ino file should be used as this file contains the 3 individual modes contained within it.

To use the GUI, open the ManualGUI.pde file in processing, and run it whilst the Zumo is plugged in. Processing may display an error "Port not found", if this occurs change the port definition (shown below) to match what is shown in arduinoIDE.
<img width="960" alt="portScreenshot" src="https://user-images.githubusercontent.com/61507843/222445827-97eeac71-1ac1-4fa4-9ffb-1d5f39284b22.png">

When using the GUI, clickable toggle buttons on-screen can be used or alternatively the w,a,s,d and x keys can be used to input forwards, backwards, left, right, and stop respectively.

## References:

[GUI Example in Processing](https://youtu.be/YrRt2V1ApKE)

[Pololu Zumo User's Guide](https://www.pololu.com/docs/0J63)
