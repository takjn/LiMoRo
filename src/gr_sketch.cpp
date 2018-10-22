// #define ENABLE_LCD
#define ENABLE_DETECT_FACE
#define ENABLE_CLOUD_FUNCTION
#define LIMORO_ID "aaaa"
#define WIFI_SSID "xxxx"
#define WIFI_PW "yyyy"
#define SERVER_URL "http://example.com/"

#include "EasyDec_WavCnv2ch.h"
#include "EasyPlayback.h"
#include "Robot.h"
#include "SimpleIR.h"
#include <Arduino.h>
#include <Camera.h>
#include <LCD.h>
#include <SdUsbConnect.h>
static EasyPlayback audio_player;

#include "BD1020.h"
#include "RPR-0521RS.h"
#include <Wire.h>
PinName tempout_pin = A0;
BD1020 bd1020;
RPR0521RS rpr0521rs;

#ifdef ENABLE_CLOUD_FUNCTION
#include <ESP32Interface.h>
#include <Firebase.h>
#include <WiFiUtility.h>

ESP32Interface wifi;
RTC rtc;
Firebase firebase(&wifi, SERVER_URL, LIMORO_ID);
#endif

// Camera & storage
#define IMAGE_HW 320
#define IMAGE_VW 240

#ifdef ENABLE_DETECT_FACE
/* FACE DETECTOR Parameters */
#include <opencv.hpp>
using namespace cv;
#define DETECTOR_SCALE_FACTOR (2)
#define DETECTOR_MIN_NEIGHBOR (4)
#define DETECTOR_MIN_SIZE (30)
#define FACE_DETECTOR_MODEL "/storage/lbpcascade_frontalface.xml"
static CascadeClassifier detector_classifier;
static bool face_detected = false;
#endif

static Camera camera(IMAGE_HW, IMAGE_VW);
static SdUsbConnect storage("storage");
static LCD lcd(IMAGE_HW, IMAGE_VW);
bool lcd_on = true;

// IR remote control
unsigned int ir_data[1024];
unsigned int ir_data_tv[] = {3479, 1621, 444, 340, 534, 1203, 446, 339, 505, 1233, 446, 339, 506, 1231, 445, 341, 503, 1234, 444, 342, 532, 1205, 442, 339, 508, 1233, 443, 1211, 506, 361, 444, 1186, 502, 391, 444, 1192, 490, 1179, 535, 1204, 445, 1181, 506, 391, 481, 303, 506, 333, 507, 1177, 533, 306, 505, 1190, 520, 307, 508, 390, 446, 1177, 507, 393, 443, 397, 448, 335, 505, 395, 444, 1180, 504, 1180, 498, 341, 505, 1185, 492, 338, 536, 315, 492, 396, 444, 1187, 494, 398, 444, 338, 509, 341, 494, 1177, 537, 308, 508, 1170, 510, 1231, 443, 70082, 3477, 1568, 536, 310, 503, 1173, 507, 347, 487, 1180, 507, 345, 492, 1178, 508, 341, 499, 1175, 507, 340, 496, 1178, 508, 342, 499, 1175, 535, 1203, 444, 338, 509, 1232, 444, 338, 509, 1232, 444, 1186, 531, 1147, 503, 1234, 442, 345, 502, 338, 534, 361, 443, 1180, 537, 361, 444, 1180, 507, 392, 443, 343, 532, 1204, 444, 349, 496, 336, 536, 363, 481, 305, 500, 1235, 444, 1180, 537, 361, 443, 1180, 537, 315, 492, 348, 492, 340, 534, 1205, 444, 338, 509, 345, 495, 392, 444, 1211, 468, 399, 442, 1188, 495, 1181, 533};
unsigned int ir_data_aircon[] = {3470, 1744, 401, 467, 396, 468, 395, 495, 369, 1331, 398, 468, 394, 1303, 429, 495, 368, 495, 367, 1301, 428, 496, 367, 496, 369, 496, 367, 495, 368, 1298, 432, 1311, 418, 495, 369, 1296, 433, 495, 368, 1331, 400, 1295, 434, 1331, 397, 1298, 434, 493, 370, 495, 366, 497, 368, 495, 366, 472, 392, 469, 396, 1312, 416, 497, 366, 473, 390, 470, 394, 1308, 424, 1301, 427, 1298, 433, 1328, 400, 468, 394, 1304, 428, 1328, 401, 1295, 435, 495, 367, 1331, 399, 466, 396, 1309, 421, 1332, 399, 1294, 436, 495, 366, 1304, 426, 1307, 423, 496, 366, 1301, 431, 495, 367, 496, 367, 496, 368, 1305, 425, 495, 368, 10837, 3467, 1715, 433, 496, 368, 496, 368, 495, 366, 1300, 432, 495, 367, 1331, 399, 470, 392, 474, 391, 1311, 418, 497, 364, 497, 367, 497, 368, 495, 368, 1333, 397, 1298, 432, 495, 368, 1333, 394, 498, 367, 1309, 421, 1332, 397, 496, 368, 1308, 421, 1333, 396, 497, 367, 1298, 432, 1331, 399, 1297, 432, 1332, 396, 1301, 430, 1312, 416, 1309, 423, 1295, 434, 497, 366, 497, 364, 498, 367, 496, 368, 497, 366, 497, 365, 498, 367, 497, 364, 1300, 432, 1332, 395, 1305, 427, 1311, 418, 1311, 419, 1296, 433, 1311, 418, 1309, 422, 495, 367, 497, 367, 497, 365, 497, 367, 497, 368, 495, 366, 545, 318, 499, 365, 10838, 3473, 1750, 394, 498, 366, 498, 368, 495, 367, 1334, 396, 495, 367, 1306, 425, 497, 365, 497, 366, 1301, 430, 497, 366, 498, 363, 500, 365, 497, 366, 1300, 430, 1334, 394, 499, 364, 1314, 418, 497, 364, 1336, 395, 1298, 432, 497, 366, 497, 366, 1302, 428, 1295, 433, 1335, 396, 1297, 434, 1333, 395, 1315, 414, 1335, 396, 1299, 430, 1319, 410, 1307, 424, 495, 367, 498, 365, 499, 364, 499, 364, 499, 364, 498, 368, 496, 365, 499, 365, 1335, 396, 1296, 433, 1335, 393, 1301, 430, 1298, 430, 1312, 420, 1295, 434, 1312, 417, 500, 363, 497, 367, 497, 366, 497, 365, 499, 365, 498, 366, 497, 366, 497, 366};
unsigned int ir_data_light[] = {9036, 4435, 511, 646, 485, 1727, 509, 1728, 509, 621, 527, 604, 533, 572, 534, 582, 550, 1677, 509, 621, 535, 597, 535, 571, 535, 595, 537, 569, 536, 594, 537, 1674, 561, 570, 537, 594, 534, 1676, 561, 571, 537, 1675, 562, 1673, 562, 597, 534, 571, 535, 595, 537, 1674, 562, 569, 537, 1674, 562, 595, 537, 569, 535, 1676, 562, 1673, 562, 1674, 564};

SimpleIR ir(D1, D0);

// Body
Robot body(A0, D4, D5, D6, D7, D8, D9, D10, D11);
#define LED_HEART 13
uint8_t led_heart_level = 0;

// timer
uint32_t last_millis = 0;
int last_minute = -1;

void setup() {
    Serial.begin(115200);

    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_HEART, OUTPUT);
    pinMode(USER_BUTTON0, INPUT);
    pinMode(USER_BUTTON1, INPUT);

#ifdef ENABLE_CLOUD_FUNCTION
    // WiFi
    wifiSetup();
#endif

    // Audio Player
    // decoder setting
    audio_player.add_decoder<EasyDec_WavCnv2ch>(".wav");
    audio_player.add_decoder<EasyDec_WavCnv2ch>(".WAV");

    // volume control
    audio_player.outputVolume(0.5); // Volume control (min:0.0 max:1.0)

    // Camera
    camera.begin();
#ifdef ENABLE_LCD
    lcd.begin(camera.getImageAdr(), camera.getWidth(), camera.getHeight());
    lcd.clear();
#endif

    // Sensors
    Wire.begin();
    rpr0521rs.init();
    bd1020.init(tempout_pin);

    // SD & USB
    Serial.print("Finding strage..");
    storage.wait_connect();
    Serial.println("done");
    Serial.println("Push UB0 to take a photo.");

#ifdef ENABLE_DETECT_FACE
    // Load the cascade classifier file
    detector_classifier.load(FACE_DETECTOR_MODEL);
    if (detector_classifier.empty()) {
        digitalWrite(PIN_LED_RED, HIGH); // Error
        CV_Assert(0);
        mbed_die();
    }
#endif
}

void take_photo() {
    FILE *wp = fopen("/storage/lychee_camera.jpg", "w");
    if (wp != NULL) {
        size_t size = camera.createJpeg();
        fwrite(camera.getJpegAdr(), sizeof(char), (int)size, wp);
        fclose(wp);
        audio_player.play("/storage/camera-shutter2.wav");
    } else {
        Serial.print("Not found jpg");
        digitalWrite(LED_RED, HIGH);
        delay(1);
    }
    Serial.println("done to take a photo");

#ifdef ENABLE_CLOUD_FUNCTION
    size_t size = camera.createJpeg();
    firebase.post_photo(camera.getJpegAdr(), size);
#endif
}

int receive_ir() {
    int ret = 0;

    // 赤外線リモコン信号の受信
    // printf("IR signal waiting..\r\n");

    ret = ir.receive(ir_data, sizeof(ir_data) / sizeof(ir_data[0]));
    if (ret > 0) {
        for (int i = 0; i < ret; i++) {
            printf("%d, ", ir_data[i]);
        }
        printf("\r\n");
    }

    return ret;
}

void blink() {
    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_HEART, HIGH);
        delay(50);
        digitalWrite(LED_HEART, LOW);
        delay(50);
    }
}

bool detect_face() {
    bool ret = false;

    Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

    Mat src;
    cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY

    // Detect a face in the frame
    Rect face_roi;
    if (detector_classifier.empty()) {
        digitalWrite(PIN_LED_RED, HIGH); // Error
    }

    // Perform detected the biggest face
    std::vector<Rect> rect_faces;
    detector_classifier.detectMultiScale(src, rect_faces,
                                         DETECTOR_SCALE_FACTOR,
                                         DETECTOR_MIN_NEIGHBOR,
                                         CASCADE_FIND_BIGGEST_OBJECT,
                                         Size(DETECTOR_MIN_SIZE, DETECTOR_MIN_SIZE));

    if (rect_faces.size() > 0) {
        // A face is detected
        face_roi = rect_faces[0];
        if (face_roi.width > 0 && face_roi.height > 0) {
            printf("Detected a face X:%d Y:%d W:%d H:%d\r\n", face_roi.x, face_roi.y, face_roi.width, face_roi.height);
            ret = true;

            // look a face
            if ((face_roi.x + face_roi.width / 2) > IMAGE_HW / 2 + 10) {
                body.look_right();
            } else if ((face_roi.x + face_roi.width / 2) < IMAGE_HW / 2 - 10) {
                body.look_left();
            }

            // TODO:take a photo
        }
    } else {
        // No face is detected, set an invalid rectangle
        face_roi.x = -1;
        face_roi.y = -1;
        face_roi.width = -1;
        face_roi.height = -1;
    }

    return ret;
}

void loop() {

    body.loop();

#ifdef ENABLE_CLOUD_FUNCTION
    int year, mon, day, hour, min, sec, week;
    rtc.getDateTime(year, mon, day, hour, min, sec, week);

    //  Check command
    if (millis() - last_millis > 1000) {
        string ret = firebase.get_command();
        if (ret == "ERROR") {
            delay(5000);
        } else if (ret == "PHOTO") {
            digitalWrite(LED_GREEN, HIGH);
            blink();
            take_photo();
            size_t size = camera.createJpeg();
            int ret = firebase.post_photo(camera.getJpegAdr(), size);
            if (ret == 0) {
                // const char message[] = "写真を撮りました";
                // firebase.post_message(message, strlen(message));
            } else {
                const char message[] = "通信に失敗しました";
                firebase.post_message(message, strlen(message));
            }
            digitalWrite(LED_GREEN, LOW);
        } else if (ret == "TV") {
            blink();
            ir.transmit(ir_data_tv, sizeof(ir_data_tv) / sizeof(ir_data_tv[0]));
            // const char message[] = "テレビをつけました";
            // firebase.post_message(message, strlen(message));
        } else if (ret == "AIRCON") {
            blink();
            ir.transmit(ir_data_aircon, sizeof(ir_data_aircon) / sizeof(ir_data_aircon[0]));
            // const char message[] = "エアコンをつけました";
            // firebase.post_message(message, strlen(message));
        } else if (ret == "LIGHT") {
            blink();
            ir.transmit(ir_data_light, sizeof(ir_data_light) / sizeof(ir_data_light[0]));
            // const char message[] = "ライトをつけました";
            // firebase.post_message(message, strlen(message));
        } else if (ret == "SWING") {
            blink();
            body.startMotion(1, 10000);
        } else if (ret == "STOP") {
            blink();
            body.stop();
        } else if (ret == "WALK_FORWARD") {
            blink();
            body.startMotion(2, 0);
            // body.walkForwards();
        } else if (ret == "WALK_BACKWARD") {
            blink();
            body.startMotion(3, 0);
            // body.walkBackwards();
        } else if (ret == "TURN_LEFT") {
            blink();
            body.turnLeft();
        } else if (ret == "TURN_RIGHT") {
            blink();
            body.turnRight();
        }

        last_millis = millis();
        led_heart_level = 255;
    }

    if (min != last_minute) {
        // temperature
        float temp;
        bd1020.get_val(&temp);

        // lx
        byte return_code;
        unsigned short ps_val;
        float als_val;
        return_code = rpr0521rs.get_psalsval(&ps_val, &als_val);
        if (return_code == 0) {
            firebase.post_sensor(temp, als_val);
        }

        // take a photo
        if (face_detected) {
            blink();
            take_photo();
            size_t size = camera.createJpeg();
            face_detected = false;
        }

        last_minute = min;
    }

#endif

    // Heart beat
    // if (led_heart_level > 0)
    // {
    //     led_heart_level --;
    // }
    // analogWrite(LED_HEART, led_heart_level);

    //  IR remote control
    int ir_length = receive_ir();
    if (ir_length > 50) {
        blink();
#ifdef ENABLE_CLOUD_FUNCTION
        const char message[] = "remocon";
        firebase.post_last_action(message, strlen(message));
#endif
    }

    //  Camera
    if (digitalRead(USER_BUTTON0) == LOW) {
        digitalWrite(LED_GREEN, HIGH);
        take_photo();
        while (digitalRead(USER_BUTTON0) == LOW)
            ;
        digitalWrite(LED_GREEN, LOW);
    }

#ifdef ENABLE_LCD
    if (digitalRead(USER_BUTTON1) == LOW) {
        lcd_on = !lcd_on;
        if (lcd_on) {
            lcd.restart();
        } else {
            lcd.stop();
        }
        while (digitalRead(USER_BUTTON1) == LOW)
            ;
    }
#endif

#ifdef ENABLE_DETECT_FACE
    face_detected = face_detected | detect_face();
#endif
}
