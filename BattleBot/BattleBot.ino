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

const int LIGHT_SENSOR_PIN = A5; // Analog pin for light sensor

const int SERVO_PIN = 10;
Servo lightServo;

const int TRG_PIN = 12;
const int ECHO_PIN = 13;
NewPing sonar(TRG_PIN, ECHO_PIN, 200);

const int LINE_SENSOR_PIN = A1;

const int BALLOON_PIN = A0;

const char fills[] = {' ', '_', '#', 'A'}; // Characters for visualizing light sensor values

bool battlemode = false;

void setup()
{
    Serial.begin(115200);

    // setup light sensor
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(BALLOON_PIN, INPUT);

    lightServo.attach(SERVO_PIN);

    lightServo.write(180);

    //determine mode based on ultrasonic distance at startup
    delay(2000); // wait for things to settle
    int distance = sonar.ping_cm();
    if (distance > 0 && distance < 20)
    {
        //other mode
        battlemode = false;
    }
    else
    {
        battlemode = true;
    }
}

const int NUM_READINGS = 40; // Number of readings for averaging
int values[NUM_READINGS];

void loop()
{
//    Serial.println(analogRead(BALLOON_PIN));
//    return;
    if (battlemode)
    {
        //spin on startup, watching for the ballon to pop
        if (analogRead(BALLOON_PIN) >= 50)
        {
            //just spin real fast
            Right(255);
        }
        else
        {
            Stop();
        }
    }
    else
    {
        delay(100);
        ReadInValues();

        // drive towards the light
        // using the ultrasonic sensor
        int distance = sonar.ping_cm();

        if (distance == 0)
        {
            distance = 200; // If no echo, assume max distance
        }

        // clamp
        if (distance > 30)
        {
            distance = 30;
        }
        distance = map(distance, 0, 30, -255, 255);
        Drive(distance);

        const int LINE_THRESHOLD = 870;
        int value = analogRead(LINE_SENSOR_PIN);
        Serial.println(value);
        int totalDelayCounter = 0;
        const int desiredDelay = 1000;
        while (value < LINE_THRESHOLD && totalDelayCounter <= desiredDelay)
        {
            const int delayPerLoop = 1;
            delay(delayPerLoop);
            totalDelayCounter += delayPerLoop;
            value = analogRead(LINE_SENSOR_PIN);
        }

        Stop();
    }
}

void ReadInValues()
{
    // Turn(90, 255); // Initial turn to look for light
    // Turn(-90, 255);
    // return;

    // Read multiple values from the light sensor
    for (int i = 0; i < NUM_READINGS; i++)
    {
        // drive the servo to the scan position
        long rotation = map(i, 0, NUM_READINGS - 1, 180, 0);
        lightServo.write(rotation);
        delay(50);
        values[i] = analogRead(LIGHT_SENSOR_PIN);
    }
    lightServo.write(180);

    // find the min and max values
    int minValue;
    int maxValue;
    int minValueAngle;
    for (int i = 0; i < NUM_READINGS; i++)
    {
        if (i == 0 || values[i] < minValue)
        {
            minValue = values[i];
            minValueAngle = map(i, 0, NUM_READINGS - 1, 180, 0);
        }
        if (i == 0 || values[i] > maxValue)
        {
            maxValue = values[i];
        }
    }

    // print out the values
    Serial.print("Light Sensor Readings:");
    Serial.println();

    // remap the values to a range of 0 to 4
    for (int i = 0; i < NUM_READINGS; i++)
    {
        // Normalize the value to a range of 0 to 4
        values[i] = map(values[i], minValue, maxValue, 3, 0);
        // Print the visual representation using fills
        Serial.print(fills[values[i]]);
    }
    Serial.println();

    // print indicators for left, right, and center
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

    Serial.println("Max Value: " + String(maxValue) + " at angle " + String(minValueAngle));
    Serial.println("Min Value: " + String(minValue));

        // remap the max angle to a turn angle
    int turnAngle = map(minValueAngle, 0, 180, 90, -90);
    Turn(turnAngle, 100);
}

// util functions

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
