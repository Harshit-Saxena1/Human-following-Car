#include <NewPing.h>           
#include <Servo.h>             
#include <AFMotor.h>           

#define RIGHT A2              
#define LEFT A3               
#define TRIGGER_PIN A1        
#define ECHO_PIN A0           
#define MAX_DISTANCE 200      

unsigned int distance = 0;    
unsigned int Right_Value = 0; 
unsigned int Left_Value = 0;  

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  

// Create motor objects
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

Servo myservo; // Create servo object
int pos = 0;   // Variable to store the servo position

void setup() {
    Serial.begin(9600); 
    myservo.attach(10); // Attach servo to pin 10
    
    // Sweep servo from 90 to 0 to 180 degrees
    for (pos = 90; pos <= 180; pos += 1) {
        myservo.write(pos);
        delay(15);
    }
    for (pos = 180; pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(15);
    }
    for (pos = 0; pos <= 90; pos += 1) {
        myservo.write(pos);
        delay(15);
    }

    pinMode(RIGHT, INPUT);
    pinMode(LEFT, INPUT);
}

void loop() {                             
    delay(50); // Wait between pings
    distance = sonar.ping_cm(); // Get distance in cm
    Serial.print("Distance: ");
    Serial.println(distance); 

    Right_Value = digitalRead(RIGHT); // Read Right IR sensor
    Left_Value = digitalRead(LEFT); // Read Left IR sensor
    Serial.print("Right: ");
    Serial.println(Right_Value); 
    Serial.print("Left: ");
    Serial.println(Left_Value); 

    if (Left_Value == 1 && distance >= 0 && distance <= 20 && Right_Value == 1) {
        moveForward();
    } else if (Right_Value == 0 && Left_Value == 1) {
        turnLeft();
    } else if (Right_Value == 1 && Left_Value == 0) {
        turnRight();
    } else if (distance > 15) {
        stopMotors();
    }
}

void moveForward() {
    setMotorSpeed(150, FORWARD);
}

void turnLeft() {
    Motor1.setSpeed(150);
    Motor1.run(FORWARD);
    Motor2.setSpeed(0);
    Motor2.run(FORWARD);
    Motor3.setSpeed(0);
    Motor3.run(FORWARD);
    Motor4.setSpeed(170);
    Motor4.run(FORWARD);
    delay(150);
}

void turnRight() {
    Motor1.setSpeed(0);
    Motor1.run(FORWARD);
    Motor2.setSpeed(150);
    Motor2.run(FORWARD);
    Motor3.setSpeed(170);
    Motor3.run(FORWARD);
    Motor4.setSpeed(0);
    Motor4.run(FORWARD);
    delay(150);
}

void stopMotors() {
    setMotorSpeed(0, RELEASE);
}

void setMotorSpeed(int speed, int direction) {
    Motor1.setSpeed(speed);
    Motor1.run(direction);
    Motor2.setSpeed(speed);
    Motor2.run(direction);
    Motor3.setSpeed(speed);
    Motor3.run(direction);
    Motor4.setSpeed(speed);
    Motor4.run(direction);
}
