/**
 * Robot.h - Robot library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#include "Robot.h"

Robot::Robot(PinName sonar, PinName head, PinName neck, PinName leftArm, PinName rightArm, PinName leftHip, PinName rightHip, PinName leftAnkle, PinName rightAnkle) {
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

Robot::~Robot() {
}

void Robot::stop() {
    motion_mode = 0;

    servoHead.write(NECK_CENTER);
    servoNeck.write(NECK_CENTER);
    servoLeftArm.write(LEFT_ARM_CENTER);
    servoRightArm.write(RIGHT_ARM_CENTER);
    servoLeftAnkle.write(LEFT_HIP_CENTER);
    servoRightAnkle.write(RIGHT_HIP_CENTER);
    servoLeftHip.write(LEFT_ANKLE_CENTER);
    servoRightHip.write(RIGHT_ANKLE_CENTER);

    neckPosition = 0;
    leftArmPosition = 0;
    rightArmPosition = 0;
    leftAnklePosition = 0;
    rightAnklePosition = 0;
    leftHipPosition = 0;
    rightHipPosition = 0;

    delay(STEP_DELAY);
}

//  This is the Walking Forwards Sub Routine.
void Robot::walkForwards1() {
    // Ankle goto 23deg
    while (leftAnklePosition < MAX_ANKLE_ANGLE) {
        leftAnklePosition = leftAnklePosition + 1;
        rightAnklePosition = rightAnklePosition + 1;
        servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
        servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards2() {
    // Hip goto 25deg
    while (leftHipPosition < MAX_HIP_ANGLE) {
        leftHipPosition = leftHipPosition + 1;
        rightHipPosition = rightHipPosition + 1;
        servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER + rightHipPosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards3() {
    // ANKLE goto -23deg
    while (leftAnklePosition > -MAX_ANKLE_ANGLE) {
        leftAnklePosition = leftAnklePosition - 1;
        rightAnklePosition = rightAnklePosition - 1;
        servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
        servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

void Robot::walkForwards4() {
    // Hip goto -25deg
    while (leftHipPosition > -MAX_HIP_ANGLE) {
        leftHipPosition = leftHipPosition - 1;
        rightHipPosition = rightHipPosition - 1;
        servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER + rightHipPosition);
        delay(ANGLE_DELAY);
    }

    delay(STEP_DELAY);
}

//  This is the Walking Backwards Sub Routine.
void Robot::walkBackwards() {
    // Ankle goto 23deg
    walkForwards1();

    // Hip goto 25deg
    while (leftHipPosition < MAX_HIP_ANGLE) {
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
    while (leftHipPosition > -MAX_HIP_ANGLE) {
        leftHipPosition = leftHipPosition - 1;
        rightHipPosition = rightHipPosition - 1;
        servoLeftHip.write(LEFT_HIP_CENTER - leftHipPosition);
        servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
        delay(ANGLE_DELAY);
    }
    delay(STEP_DELAY);
}

//this is a 90degree turn to the right sub routine
void Robot::turnRight() {
Move1:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < MAX_ANKLE_ANGLE) {
        goto Move1;
    } //Left Ankle goto 23deg
    if (rightAnklePosition < MAX_ANKLE_ANGLE) {
        goto Move1;
    } //Right Ankle goto 23deg

    delay(STEP_DELAY);

Move2:
    rightHipPosition = rightHipPosition + 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition < MAX_HIP_ANGLE) {
        goto Move2;
    } //Right Hipe goto 25deg

    delay(STEP_DELAY);

Move3:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > -MAX_ANKLE_ANGLE) {
        goto Move3;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > -MAX_ANKLE_ANGLE) {
        goto Move3;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move4:
    leftHipPosition = leftHipPosition - 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition > -MAX_HIP_ANGLE) {
        goto Move4;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move5:
    rightHipPosition = rightHipPosition - 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition > 0) {
        goto Move5;
    } //Right Hipe goto 25deg

    delay(STEP_DELAY);

Move6:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < +MAX_ANKLE_ANGLE) {
        goto Move6;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition < +MAX_ANKLE_ANGLE) {
        goto Move6;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move7:
    leftHipPosition = leftHipPosition + 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition < 0) {
        goto Move7;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move8:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER + leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER + rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > 0) {
        goto Move8;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > 0) {
        goto Move8;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);
}

//this is a 90degree turn to the left sub routine
void Robot::turnLeft() {
Move1:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < MAX_ANKLE_ANGLE) {
        goto Move1;
    } //Left Ankle goto 23deg
    if (rightAnklePosition < MAX_ANKLE_ANGLE) {
        goto Move1;
    } //Right Ankle goto 23deg

    delay(STEP_DELAY);

Move2:
    leftHipPosition = leftHipPosition + 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition < MAX_HIP_ANGLE) {
        goto Move2;
    }

    delay(STEP_DELAY);

Move3:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > -MAX_ANKLE_ANGLE) {
        goto Move3;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > -MAX_ANKLE_ANGLE) {
        goto Move3;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move4:
    rightHipPosition = rightHipPosition - 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition > -MAX_HIP_ANGLE) {
        goto Move4;
    }

    delay(STEP_DELAY);

Move5:
    leftHipPosition = leftHipPosition - 1;
    servoLeftHip.write(LEFT_HIP_CENTER + leftHipPosition);
    delay(ANGLE_DELAY);
    if (leftHipPosition > 0) {
        goto Move5;
    }

    delay(STEP_DELAY);

Move6:
    leftAnklePosition = leftAnklePosition + 1;
    rightAnklePosition = rightAnklePosition + 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition < +MAX_ANKLE_ANGLE) {
        goto Move6;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition < +MAX_ANKLE_ANGLE) {
        goto Move6;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);

Move7:
    rightHipPosition = rightHipPosition + 1;
    servoRightHip.write(RIGHT_HIP_CENTER - rightHipPosition);
    delay(ANGLE_DELAY);
    if (rightHipPosition < 0) {
        goto Move7;
    } //Left Hip goto -25deg

    delay(STEP_DELAY);

Move8:
    leftAnklePosition = leftAnklePosition - 1;
    rightAnklePosition = rightAnklePosition - 1;
    servoLeftAnkle.write(LEFT_ANKLE_CENTER - leftAnklePosition);
    servoRightAnkle.write(RIGHT_ANKLE_CENTER - rightAnklePosition);
    delay(ANGLE_DELAY);
    if (leftAnklePosition > 0) {
        goto Move8;
    } //Left Ankle goto 23degrees
    if (rightAnklePosition > 0) {
        goto Move8;
    } //Right Ankle goto 23degrees

    delay(STEP_DELAY);
}

double Robot::sonar() {
    // TODO
    return 100;

    double ans;
    int i;
    ans = 0;
    for (i = 0; i < 100; i++) {
        ans = ans + analogRead(pinSonar);
    }
    return 19501.14 * pow(ans / 100, -1.256676);
}

void Robot::ping() {
    while (true) {
        if (sonar() > 10) {
            break;
        }

        stop();
        walkBackwards();

        if (sonar() < 6) {
            continue;
        }

        stop();
        turnRight(); //by about 90degrees

        if (sonar() > 10) {
            break;
        }

        stop();
        turnLeft(); //by about 90degrees
        turnLeft(); //by about 90degrees

        if (sonar() > 10) {
            break;
        }
    }
}

void Robot::walkForwards() {
    ping();
    walkForwards1();
    ping();
    walkForwards2();
    ping();
    walkForwards3();
    ping();
    walkForwards4();
}

void Robot::loop() {
    if (motion_timeout > 0) {
        if (start_motion_millis + motion_timeout < millis()) {
            stop();
        }
    }

    if (motion_mode == 1) {
        swing();
        motion_loop++;
    }
    if (motion_mode == 2) {
        walkForwards();
    }
    if (motion_mode == 3) {
        walkBackwards();
    }
}

void Robot::swing() {
    if (motion_loop < 20) {
        leftArmPosition += 1;
        rightArmPosition += 1;
    } else if (motion_loop < 58) {
        leftArmPosition -= 1;
        rightArmPosition -= 1;
    } else if (motion_loop < 76) {
        leftArmPosition += 1;
        rightArmPosition += 1;
    } else {
        leftArmPosition = 0;
        rightArmPosition = 0;
        motion_loop = 0;
    }

    servoLeftArm.write(LEFT_ARM_CENTER + leftArmPosition);
    servoRightArm.write(RIGHT_ARM_CENTER + rightArmPosition);
    delay(ANGLE_DELAY);
    // printf("%d: %d, %d\r\n", motion_loop, leftArmPosition, rightArmPosition);
}

void Robot::startMotion(int mode, int timeout) {
    motion_mode = mode;
    motion_timeout = timeout;
    start_motion_millis = millis();
}

void Robot::look_left() {
    if (neckPosition < 25) {
        neckPosition++;
    }
    servoNeck.write(NECK_CENTER + neckPosition);
}

void Robot::look_right() {
    if (neckPosition > -25) {
        neckPosition--;
    }
    servoNeck.write(NECK_CENTER + neckPosition);
}
