#include <NewPing.h>
#include <SparkFun_TB6612.h>
#include <Servo.h>

const int LEFT_MOTOR_IN1 = 4;
const int LEFT_MOTOR_IN2 = 5;
const int LEFT_MOTOR_PWM = 3;
const int LEFT_MOTOR_OFFSET = 1; // lets me flip direction if I wired wrong

const int RIGHT_MOTOR_IN1 = 7;
const int RIGHT_MOTOR_IN2 = 8;
const int RIGHT_MOTOR_PWM = 6;
const int RIGHT_MOTOR_OFFSET = 1; // lets me flip direction if I wired wrong

const int STBY = 9;

Motor leftMotor(LEFT_MOTOR_IN1, LEFT_MOTOR_IN2, LEFT_MOTOR_PWM, LEFT_MOTOR_OFFSET, STBY);
Motor rightMotor(RIGHT_MOTOR_IN1, RIGHT_MOTOR_IN2, RIGHT_MOTOR_PWM, RIGHT_MOTOR_OFFSET, STBY);

const int LIGHT_SENSOR_PIN = A0; // Analog pin for light sensor

const int SERVO_PIN = 10;
Servo lightServo;

const int TRG_PIN = 12;
const int ECHO_PIN = 13;
NewPing sonar(TRG_PIN, ECHO_PIN, 200);

const int LINE_SENSOR_PIN = A1;

const int BALLOON_PIN = 2;

const char fills[] = {' ', '_', '#', 'A'}; // Characters for visualizing light sensor values

void setup()
{
    Serial.begin(115200);

    //setup light sensor
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(BALLOON_PIN, INPUT);

    lightServo.attach(SERVO_PIN);

    lightServo.write(180);
}

const int NUM_READINGS = 40; // Number of readings for averaging
int values[NUM_READINGS];

void loop()
{
    int val = analogRead(LIGHT_SENSOR_PIN);
    Serial.println(val);
    return;
    //line sensor stuff
    //drive forward constantly
    Forward(100);
    //check line sensor
    const int LINE_THRESHOLD = 870;
    int value = analogRead(LINE_SENSOR_PIN);
    Serial.println(value);
    if (value > LINE_THRESHOLD)
    {
        //we are over the line, back up and turn
        Backwards(100);
        Serial.println("Line detected, backing up");
        delay(300);
        Turn(180, 100);
    }

/*     int distance = sonar.ping_cm();

    if (distance == 0)
    {
        distance = 200; // If no echo, assume max distance
    }

    //clamp
    if (distance > 30)
    {
        distance = 30;
    }
    distance = map(distance, 0, 30, -255, 255);
    Drive(distance);
    Serial.println(distance); */

    //delay(100);
    //ReadInValues();
}

void ReadInValues()
{
    //Turn(90, 255); // Initial turn to look for light
    //Turn(-90, 255);
    //return;

    // Read multiple values from the light sensor
    for (int i = 0; i < NUM_READINGS; i++)
    {
        //drive the servo to the scan position
        long rotation = map(i, 0, NUM_READINGS - 1, 180, 0);
        lightServo.write(rotation);
        delay(50);
        values[i] = analogRead(LIGHT_SENSOR_PIN);
    }
    lightServo.write(180);

    //find the min and max values
    int minValue;
    int maxValue;
    int maxValueAngle;
    for (int i = 0; i < NUM_READINGS; i++)
    {
        if (i == 0 || values[i] < minValue)
        {
            minValue = values[i];
        }
        if (i == 0 || values[i] > maxValue)
        {
            maxValue = values[i];
            maxValueAngle = map(i, 0, NUM_READINGS - 1, 180, 0);
        }
    }

    //print out the values
    Serial.print("Light Sensor Readings:");
    Serial.println();

    //remap the values to a range of 0 to 4
    for (int i = 0; i < NUM_READINGS; i++)
    {
        // Normalize the value to a range of 0 to 4
        values[i] = map(values[i], minValue, maxValue, 0, 3);
        // Print the visual representation using fills
        Serial.print(fills[values[i]]);
    }
    Serial.println();

    //print indicators for left, right, and center
    for (int i = 0; i < NUM_READINGS; i++)
    {
        if (i == 0)
        {
            Serial.print("L");
        }
        else if (i == NUM_READINGS - 1)
        {
            Serial.print("R");
        }
        else if (i == NUM_READINGS / 2)
        {
            Serial.print("C");
        }
        else
        {
            Serial.print(" ");
        }
    }

    Serial.println();

    //remap the max angle to a turn angle
    int turnAngle = map(maxValueAngle, 0, 180, 90, -90);
    Turn(turnAngle, 100);

    //drive towards the light
    Drive(120);
    delay(500);
    Stop();
}






//util functions

// Turn by a specified ammount of degrees either positive or negative by a 0 to 100 pct
void Turn(int degrees, int speedPct)
{
// hand tuned scalar to make the degrees match with the delay
#define degreesScalar 800
    int timeDelay = (long(degreesScalar) * long(abs(degrees))) * (1.0 / speedPct);
    if (degrees > 0)
    {
        Right(speedPct);
    }
    else
    {
        Left(speedPct);
    }
    delay(timeDelay);
    Stop();
}

void Drive(int speedPct)
{
    if (speedPct > 0)
    {
        Forward(speedPct);
    }
    else
    {
        Backwards(abs(speedPct));
    }
}

// General movement commands with a percentage
void Forward(int speedPct)
{
    rightMotor.drive(speedPct);
    leftMotor.drive(speedPct);
}

void Backwards(int speedPct)
{
    rightMotor.drive(-speedPct);
    leftMotor.drive(-speedPct);
}

void Right(int speedPct)
{
    rightMotor.drive(-speedPct);
    leftMotor.drive(speedPct);
}

void Left(int speedPct)
{
    rightMotor.drive(speedPct);
    leftMotor.drive(-speedPct);
}

// Drives the robot forward with a differential between both wheels
// Positive is right, negative is left
void DifferentialForward(int speedPct, int ratio)
{
    rightMotor.drive(speedPct - ratio);
    leftMotor.drive(speedPct + ratio);
}

// stop both motors, with a delay afterwards to prevent drifting
void Stop()
{
    rightMotor.brake();
    leftMotor.brake();
    delay(500); // wait half a second to avoid drifting
}
