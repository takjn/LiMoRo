#define ENABLE_CLOUD_FUNCTION
#define LIMORO_ID "aaaa"
#define WIFI_SSID "xxxx"
#define WIFI_PW "yyyy"
#define SERVER_URL "https://example.com/"

#include <Arduino.h>
#include <Camera.h>
#include <SdUsbConnect.h>
#include "Robot.h"
#include "SimpleIR.h"
#include "EasyPlayback.h"
#include "EasyDec_WavCnv2ch.h"
static EasyPlayback audio_player;

#ifdef ENABLE_CLOUD_FUNCTION
#include <ESP32Interface.h>
#include "WiFiUtility.h"
#include "Firebase.h"

ESP32Interface wifi;
Firebase firebase(&wifi, SERVER_URL, LIMORO_ID);
#endif

// Camera & storage
Camera camera(320, 240);
SdUsbConnect storage("storage");

// IR remote control
unsigned int ir_data[1024];
unsigned int ir_data_tv[] = {3479, 1621, 444, 340, 534, 1203, 446, 339, 505, 1233, 446, 339, 506, 1231, 445, 341, 503, 1234, 444, 342, 532, 1205, 442, 339, 508, 1233, 443, 1211, 506, 361, 444, 1186, 502, 391, 444, 1192, 490, 1179, 535, 1204, 445, 1181, 506, 391, 481, 303, 506, 333, 507, 1177, 533, 306, 505, 1190, 520, 307, 508, 390, 446, 1177, 507, 393, 443, 397, 448, 335, 505, 395, 444, 1180, 504, 1180, 498, 341, 505, 1185, 492, 338, 536, 315, 492, 396, 444, 1187, 494, 398, 444, 338, 509, 341, 494, 1177, 537, 308, 508, 1170, 510, 1231, 443, 70082, 3477, 1568, 536, 310, 503, 1173, 507, 347, 487, 1180, 507, 345, 492, 1178, 508, 341, 499, 1175, 507, 340, 496, 1178, 508, 342, 499, 1175, 535, 1203, 444, 338, 509, 1232, 444, 338, 509, 1232, 444, 1186, 531, 1147, 503, 1234, 442, 345, 502, 338, 534, 361, 443, 1180, 537, 361, 444, 1180, 507, 392, 443, 343, 532, 1204, 444, 349, 496, 336, 536, 363, 481, 305, 500, 1235, 444, 1180, 537, 361, 443, 1180, 537, 315, 492, 348, 492, 340, 534, 1205, 444, 338, 509, 345, 495, 392, 444, 1211, 468, 399, 442, 1188, 495, 1181, 533};
unsigned int ir_data_aircon[] = {3479, 1621, 444, 340, 534, 1203, 446, 339, 505, 1233, 446, 339, 506, 1231, 445, 341, 503, 1234, 444, 342, 532, 1205, 442, 339, 508, 1233, 443, 1211, 506, 361, 444, 1186, 502, 391, 444, 1192, 490, 1179, 535, 1204, 445, 1181, 506, 391, 481, 303, 506, 333, 507, 1177, 533, 306, 505, 1190, 520, 307, 508, 390, 446, 1177, 507, 393, 443, 397, 448, 335, 505, 395, 444, 1180, 504, 1180, 498, 341, 505, 1185, 492, 338, 536, 315, 492, 396, 444, 1187, 494, 398, 444, 338, 509, 341, 494, 1177, 537, 308, 508, 1170, 510, 1231, 443, 70082, 3477, 1568, 536, 310, 503, 1173, 507, 347, 487, 1180, 507, 345, 492, 1178, 508, 341, 499, 1175, 507, 340, 496, 1178, 508, 342, 499, 1175, 535, 1203, 444, 338, 509, 1232, 444, 338, 509, 1232, 444, 1186, 531, 1147, 503, 1234, 442, 345, 502, 338, 534, 361, 443, 1180, 537, 361, 444, 1180, 507, 392, 443, 343, 532, 1204, 444, 349, 496, 336, 536, 363, 481, 305, 500, 1235, 444, 1180, 537, 361, 443, 1180, 537, 315, 492, 348, 492, 340, 534, 1205, 444, 338, 509, 345, 495, 392, 444, 1211, 468, 399, 442, 1188, 495, 1181, 533};
SimpleIR ir(D1, D0);

// Body
Robot body(A0, D4, D5, D6, D7, D8, D9, D10, D11);

void setup()
{
    Serial.begin(115200);

    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
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
    audio_player.outputVolume(1.0); // Volume control (min:0.0 max:1.0)

    // Camera
    camera.begin();
    delay(100);

    // SD & USB
    Serial.print("Finding strage..");
    storage.wait_connect();
    Serial.println("done");
    Serial.println("Push UB0 to take a photo.");

    // delay(5000);
    // while (true) {
    //     body.demoWalk();
    // }
}

uint32_t last_millis = 0;

void take_photo()
{
    FILE *wp = fopen("/storage/lychee_camera.jpg", "w");
    if (wp != NULL)
    {
        size_t size = camera.createJpeg();
        fwrite(camera.getJpegAdr(), sizeof(char), (int)size, wp);
        fclose(wp);
        audio_player.play("/storage/camera-shutter2.wav");
    }
    else
    {
        Serial.print("Not found jpg");
        digitalWrite(LED_RED, HIGH);
        delay(1);
    }
    Serial.println("done to take a photo");
}

int receive_ir()
{
    int ret = 0;

    // 赤外線リモコン信号の受信
    // printf("IR signal waiting..\r\n");

    ret = ir.receive(ir_data, sizeof(ir_data) / sizeof(ir_data[0]));
    if (ret > 0)
    {
        for (int i = 0; i < ret; i++)
        {
            printf("%d, ", ir_data[i]);
        }
        printf("\r\n");
    }

    return ret;
}

void loop()
{

#ifdef ENABLE_CLOUD_FUNCTION
    //  Check command
    if (millis() - last_millis > 1000)
    {
        string ret = firebase.get_command();
        if (ret == "ERROR")
        {
            delay(5000);
        }
        else if (ret == "PHOTO")
        {
            digitalWrite(LED_GREEN, HIGH);
            take_photo();
            size_t size = camera.createJpeg();
            int ret = firebase.post_photo(camera.getJpegAdr(), size);
            if (ret == 0)
            {
                // const char message[] = "写真を撮りました";
                // firebase.post_message(message, strlen(message));
            }
            else
            {
                const char message[] = "通信に失敗しました";
                firebase.post_message(message, strlen(message));
            }
            digitalWrite(LED_GREEN, LOW);
        }
        else if (ret == "TV")
        {
            ir.transmit(ir_data_tv, sizeof(ir_data_tv) / sizeof(ir_data_tv[0]));
            // const char message[] = "テレビをつけました";
            // firebase.post_message(message, strlen(message));
        }
        else if (ret == "AIRCON")
        {
            ir.transmit(ir_data_aircon, sizeof(ir_data_aircon) / sizeof(ir_data_aircon[0]));
            // const char message[] = "エアコンをつけました";
            // firebase.post_message(message, strlen(message));
        }

        last_millis = millis();
    }
#endif

    //  IR remote control
    int ir_length = receive_ir();

    //  Camera
    if (digitalRead(USER_BUTTON0) == LOW)
    {
        digitalWrite(LED_GREEN, HIGH);
        take_photo();
#ifdef ENABLE_CLOUD_FUNCTION
        size_t size = camera.createJpeg();
        firebase.post_photo(camera.getJpegAdr(), size);
#endif
        while (digitalRead(USER_BUTTON0) == LOW)
            ;
        digitalWrite(LED_GREEN, LOW);
    }
}
