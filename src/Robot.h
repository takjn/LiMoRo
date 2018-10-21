/**
 * Robot.h - Robot library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <Arduino.h>
#include <Servo.h>

class Robot {
  public:
    Robot(PinName sonar, PinName head, PinName neck, PinName leftArm, PinName rightArm, PinName leftHip, PinName rightHip, PinName leftAnkle, PinName rightAnkle);
    ~Robot();

    void stop();
    void turnRight();
    void turnLeft();
    double sonar();
    void ping();
    void walkForwards();
    void walkBackwards();
    void loop();
    void swing();
    void startMotion(int, int);
    void look_right();
    void look_left();

  private:
    const int STEP_DELAY = 200; //  small delay inbetween step moves, adjust to slow down walk
    const int ANGLE_DELAY = 20; //  small dealy to slow down servo speed

    const int MAX_ANKLE_ANGLE = 7;
    const int MAX_HIP_ANGLE = 25;

    //  Enter actual servo alingment values here
    const int LEFT_HIP_CENTER = 90;
    const int RIGHT_HIP_CENTER = 90;
    const int LEFT_ANKLE_CENTER = 90;
    const int RIGHT_ANKLE_CENTER = 90;
    const int HEAD_CENTER = 90;
    const int NECK_CENTER = 80;
    const int LEFT_ARM_CENTER = 90;
    const int RIGHT_ARM_CENTER = 90;

    unsigned long start_motion_millis = 0;
    int motion_mode = 0;
    int motion_loop = 0;
    int motion_timeout = 0;

    Servo servoLeftHip;
    Servo servoRightHip;
    Servo servoLeftAnkle;
    Servo servoRightAnkle;
    Servo servoHead;
    Servo servoNeck;
    Servo servoLeftArm;
    Servo servoRightArm;

    PinName pinSonar;

    int neckPosition = 0;
    int leftArmPosition = 0;
    int rightArmPosition = 0;
    int leftAnklePosition = 0;
    int rightAnklePosition = 0;
    int leftHipPosition = 0;
    int rightHipPosition = 0;

    void walkForwards1();
    void walkForwards2();
    void walkForwards3();
    void walkForwards4();
};
#endif
