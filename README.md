# Introduction:

Welcome to The Mars team’s Engineering Documentation Journal, going by the WRO (World Robot Olympiad) that is hosting a competition for people around the world to compete in with different challenges using Autonomous driving Cars.

The Palestinian Mars Team consists of 3 people, The repository includes a photo showing all team members, And Vehicle photos showing the car used in the competition.

This GitHub Repository will include the source code written by the team itself used for the Autonomous Vehicle, Photos of the team, Photos of the Vehicle from multiple directions, Videos of the car driving Autonomously in the field through both challenges, And the schemes containing a diagram showing how the components used in the vehicle are connected to each other and to the Arduino board.

The Mars Team hopes for the Engineering Documentation Journal to help other people Improve upon their projects no matter what it is, and to give them inspiration to know what to do next, and create new stuff.

# The Vehicle:

The Mars Vehicle is approximately 25cm long, 19cm wide and 24cm high, the majority of its chassis was taken from multiple disassembled RC Cars, which consisted of a car body, 2 wheels for driving (forward, backwards), 2 wheels for steering (left, right), a DC Motor and new gears for it.

The original DC Motor of the first RC car was not suitable enough for the Vehicle’s tasks, so it was replaced with a newer DC Motor that was taken from another RC Car which also had new gears alongside the DC Motor which gave it more torque and made it steadier. 

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

The Ultrasonic Sensors play a major role in open challenge, as they are used to stay in a specific range to keep the Vehicle steady on the field and not crash into any walls.

# Power Distribution Management:

The Vehicle is powered by 3 Lithium batteries connected in parallel which all output 12V (Volts) and 2A (Ampere), The batteries are connected to a PMS (Power Management System), Which manages the power coming in and out of the battery (hence the name) in a safe manner WITHOUT the battery failing, The batteries power the DC Motor and the Driver Motor controlling the DC, Outputting 12V and 2A, the batteries also power the USB Car charger, which converts the 12V to 5V (Because naturally the other components only need 5V and giving any more would make them fail) but still outputting 2A, The reason a USB Car charger was used instead of a boost because of its price and the budget was not enough to afford it, and it wasn’t easy to get ahold of it either, The USB Car Charger then powers all 4 Ultrasonic sensors, The Servo Motor, The Arduino board and the HuskyLens Camera, all of which (The Batteries and USB Car Charger) are powered on using a switch.


# The HuskyLens Camera:

The HuskyLens camera came in clutch, for it has Onboard AI processing and learning capability, which makes up for not using additional controllers/microcontrollers, and that enables it to update readings and feedback in real time based on what the camera is seeing.

This plays a major role in obstacle challenge, as the HuskyLens has built-in color recognition and object classification/recognition, which for example can help determine the route of the vehicle on the field based on the color of the traffic signs, etc…

The HuskyLens camera is installed on a tall metal piece at the front of the vehicle, the reason this was done is so that the camera does not see anything beyond the field borders, which can cause interference with the camera and can make it provide false readings, the HuskyLens camera is connected directly to the Arduino board as every other component (also shown in the diagram), Both the Ultrasonic Sensors and the HuskyLens combined ensure the vehicle does not hit any walls or traffic signs, based on the calculations and readings of the camera that it collects in real time, all of which lower the chance of any unexpected events to happen.

The HuskyLens also helps with the parallel parking in finding the borders and using its AI processing to figure out the best way to park the Vehicle IN PARALLEL without hitting the borders of the parking area.


# The C Coding and Algorithms: 

When It comes to the Algorithms this Vehicle uses, 2 different algorithms are used, and the reason for that is the difference between open challenge and obstacle challenge which did not allow for 1 algorithm to be used for the Mars vehicle's case, and because of that 2 algorithms were used.

4 Runtime libraries were used in the code, 2 of which are shared, the "Servo.h" and "Wire.h" libraries are used for both Algorithms, However, the "HUSKYLENS.h" and “SoftwareSerial.h” libraries are only used in the obstacle challenge.

The libraries have to be defined inside the code so they can be properly used, and to do that the “#include (library name)” order is used, and to define them for a specific section of the code, for example, the “HUSKYLENS huskylens;", "Servo servo;" orders are used, the first word is the library name and the second one defines what section of the library is used.

 In the Obstacle Challenge, there is another important order to power the HuskyLens on which is “#define HUSKYLENS_I2C_ADDRESS 0x32”, and without it the HuskyLens does not power on.
 
After defining the libraries, Variables are defined to be used for different tasks in the code, The main order used is the integer “int” order, for example: "int speedo = 255;", "int lap = 0;", all of which will be explained alongside the Algorithms.

Booleans (“bool” order) are also used to return 1 or 0 values (true/false, high/low, etc..), And the value can be edited inside the code itself, an example of a boolean order: "bool intdistance=true;" etc..

The “byte” function is also very important to define the pins that every component is connected to on the Arduino board.

**”void setup()” function:**

The “void setup()” function gets the Vehicle ready for the upcoming tasks when it powers on, As it has to define every electronic part on the Arduino as an OUTPUT/INPUT, LOW/HIGH, etc…

In the “void setup();” function, the “Serial.begin(115200);” order defines which speed the serial readings would be, The speed of 115200 suited the Vehicle well as it didn’t have any problems while testing the Vehicle doing said tasks.

The electronic parts are defined by using “pinMode(‘pinname’, OUTPUT/INPUT);”, for example, the “pinMode(dcmotfpin, OUTPUT);” defines the DC Motor as an OUTPUT PIN, and we also use the “digitalWrite(pinname, HIGH/LOW);” for example, the “digitalWrite(dcmotfpin,LOW);” order defines the DC Motor to be “LOW” which means it should be turned off.

The important thing that the Void setup function does is that when the Vehicle powers on, It gives an order to the ultrasonic sensors to measure the distance from all directions before the Vehicle starts moving and saves the values measured to determine where to stop after finishing the final lap, this is done by using the function named "intXdistance" where "X" is the direction of the ultrasonic sensor, so "intFdistance" F = forward, B = Backward, etc...

The ”delay(value);” variable is an order to make the code wait a specific set of time before continuing, the value in the delay variable is in MS (milliseconds) which means 1000 MS = 1 Second, This simple delay order that is used in the void setup “delay(5);” is very important for the code to make it not malfunction and give any false readings, as the code has to do multiple things like read from different ultrasonic sensors and without the delay many problems arise like getting false readings or one order being ignored, and for that the delay of 5 MS is put in between every order to avoid these problems.

The “while(true)” order that is used at the end of the void setup function checks if the Vehicle button is ON or OFF (HIGH/LOW) so it can start the “void loop()” function which makes the Vehicle be ready and begin it’s tasks.

Additionally, In the Obstacle challenge, 2 If conditions are used to check if the connection with the HuskyLens camera is working.

**The Additional Voids:**

There are multiple voids used for the code, most of them are shared between open and obstacle challenge but some are only in the obstacle challenge, these shared voids are:

•	“void readdistance(int trigPin, int echoPin, char u)” defines which ultrasonic sensor to read distance from.

•	“void readul()” orders all ultrasonics to read the distance at the same time, it is put in 1 function to save time, processing power and code lines.

•	“void straight()”, “void right()”, “void left()” these functions control the Servo motor to steer the front-wheels.

•	“void backward(int speedo)”, “void forward(int speedo)” Control the DC Motor speed and direction which changes the speed and the direction of the back wheels.

•	“void stopdriving()” turns the DC Motor off which stops the Vehicle from moving.

•	“void Right_Left_dis()” This void function will be explained at the Open Challenge Algorithm.

•	“void husky()” This function turns the camera on and specifies the position and color of the signs. (Obstacle Challenge only)

•	"void First_Pillar_Check()" This function is used before the Vehicle starts moving to save the value of the first pillar the camera either (either Red or Green), And the saved value is used at the end of the second lap to determine whether to do a U-turn on the section or not. (Obstacle Challenge Only)

•	"void turn" This function is used on the corners of the field to know if the Vehicle is moving either clockwise or counter-clockwise. (Obstacle Challenge Only)

•	"void huskyread()" Is used to create an algorithm for passing the traffic signs either left or right depending on the color of the traffic sign. (Obstacle Challenge Only)

•	"void U_Turn()" Is used to change the Vehicle's direction (clockwise/counter-clockwise) at the end of the second lap. (Obstacle Challenge Only)

•	"void Lap_8()" This function is used to do a U-Turn at the end of the second lap. (Obstacle Challenge Only)

•	"void Lap_12()" This function is used to stop the Vehicle at the starting section after completing the 3 laps. (Obstacle Challenge Only)

There are 2 different Algorithms for each Challenge, starting with:

**Open Challenge:**

The general idea of this Algorithm is using the UltraSonic Sensors as the primary components to complete the tasks from start to finish.

When the start button is pressed, the Vehicle adjusts the front Wheels controlled by the Servo to be exactly straight, then the forward order is given to move the Vehicle so it starts going forward, The Front Ultrasonic sensor starts measuring the distance between the border wall and the Vehicle, So if the distance measured is less than 100cm, The Vehicle lowers its speed and an order is given to both left and right ultrasonic sensors to read the distance, So If the distance measured from the right ultrasonic sensor is 100cm < then the Vehicle turns right, and it adds +1 to the Lap value (which counts how many times the Vehicle has turned a corner) by using the order “lap++;”, Same thing happens with the Left Ultrasonic but instead of turning right it turns left.

After the Vehicle finishes turning a corner, It may not be fully straight on the field every time, so it changes the value of “distanceCaptured” to false “distanceCaptured = false;”, and this order is used in the “Right_left_dis” function which was mentioned above.

This void function reads the distance measured from both left and right ultrasonic sensors and saves that value at a certain point of time to do mathematical operations on it, so it takes both values and adds them together and then divides the number it gets by 2, Then this value is saved in a float value called “distanceValueAtDetection”, And this all happens in a specific order which is:

“distanceValueAtDetection = ((ldistance + rdistance) /2);”, and after this all happens the distanceCaptured value is returned to true.
“distanceCaptured = true;”
Then 2 integer values are used, first one being “mindis” which equals the distanceValueAtDetection but it removes 10 from its value, and the other one being “maxdis” which is the same thing, however it adds 10 to the value rather than removing.

These mathematical calculations are all used to keep the Vehicle in a specific range of distance on the field, meaning that if the Vehicle goes beyond that range it will adjust itself to go back to that range and stay in it, which keeps it from crashing into the wall border, All of these tasks and orders keep happening in a loop until the lap count reaches a number higher than 11, so when it reaches the final Lap (12), The integer “uldiff=10” is used to create a range by using the integer values “minX/maxX” where “X” defines which ultrasonic is used, and in this case all ultrasonics are defined, “minf, maxf, maxb, etc….”, and the min/max means -10 or +10, so “minX” equals “intXdistance – uldiff” and “maxX” equals “intXdistance + uldiff”, then the “readul();” function is used to make all ultrasonic sensors on the Vehicle start measuring the distance from all directions and move to the “minX/maxX” range that was measured at the beginning, and when it reaches that specific range, the Vehicle stops driving and finishes the task.

**Obstacle Challenge:**

The main component that was used for the Algorithm was the HuskyLens Camera which has color recognition feature that was used for this Algorithm.

When the Vehicle starts, it is mostly the same as the open challenge algorithm, it adjusts the wheels and the vehicle starts going forwards, Before it starts moving, the HuskyLens camera view is split in half vertically to distinguish between the traffic signs on both sides, Then the camera begins its color recognition to check for any obstacles ahead of it, and if it doesn’t detect any obstacles it will start moving forward, however if it detects an obstacle ahead of it, it checks where and what it’s color is, so for example if it detects a green sign on the right or a red sign on the left it will go forward, but if it detects a red sign on the right, it will pass through the sign from its right, and if it detects a green sign on the left it will pass through the sign from its left, then after passing the signs it will adjust itself to be straight on the field, and keep moving normally like it would in open challenge while doing the same lap counter after turning a corner and avoiding the obstacles while doing so.

When the lap count reaches == 8, The Vehicle continues in the section until it sees the last sign color, if it’s green the Vehicle continues normally and stops after lap 12 as said in the open challenge, however if the color of the sign is red it will do U-turn around the red sign to change the vehicle’s direction and then continue the third lap until the lap count reaches 13.

(Note: all of this is explained in detail inside the source code)

# Conclusion

This readme may have been long but it explains almost everything about the Mars Vehicle, how it behaves depending on specific circumstances, The way everything is connected, The Vehicle’s components, their role in the Vehicle and the coding algorithm the Vehicle uses, The Mars Team hopes this can be of use to future teams trying to compete or doing their own projects no matter what they are.
