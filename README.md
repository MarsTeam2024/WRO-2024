# Introduction:

Welcome to The Mars team’s Engineering Documentation Journal, going by the WRO (World Robot Olympiad) that is hosting a competition for people around the world to compete in with different challenges using Autonomous driving Cars.
The Palestinian Mars Team consists of 3 people, The repository includes a photo showing all team members, And Vehicle photos showing the car used in the competition.
This Github Repository will include the source code written by the team itself used for the Autonomous Vehicle, Photos of the team, Photos of the Vehicle from multiple directions, 3D Print models, Videos of the car driving Autonomously in the field through both challenges, And the schemes containing maps of how the components used in the vehicle are connected to each other and to the Arduino board.
The Mars Team hopes for the Engineering Documentation Journal to help other people Improve upon their projects no matter what it is, and to give them inspiration to know what to do next, and create new stuff.

# The Vehicle:

The Mars Vehicle is approximately 23cm tall, 15cm wide and 30cm high, the majority of its chassis was taken from a disassembled toy car, which consisted of a car body, 2 wheels for driving (forward, backwards), 2 wheels for steering (left, right).
The original DC Motor of the toy car was not suitable enough for the Vehicle’s tasks, so it was replaced with a newer DC Motor that was taken from a disassembled printer, and gears were added to the DC Motor to give it more torque, and alongside the DC Motor is an encoder installed on it, which is used to count the amount of times the DC Motor has turned, which can help determine the speed of the vehicle.
Similar to the DC Motor, the original motor for the steering wheels was also not good enough for the task, so it was changed for a newer Servo Motor with higher precision and position readings.
The main controller used for the Mars Vehicle is an Arduino Mega 2560 that controls all the components used for the vehicle, The reason no additional controllers or microcontrollers were used for the Vehicle is because of the Vehicle’s HuskyLens Camera that has AI real time tracking which basically makes up for not using additional controllers/microcontrollers, the HuskyLens camera will be explained later down the line.
The components that were used for the Vehicle are the following:
•	4 Ultrasonic Sensors.
•	1 DC Motor, 1 Servo Motor.
•	1 HuskyLens AI Tracking Camera.
•	1 L298N Driver Motor.
•	3 Lithium Batteries and a PMS (Power Management System).
•	A USB Car Charger.

# The Mobility Management System:

The DC Motor, controlled by an L298N Driver Motor, is responsible for controlling the Vehicle’s back wheels to make them go either Forwards or Backwards, The DC Motor’s high rotational speed suited the Vehicle well for back-wheel driving, and also gave the vehicle enough speed for completing said tasks.
On the other hand, The Servo Motor (Not controlled by any drivers) Is responsible for controlling the Vehicle’s front wheels for steering them, the reason it was used instead of another DC Motor was because of its good accuracy at providing position feedback, unlike the DC which has lack of position control.

# The Sensors:

The Ultrasonic Sensors are divided in different places in different directions of the Vehicle (Front, Back, Left, Right).
The Ultrasonic Sensors read the distance from all directions of the car which is (for example) used to read the distance from the sensors at the start position of the Vehicle to determine where its original starting position is.
The Ultrasonic Sensors play a major role in open challenge, as they can be used to move away from the border walls when getting too close to them.

# Power Distribution Management:

The Vehicle is powered by 3 Lithium batteries connected in parallel which all output 12V (Volts) and 2A (Ampere), The batteries are connected to a PMS (Power Management System), Which manages the power coming in and out of the battery (hence the name) in a safe manner WITHOUT the battery failing, The batteries power the DC Motor and the Driver Motor controlling the DC, Outputting 12V and 2A, the batteries also power the USB Car charger, which converts the 12V to 5V (Because naturally the other components only need 5V and giving any more would make them fail) but still outputting 2A, The USB Charger then powers all 4 Ultrasonic sensors, The Servo Motor, The Arduino board and the HuskyLens Camera, all of which (The Batteries and USB Car Charger) are powered on using a switch.


# The HuskyLens Camera:

The HuskyLens camera came in clutch, for it has Onboard AI processing and learning capability, which makes up for not using additional controllers/microcontrollers, and that enables it to update readings and feedback in real time based on what the camera is seeing.
This plays a major role in obstacle challenge, as the HuskyLens has built-in color recognition and object classification/recognition, which for example can help determine the route of the vehicle on the field based on the color of the traffic signs, etc….
The HuskyLens camera is installed on a tall metal piece at the back of the vehicle, the reason this was done is so that the camera does not see anything beyond the field borders, which can cause interference with the camera and can make it provide false readings, the HuskyLens camera is connected directly to the Arduino board as every other component (also shown in the diagram), Both the Ultrasonic Sesnors and the HuskyLens combined ensure the vehicle does not hit any walls or traffic signs, based on the calculations and readings of the camera that it collects in real time, all of which lower the chance of any unexpected events to happen.
The HuskyLens also helps with the parallel parking in finding the borders and using its AI processing to figure out the best way to park the Vehicle IN PARALLEL without hitting the borders of the parking area.


# The C Coding and Algorithms:

The Readme is yet to be complete


