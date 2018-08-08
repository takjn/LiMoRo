/**
 * Robot.h - Robot library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <Arduino.h>
#include <Servo.h>

class Robot
{
  public:
    Robot(PinName leftHip, PinName rightHip, PinName leftAnkle, PinName rightAnkle);
    ~Robot();

    void stop();
    void walkForwards1();
    void walkForwards2();
    void walkForwards3();
    void walkForwards4();
    void walkBackwards();
    void turnRight();
    void turnLeft();

  private:
    const int STEP_DELAY = 200; //  small delay inbetween step moves, adjust to slow down walk
    const int ANGLE_DELAY = 10; //  small dealy to slow down servo speed

    const int MAX_ANCLE_ANGLE = 23;
    const int MAX_HIP_ANGLE = 25;

    //  Enter actual servo alingment values here
    const int LEFT_HIP_CENTER = 90;
    const int RIGHT_HIP_CENTER = 97;
    const int LEFT_ANKLE_CENTER = 90;
    const int RIGHT_ANKLE_CENTER = 85;

    Servo servoLeftHip;
    Servo servoRightHip;
    Servo servoLeftAnkle;
    Servo servoRightAnkle;

    int LAP = 0;
    int RAP = 0;
    int LHP = 0;
    int RHP = 0;
};
#endif
