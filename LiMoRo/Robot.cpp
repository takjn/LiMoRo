/**
 * Robot.h - Robot library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#include "Robot.h"

Robot::Robot(PinName leftHip, PinName rightHip, PinName leftAnkle, PinName rightAnkle)
{
    // attach the servo
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
    servoLeftAnkle.write(LEFT_HIP_CENTER);
    servoRightAnkle.write(RIGHT_HIP_CENTER);
    servoLeftHip.write(LEFT_ANKLE_CENTER);
    servoRightHip.write(RIGHT_ANKLE_CENTER);

    LAP = 0;
    RAP = 0;
    LHP = 0;
    RHP = 0;

    delay(STEP_DELAY);
}

//  This is the Walking Forwards Sub Routine.
void Robot::walkForwards1()
{
    // Ankle goto 23deg
    while (LAP < MAX_ANCLE_ANGLE)
    {
        LAP = LAP + 1;
        RAP = RAP + 1;
        servoLeftAnkle.write(LEFT_ANKLE_CENTER - LAP);
        servoRightAnkle.write(RIGHT_ANKLE_CENTER - RAP);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards2()
{
    // Hip goto 25deg
    while (LHP < MAX_HIP_ANGLE)
    {
        LHP = LHP + 1;
        RHP = RHP + 1;
        servoLeftHip.write(LEFT_HIP_CENTER + LHP);
        servoRightHip.write(RIGHT_HIP_CENTER + RHP);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards3()
{
    // Ancle goto -23deg
    while (LAP > -MAX_ANCLE_ANGLE)
    {
        LAP = LAP - 1;
        RAP = RAP - 1;
        servoLeftAnkle.write(LEFT_ANKLE_CENTER - LAP);
        servoRightAnkle.write(RIGHT_ANKLE_CENTER - RAP);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards4()
{
    // Hip goto -25deg
    while (LHP > -MAX_HIP_ANGLE)
    {
        LHP = LHP - 1;
        RHP = RHP - 1;
        servoLeftHip.write(LEFT_HIP_CENTER + LHP);
        servoRightHip.write(RIGHT_HIP_CENTER + RHP);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

//  This is the Walking Backwards Sub Routine.
void Robot::walkBackwards()
{

Move1:
    walkForwards1();

Move2:
    // Hip goto 25deg
    while (LHP < MAX_HIP_ANGLE)
    {
        LHP = LHP + 1;
        RHP = RHP + 1;
        servoLeftHip.write(LEFT_HIP_CENTER - LHP);
        servoRightHip.write(RIGHT_HIP_CENTER - RHP);
        delay(ANGLE_DELAY);
    }
    delay(STEP_DELAY);

Move3:
    walkForwards3();

Move4:
    // Hip goto -25deg
    while (LHP > -MAX_HIP_ANGLE)
    {
        LHP = LHP - 1;
        RHP = RHP - 1;
        servoLeftHip.write(LEFT_HIP_CENTER - LHP);
        servoRightHip.write(RIGHT_HIP_CENTER - RHP);
        delay(ANGLE_DELAY);
    }
    delay(STEP_DELAY);
}

//this is a 90degree turn to the right sub routine
void Robot::turnRight()
{
Move1:
    LAP = LAP + 1;
    RAP = RAP + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + RAP);
    delay(ANGLE_DELAY);
    if (LAP < 23)
    {
        goto Move1;
    } //Left Ankle goto 23deg
    if (RAP < 23)
    {
        goto Move1;
    } //Right Ankle goto 23deg

    delay(STEP_DELAY);

Move2:
    RHP = RHP + 1;
    servoRightHip.write(RIGHT_HIP_CENTER - RHP);
    delay(ANGLE_DELAY);
    if (RHP < 30)
    {
        goto Move2;
    } //Right Hipe goto 25deg

    delay(STEP_DELAY);

Move3:
    LAP = LAP - 1;
    RAP = RAP - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + RAP);
    delay(ANGLE_DELAY);
    if (LAP > -23)
    {
        goto Move3;
    } //Left Ankle goto 23degrees
    if (RAP > -23)
    {
        goto Move3;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move4:
    LHP = LHP - 1;
    servoLeftHip.write(LEFT_HIP_CENTER + LHP);
    delay(ANGLE_DELAY);
    if (LHP > -25)
    {
        goto Move4;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move5:
    RHP = RHP - 1;
    servoRightHip.write(RIGHT_HIP_CENTER - RHP);
    delay(ANGLE_DELAY);
    if (RHP > 0)
    {
        goto Move5;
    } //Right Hipe goto 25deg

    delay(STEP_DELAY);

Move6:
    LAP = LAP + 1;
    RAP = RAP + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + RAP);
    delay(ANGLE_DELAY);
    if (LAP < +23)
    {
        goto Move6;
    } //Left Ankle goto 23degrees
    if (RAP < +23)
    {
        goto Move6;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move7:
    LHP = LHP + 1;
    servoLeftHip.write(LEFT_HIP_CENTER + LHP);
    delay(ANGLE_DELAY);
    if (LHP < 0)
    {
        goto Move7;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move8:
    LAP = LAP - 1;
    RAP = RAP - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + RAP);
    delay(ANGLE_DELAY);
    if (LAP > 0)
    {
        goto Move8;
    } //Left Ankle goto 23degrees
    if (RAP > 0)
    {
        goto Move8;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);
}

//this is a 90degree turn to the left sub routine
void Robot::turnLeft()
{
Move1:
    LAP = LAP + 1;
    RAP = RAP + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - RAP);
    delay(ANGLE_DELAY);
    if (LAP < 23)
    {
        goto Move1;
    } //Left Ankle goto 23deg
    if (RAP < 23)
    {
        goto Move1;
    } //Right Ankle goto 23deg

    delay(STEP_DELAY);

Move2:
    LHP = LHP + 1;
    servoLeftHip.write(LEFT_HIP_CENTER + LHP);
    delay(ANGLE_DELAY);
    if (LHP < 30)
    {
        goto Move2;
    }

    delay(STEP_DELAY);

Move3:
    LAP = LAP - 1;
    RAP = RAP - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - RAP);
    delay(ANGLE_DELAY);
    if (LAP > -23)
    {
        goto Move3;
    } //Left Ankle goto 23degrees
    if (RAP > -23)
    {
        goto Move3;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move4:
    RHP = RHP - 1;
    servoRightHip.write(RIGHT_HIP_CENTER - RHP);
    delay(ANGLE_DELAY);
    if (RHP > -25)
    {
        goto Move4;
    }

    delay(STEP_DELAY);

Move5:
    LHP = LHP - 1;
    servoLeftHip.write(LEFT_HIP_CENTER + LHP);
    delay(ANGLE_DELAY);
    if (LHP > 0)
    {
        goto Move5;
    }

    delay(STEP_DELAY);

Move6:
    LAP = LAP + 1;
    RAP = RAP + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - RAP);
    delay(ANGLE_DELAY);
    if (LAP < +23)
    {
        goto Move6;
    } //Left Ankle goto 23degrees
    if (RAP < +23)
    {
        goto Move6;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move7:
    RHP = RHP + 1;
    servoRightHip.write(RIGHT_HIP_CENTER - RHP);
    delay(ANGLE_DELAY);
    if (RHP < 0)
    {
        goto Move7;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move8:
    LAP = LAP - 1;
    RAP = RAP - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - LAP);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - RAP);
    delay(ANGLE_DELAY);
    if (LAP > 0)
    {
        goto Move8;
    } //Left Ankle goto 23degrees
    if (RAP > 0)
    {
        goto Move8;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);
}
