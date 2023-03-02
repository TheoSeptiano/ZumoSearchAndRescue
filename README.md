# ZumoSearchAndRescue

This project makes the Zumo search a white corridor with black walls for objects (representing people.)

This is achieved using 3 modes:

* Mode 1: Manual Control, wherein the user sends commands via a USB cable using the ManualGUI.pde file to move the Zumo.

* Mode 2: Semi-autonomous Control. In this mode, the Zumo moves forwards on its own until it senses an object, at which point it will output "Command" to the LCD screen attached to the Zumo and await the user to input a command manually using the GUI.

* Mode 3: Autonomous Control. In this mode, the Zumo will traverse the corridors scanning for objects without user intervention.

In any mode the LCD screen will display "Person l" or "Person r" when an object is detected to the left or right respectively.

The AllModes.ino file should be used as this file contains the 3 individual modes contained within it.

When using the GUI, clickable toggle buttons on-screen can be used or alternatively the w,a,s,d and x keys can be used to input forwards, backwards, left, right, and stop respectively.

References:

[GUI Example in Processing](https://youtu.be/YrRt2V1ApKE)

[Pololu Zumo User's Guide](https://www.pololu.com/docs/0J63)
