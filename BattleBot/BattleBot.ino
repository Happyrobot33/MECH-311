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

void setup()
{
    Serial.begin(115200);

    //setup light sensor
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    lightServo.attach(SERVO_PIN);
}

const int NUM_READINGS = 40; // Number of readings for averaging
int values[NUM_READINGS];

void loop()
{
    ReadInValues();
}

void ReadInValues()
{
    // Read multiple values from the light sensor
    for (int i = 0; i < NUM_READINGS; i++)
    {
        //drive the servo to the scan position
        long rotation = map(i, 0, NUM_READINGS - 1, 0, 180);
        lightServo.write(rotation);
        delay(200);
        values[i] = analogRead(LIGHT_SENSOR_PIN);
    }

    //print out the values
    Serial.print("Light Sensor Readings:");
    for (int i = 0; i < NUM_READINGS; i++)
    {
        Serial.print(" ");
        Serial.print(values[i]);
    }
    Serial.println();
}






//util functions

// Turn by a specified ammount of degrees either positive or negative by a 0 to 100 pct
void Turn(int degrees, int speedPct)
{
// hand tuned scalar to make the degrees match with the delay
#define degreesScalar 265
    int timeDelay = (degreesScalar * abs(degrees)) * (1.0 / speedPct);
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
