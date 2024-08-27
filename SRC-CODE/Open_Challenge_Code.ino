#include "Wire.h"
#include <Servo.h>

// Define ultrasonic sensor pins
#define fecho 2                            // input echo pin for FORWARD ultrasonic
#define ftrig 3                            // output trig pin for FORWARD ultrasonic

#define recho 10                           // input echo pin for RIGHT ultrasonic
#define rtrig 11                           // output trig pin for RIGHT ultrasonic

#define lecho 8                            // input echo pin for LEFT ultrasonic
#define ltrig 9                            // output trig pin for LEFT ultrasonic

#define becho 28                           // input echo pin for BACK ultrasonic
#define btrig 26                           // output trig pin for BACK ultrasonic

// DC motor parameters and pins
byte dcmotfpin = 5;                     // output pin for FORWARD DC driving motor 
byte dcmotbpin = 4;                     // output pin for BACKWARD DC driving motor 
int speedo=255;                         // speed for driving dc motor

// Stearing motor parameters and pins           
Servo servo;                           // Create a Servo object for steering
int pos = 90 ;                         // Initial angle position of the servo (straight)
byte servopin = 22;                    // Output PMW pin for servo stearing motor

// Start button parameters and pins
byte startpin = 37;                      // Input pin for start push button               
byte startsignal;                        // Variable to store the button state (HIGH or LOW)

// Indicator LED pin
byte ledpin = 11;                        // Pin for LED indicator

// Variables for ultrasonic sensor readings
unsigned int ADCValue;

int fdistance,rdistance,ldistance,bdistance;                    //  Distances from ultrasonic sensors // Xdistance, X=FRONT/BACK/RIGHT/LEFT ultrasonic readings
int intfdistance,intrdistance,intldistance,intbdistance;        //  Initial distances from ultrasonic sensors // intXdistance, X=FRONT/BACK/RIGHT/LEFT ultrasonic readings
bool intdistance = true;                                        // Flag to check if distance values are captured
bool intdis = true;
long duration;

bool restartLoop=true;
int lap = 0;

// Variables for lap tracking and distance ranges
int uldiff=10;                           // Difference to create a range for stopping at the starting point

// Range values for sensor distances
int minf=intfdistance-uldiff;
int maxf=intfdistance+uldiff;
int minr=intrdistance-uldiff;
int maxr=intrdistance+uldiff;
int minl=intldistance-uldiff;
int maxl=intldistance+uldiff;
int minb=intbdistance-uldiff;
int maxb=intbdistance+uldiff;

// Creating a mathmatical calculation range to keep the car at the middle or the rode
bool distanceCaptured = false;              // Flag to ensure the distance value is captured only once
float distanceValueAtDetection;             // Variable to store the distance value at the moment of detection

// Range for distance calculations
int mindis=distanceValueAtDetection-10;
int maxdis=distanceValueAtDetection+10;

void setup() 
{
  Serial.begin(115200);                      // Initialize serial communication
  Wire.begin();                              // Initialize I2C communication
  delay(10);

  // Initialize DC motor pins
  pinMode(dcmotfpin, OUTPUT);
  pinMode(dcmotbpin, OUTPUT);
  digitalWrite(dcmotfpin,LOW);
  digitalWrite(dcmotbpin,LOW);

  // Initialize servo motor
  pinMode(servopin, OUTPUT);
  pinMode(ledpin, OUTPUT);
  servo.attach(servopin);                     // Attach servo to pin
  servo.write(pos);                           // Set initial servo position

  // Initialize ultrasonic sensor pins
  pinMode(ftrig, OUTPUT); 
  pinMode(fecho, INPUT);
  pinMode(rtrig, OUTPUT); 
  pinMode(recho, INPUT);
  pinMode(ltrig, OUTPUT); 
  pinMode(lecho, INPUT);
  pinMode(btrig, OUTPUT); 
  pinMode(becho, INPUT);

  // Read initial distances from sensors
  intdistance = true;
  readdistance(ftrig, fecho, 'F');
  delay(5);
  readdistance(rtrig, recho, 'R');
  delay(5);
  readdistance(ltrig, lecho, 'L');
  delay(5);
  readdistance(btrig, becho, 'B');
  delay(5);

  // Print initial distances to serial monitor
  Serial.print("intfdis= ");
  Serial.println(intfdistance);
  delay(5);
  Serial.print("intrdis= ");
  Serial.println(intrdistance);
  delay(5);
  Serial.print("intldis= ");
  Serial.println(intldistance);
  delay(5);
  Serial.print("intbdis= ");
  Serial.println(intbdistance);
  
  // Turn on LED to indicate readiness
  digitalWrite(ledpin,HIGH);

  // Wait for start button press to begin
  while(true)
  {
    startsignal=digitalRead(startpin);
    if(startsignal == HIGH)
    {
      digitalWrite(ledpin,LOW);
      break;
    }
  }
}

void loop() 
{
  while(true)
  {
    restartLoop = true;
    if(restartLoop)
    {
      restartLoop = false;
      straight();           // Set servo to straight position
      forward(100);         // Move forward at speed 100
      
      while(true)
      {
        readdistance(ftrig, fecho, 'F');     // Read forward distance
        if (fdistance < 120)
        {
          readdistance(ftrig, fecho, 'F');
          forward(70);                       // Adjust speed if obstacle detected
          
          if ((fdistance < 100 && fdistance > 20))
          {
            readdistance(rtrig, recho, 'R'); // Read right distance
            delay(5);
            readdistance(ltrig, lecho, 'L'); // Read left distance
            // Serial.println(rdistance);
            // Serial.println(ldistance);

            if (rdistance > 100)
            {
              right();               // Turn right
              delay(1500);           // Delay to complete turn
              lap++;                 // Increment lap count
              Serial.print("lap = "); 
              Serial.println(lap);
              straight();            // Set servo to straight position
              distanceCaptured = false;
              Right_Left_dis();      // Capture distance between left and right sensors
              
              while(true)
              {
                if((rdistance > maxdis) || (ldistance < mindis))
                {
                  right();         // Turn right if needed
                }
                if((rdistance < mindis) || (ldistance > maxdis))
                {
                  left();          // Turn left if needed
                }
                if(((rdistance > mindis) && (rdistance < maxdis)) || ((ldistance < maxdis) && (ldistance > mindis)))
                {
                  straight();      // Move straight if within range
                }
                else
                {
                  break;           // Exit loop if conditions are met
                }
              }
              
              // Check if lap count exceeds 11
              if(lap > 11)
              {
                while(true)
                {
                  straight();        // Keep moving straight
                  delay(10);
                  //forward(80);     // Optional forward movement
                  readul();          // Read distances from all sensors
                  
                  // Check if all distances are within range
                  if((minf < intfdistance && intfdistance < maxf) && 
                     (minr < intrdistance && intrdistance < maxr) && 
                     (minl < intldistance && intldistance < maxl) && 
                     (minb < intbdistance && intbdistance < maxb))
                  {
                    while(true)
                    {
                      delay(100);
                      stopdriving(); // Stop driving
                      Serial.println("Mission complete !"); // Print completion message
                    }
                  }
                }
              }
            }
            else if(ldistance > 100)
            {
              left();              // Turn left if obstacle detected
              delay(1500);         // Delay to complete turn
              lap++;               // Increment lap count
              Serial.print("lap = "); 
              Serial.println(lap);
              straight();          // Set servo to straight position
              distanceCaptured = false;
              Right_Left_dis();    // Capture distance between left and right sensors
              
              while(true)
              {
                if((rdistance > maxdis) || (ldistance < mindis))
                {
                  right();       // Turn right if needed
                }
                if((rdistance < mindis) || (ldistance > maxdis))
                {
                  left();        // Turn left if needed
                }
                if(((rdistance > mindis) && (rdistance < maxdis)) || ((ldistance < maxdis) && (ldistance > mindis)))
                {
                  straight();    // Move straight if within range
                }
                else
                {
                  break;         // Exit loop if conditions are met
                }
              }
              
              // Check if lap count exceeds 11
              if(lap > 11)
              {
                while(true)
                {
                  straight();         // Keep moving straight
                  delay(10);
                  //forward(80);      // Optional forward movement
                  readul();           // Read distances from all sensors
                  
                  // Check if all distances are within range
                  if((minf < intfdistance && intfdistance < maxf) && 
                     (minr < intrdistance && intrdistance < maxr) && 
                     (minl < intldistance && intldistance < maxl) && 
                     (minb < intbdistance && intbdistance < maxb))
                  {
                    while(true)
                    {
                      delay(100);
                      stopdriving(); // Stop driving
                      Serial.println("Mission complete !"); // Print completion message
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else
    {
      break; // Exit outer loop
    }
  }
}

// Capture distance between left and right sensors
void Right_Left_dis()
{
  if(intdis == true)
  {
    readdistance(rtrig, recho, 'R');                          // Read right distance
    delay(5);
    readdistance(ltrig, lecho, 'L');                          // Read left distance
    distanceValueAtDetection = ((ldistance + rdistance)/2) ;  // Average distance between left and right
    Serial.print(distanceValueAtDetection);                   // Print distance value for debugging
    distanceCaptured = true;                                  // Set flag to indicate distance has been captured
  }
}

// Function to read distance from ultrasonic sensors
void readdistance(int trigPin, int echoPin, char u)
{
  digitalWrite(trigPin, LOW);        // Ensure trigPin is LOW
  delayMicroseconds(2);              // Small delay
  digitalWrite(trigPin, HIGH);       // Trigger the ultrasonic pulse
  delayMicroseconds(10);             // Pulse duration
  digitalWrite(trigPin, LOW);        // End of pulse

  duration = pulseIn(echoPin, HIGH); // Measure the duration of the echo pulse

  // Calculate distance based on duration and speed of sound
  if(u=='F')
  {
    fdistance = duration * 0.034 / 2;         // Calculate forward distance
    if(intdistance == true)
    {
      intfdistance = duration * 0.034 / 2;    // Store initial forward distance
     // intdistance = false;
    }
  }
  if(u=='R')
  {
    rdistance = duration * 0.034 / 2 ;        // Calculate right distance
    if(intdistance == true)
    {
      intrdistance = duration * 0.034 / 2;    // Store initial right distance
     // intdistance = false;
    }
  }
  if(u=='L')
  {
    ldistance = duration * 0.034 / 2;         // Calculate left distance
    if( intdistance == true)
    {
      intldistance = duration * 0.034 / 2;    // Store initial left distance
      //intdistance = false;
    }
  }
  if(u=='B')
  {
    bdistance = duration * 0.034 / 2;         // Calculate back distance
    if( intdistance == true)
    {
      intbdistance = duration * 0.034 / 2;    // Store initial back distance
      //intdistance = false;
    }
  }
}

// Read distances from all ultrasonic sensors
void readul()
{
  readdistance(ftrig, fecho, 'F'); // Read forward distance
  delay(10);                       // Short delay
  readdistance(rtrig, recho, 'R'); // Read right distance
  delay(10);                       // Short delay
  readdistance(ltrig, lecho, 'L'); // Read left distance
  delay(10);                       // Short delay
  readdistance(btrig, becho, 'B'); // Read back distance
}

// Set servo to straight position
void straight()
{
  pos = 90;         // Center position for the servo
  servo.write(pos); // Command the servo to the center position
}

// Turn servo right
void right()
{
  pos = 25;         // Position to turn right
  servo.write(pos); // Command the servo to turn right
}

// Turn servo left
void left()
{
  pos = 155;        // Position to turn left
  servo.write(pos); // Command the servo to turn left  
}

// Stop the DC motors
void stopdriving()
{
  digitalWrite(dcmotfpin, LOW); // Stop forward motor
  digitalWrite(dcmotbpin, LOW); // Stop backward motor
}

// Move the robot backward
void backward(int speedo)
{
  analogWrite(dcmotbpin, speedo); // Set speed for backward motion
  digitalWrite(dcmotfpin, LOW);   // Ensure forward motor is off
}

// Move the robot forward
void forward(int speedo)
{
  analogWrite(dcmotfpin, speedo); // Set speed for forward motion
  digitalWrite(dcmotbpin, LOW);   // Ensure backward motor is off
}