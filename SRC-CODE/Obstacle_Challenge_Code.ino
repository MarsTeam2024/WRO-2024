#include <Wire.h>
#include "HUSKYLENS.h"
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

bool intcamera = true;
bool empty = false;


// تعيين عنوان I2C لـ HuskyLens
#define HUSKYLENS_I2C_ADDRESS 0x32         // تحقق من دليل HuskyLens للحصول على العنوان الصحيح

HUSKYLENS huskylens;
HUSKYLENSResult result;                    // Declare result globally

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

  // بدء الاتصال مع HuskyLens عبر I2C
  if (!huskylens.begin(Wire)) 
  {
    Serial.println("HuskyLens not connected!");
  } else 
  {
    Serial.println("HuskyLens connected!");
    // اضبط HuskyLens على وضع التعرف على الألوان
    huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
  }
}

void loop() 
{
  while(true)
  {
    straight();
    First_Pillar_Check();
    huskey();
    huskeyread();
    turn();
    Lap_8();
    Lap_12();
  }
}

void Lap_12()
{
  if(lap>11)
  {
    huskeyread();
    while(true)
    {
      delay(300);
      stopdriving();
      //dealy(100000000000);
    }
  }
}

void Lap_8()
{
  if(lap == 8)
  {
    huskeyread();
    if(!empty)
    {
      if(!intcamera)
      {
        huskeyread();
        U_Turn();
        huskeyread();
      }
      if(intcamera)
      {
        huskeyread();
      }
    }
    if(empty)
    {
      huskey();
      if(result.ID == 1)
      {
        intcamera = false;
        huskeyread();
        U_Turn();
        huskeyread();
      }
      else if(result.ID == 2)
      {
        intcamera = true;
        huskeyread();
      }
    }
  }
}

void U_Turn()
{
  left();
  delay(1000);
  stopdriving();
  delay(100);
  straight();
  backward(40);
  delay(500);
  stopdriving();
  delay(100);
  forward(40);
  left();
  delay(1000);
  stopdriving();
  delay(100);
  backward(40);
  delay(300);
  stopdriving();
  delay(100);
  forward(40);
}

void turn()
{
  forward(40);
  readdistance(ftrig, fecho, 'F');     // Read forward distance
  if (fdistance < 120)
  {
    forward(40);                       // Adjust speed if obstacle detected
    readdistance(ftrig, fecho, 'F');      
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
      }
      if(ldistance > 100)
      {
        left();              // Turn left if obstacle detected
        delay(1500);         // Delay to complete turn
        lap++;               // Increment lap count
        Serial.print("lap = "); 
        Serial.println(lap);
        straight();          // Set servo to straight position
      }
    }
  }  
}

void First_Pillar_Check()
{
  huskey();
  if((lap == 0) && (result.ID == 1))
  {
    intcamera = false;
  }
  else if((lap == 0) && (result.ID == 2))
  {
    intcamera = true;
  }
  else
  {
    empty = true;
  }
}

void huskeyread()
{
  huskey();
  if(result.xCenter<50)
  {
    if(result.ID == 1)
    {
      forward(40);
      right();
      delay(300);
      left();
      delay(300);
      straight();
    }
  }
  if(result.xCenter>265)
  {
    if(result.ID == 2)
    {
      forward(40);
      left();
      delay(300);
      right();
      delay(300);
      straight();
    }
  }
  if(result.xCenter>157.5)
  {
    if(result.ID == 1)
    {
      forward(40);
      right();
      readdistance(ftrig, fecho, 'F');
      if(fdistance<16)
      {
        stopdriving();
        straight();
        backward(40);
        readdistance(ftrig, fecho, 'F');
        if(fdistance>15)
        {
          stopdriving();
          forward(40);
          right();
          huskey();
          if(result.xCenter<50)       
          {
            if(result.ID == 1)
            {
              straight();
              readdistance(ltrig, lecho, 'L');    
              if(ldistance<20)
              {
                straight();
                delay(100);
                left();
                readdistance(ltrig, lecho, 'L'); 
                if(ldistance>20)
                {
                  straight();
                }
              }
            }
          }
        }
      }
      else if(fdistance>16)
      {
        huskey();
        if(result.xCenter<50)       
        {
          if(result.ID == 1)
          {
            straight();
            readdistance(ltrig, lecho, 'L');     
            if(ldistance<20)
            {
              straight();
              delay(100);
              left();
              readdistance(ltrig, lecho, 'L'); 
              if(ldistance>20)
              {
                straight();
              }
            }
          }
        }
      }
    }
  }
  if(result.xCenter<157.5)
  {
    if(result.ID == 2)
    {
      forward(40);
      left();
      readdistance(ftrig, fecho, 'F'); 
      if(fdistance<16)
      {
        stopdriving();
        straight();
        backward(40);
        readdistance(ftrig, fecho, 'F'); 
        if(fdistance>15)
        {
          stopdriving();
          forward(40);
          left();
          huskey();
          if(result.xCenter>265)       
          {
            if(result.ID == 2)
            {
              straight();
              readdistance(rtrig, recho, 'R');     
              if(rdistance<20)
              {
                straight();
                delay(100);
                right();
                readdistance(rtrig, recho, 'R');
                if(rdistance>20)
                {
                  straight();
                }
              }
            }
          }
        }
      }
      else if(fdistance>16)
      {
        huskey();
        if(result.xCenter>265)       
        {
          if(result.ID == 2)
          {
            straight();
            readdistance(rtrig, recho, 'R');    
            if(rdistance<20)
            {
              straight();
              delay(100);
              left();
              readdistance(rtrig, recho, 'R');
              if(rdistance>20)
              {
                straight();
              }
            }
          }
        }
      }
    }
  }
}

void huskey()
{
  if (huskylens.request()) 
  {
    Serial.println("HuskyLens request successful!");
    
    int numObjects = huskylens.available();
    
    for (int i = 0; i < numObjects; i++) 
    {
      result = huskylens.read(); // Assign to global variable 'result'
      if (result.command == COMMAND_RETURN_BLOCK) 
      {
        Serial.print("Color ID: ");
        Serial.println(result.ID);
        Serial.print("Position: ");
        Serial.print("X = ");
        Serial.print(result.xCenter);
        Serial.print(", Y = ");
        Serial.print(result.yCenter);
        Serial.println();
      }
    }
  } else 
  {
    Serial.println("HuskyLens request failed!");
  }
  delay(100); 
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