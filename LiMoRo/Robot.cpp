/**
 * Robot.h - Robot library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#include "Robot.h"

Robot::Robot(PinName sonar, PinName head, PinName neck, PinName leftArm, PinName rightArm, PinName leftHip, PinName rightHip, PinName leftAnkle, PinName rightAnkle)
{
    // attach the sonar
    pinSonar = sonar;

    // attach the servo
    servoHead.attach(head);
    servoNeck.attach(neck);
    servoLeftArm.attach(leftArm);
    servoRightArm.attach(rightArm);
    servoLeftHip.attach(leftHip);
    servoRightHip.attach(rightHip);
    servoLeftAnkle.attach(leftAnkle);
    servoRightAnkle.attach(rightAnkle);

    // move servo to the center position
    stop();
}

Robot::~Robot()
{
}

void Robot::stop()
{
    servoHead.write(NECK_CENTER);
    servoNeck.write(NECK_CENTER);
    servoLeftArm.write(LEFT_ARM_CENTER);
    servoRightArm.write(RIGHT_ARM_CENTER);
    servoLeftAnkle.write(LEFT_HIP_CENTER);
    servoRightAnkle.write(RIGHT_HIP_CENTER);
    servoLeftHip.write(LEFT_ANKLE_CENTER);
    servoRightHip.write(RIGHT_ANKLE_CENTER);

    leftAnklePosition = 0;
    rightAnklePosition = 0;
    leftHipPosition = 0;
    rightHipPosition = 0;

    delay(STEP_DELAY);
}

//  This is the Walking Forwards Sub Routine.
void Robot::walkForwards1()
{
    // Ankle goto 23deg
    while (leftAnklePosition < MAX_ANKLE_ANGLE)
    {
        leftAnklePosition = leftAnklePosition + 1;
        rightAnklePosition = rightAnklePosition + 1;
        servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
        servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards2()
{
    // Hip goto 25deg
    while (leftHipPosition < MAX_HIP_ANGLE)
    {
        leftHipPosition = leftHipPosition + 1;
        rightHipPosition = rightHipPosition + 1;
        servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER + rightHipPosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards3()
{
    // ANKLE goto -23deg
    while (leftAnklePosition > -MAX_ANKLE_ANGLE)
    {
        leftAnklePosition = leftAnklePosition - 1;
        rightAnklePosition = rightAnklePosition - 1;
        servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
        servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards4()
{
    // Hip goto -25deg
    while (leftHipPosition > -MAX_HIP_ANGLE)
    {
        leftHipPosition = leftHipPosition - 1;
        rightHipPosition = rightHipPosition - 1;
        servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER + rightHipPosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

//  This is the Walking Backwards Sub Routine.
void Robot::walkBackwards()
{
    // Ankle goto 23deg
    walkForwards1();

    // Hip goto 25deg
    while (leftHipPosition < MAX_HIP_ANGLE)
    {
        leftHipPosition = leftHipPosition + 1;
        rightHipPosition = rightHipPosition + 1;
        servoLeftHip.write(LEFT_HIP_CENTER - leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
        delay(ANGLE_DELAY);
    }
    delay(STEP_DELAY);

    // ANKLE goto -23deg
    walkForwards3();

    // Hip goto -25deg
    while (leftHipPosition > -MAX_HIP_ANGLE)
    {
        leftHipPosition = leftHipPosition - 1;
        rightHipPosition = rightHipPosition - 1;
        servoLeftHip.write(LEFT_HIP_CENTER - leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
        delay(ANGLE_DELAY);
    }
    delay(STEP_DELAY);
}

//this is a 90degree turn to the right sub routine
void Robot::turnRight()
{
Move1:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < 23)
    {
        goto Move1;
    } //Left Ankle goto 23deg
    if (rightAnklePosition < 23)
    {
        goto Move1;
    } //Right Ankle goto 23deg

    delay(STEP_DELAY);

Move2:
    rightHipPosition = rightHipPosition + 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition < 30)
    {
        goto Move2;
    } //Right Hipe goto 25deg

    delay(STEP_DELAY);

Move3:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > -23)
    {
        goto Move3;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > -23)
    {
        goto Move3;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move4:
    leftHipPosition = leftHipPosition - 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition > -25)
    {
        goto Move4;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move5:
    rightHipPosition = rightHipPosition - 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition > 0)
    {
        goto Move5;
    } //Right Hipe goto 25deg

    delay(STEP_DELAY);

Move6:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < +23)
    {
        goto Move6;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition < +23)
    {
        goto Move6;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move7:
    leftHipPosition = leftHipPosition + 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition < 0)
    {
        goto Move7;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move8:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > 0)
    {
        goto Move8;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > 0)
    {
        goto Move8;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);
}

//this is a 90degree turn to the left sub routine
void Robot::turnLeft()
{
Move1:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < 23)
    {
        goto Move1;
    } //Left Ankle goto 23deg
    if (rightAnklePosition < 23)
    {
        goto Move1;
    } //Right Ankle goto 23deg

    delay(STEP_DELAY);

Move2:
    leftHipPosition = leftHipPosition + 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition < 30)
    {
        goto Move2;
    }

    delay(STEP_DELAY);

Move3:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > -23)
    {
        goto Move3;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > -23)
    {
        goto Move3;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move4:
    rightHipPosition = rightHipPosition - 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition > -25)
    {
        goto Move4;
    }

    delay(STEP_DELAY);

Move5:
    leftHipPosition = leftHipPosition - 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition > 0)
    {
        goto Move5;
    }

    delay(STEP_DELAY);

Move6:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < +23)
    {
        goto Move6;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition < +23)
    {
        goto Move6;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move7:
    rightHipPosition = rightHipPosition + 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition < 0)
    {
        goto Move7;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move8:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > 0)
    {
        goto Move8;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > 0)
    {
        goto Move8;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);
}

double Robot::sonar()
{
    double ans;
    int i;
    ans = 0;
    for (i = 0; i < 100; i++)
    {
        ans = ans + analogRead(pinSonar);
    }
    return 19501.14 * pow(ans / 100, -1.256676);
}

void Robot::ping()
{
    while (true)
    {
        if (sonar() > 10)
        {
            break;
        }

        stop();
        walkBackwards();

        if (sonar() < 6)
        {
            continue;
        }

        stop();
        turnRight(); //by about 90degrees

        if (sonar() > 10)
        {
            break;
        }

        stop();
        turnLeft(); //by about 90degrees
        turnLeft(); //by about 90degrees

        if (sonar() > 10)
        {
            break;
        }
    }
}

void Robot::demoWalk()
{
    ping();
    walkForwards1();
    ping();
    walkForwards2();
    ping();
    walkForwards3();
    ping();
    walkForwards4();
}
