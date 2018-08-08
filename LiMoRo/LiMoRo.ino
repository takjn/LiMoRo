// #define ENABLE_CLOUD_FUNCTION
#define WIFI_SSID "xxxx"
#define WIFI_PW "yyyy"
#define SERVER_URL_COMMAND "https://example.com/command"
#define SERVER_URL_PHOTO "https://example.com/photo"
#define SERVER_URL_MESSAGE "https://example.com/message"

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
#include <NTPClient.h>
#include <RTC.h>
#include "Firebase.h"

ESP32Interface wifi;
RTC rtc;
Firebase firebase(&wifi, SERVER_URL_COMMAND, SERVER_URL_PHOTO, SERVER_URL_MESSAGE);
#endif

#define PIN_SERVO0 9
#define PIN_SERVO1 10
#define PIN_LIGHT 13

#define INTERVAL 200

Camera camera(320, 240);
SdUsbConnect storage("storage");

char g_servo0_inc = 10;
char g_servo1_inc = -10;
unsigned char g_servo0_pos = 90;
unsigned char g_servo1_pos = 90;
Servo servo0;
Servo servo1;

// unsigned int ir_data[1024];
unsigned int ir_data[] = {3479, 1621, 444, 340, 534, 1203, 446, 339, 505, 1233, 446, 339, 506, 1231, 445, 341, 503, 1234, 444, 342, 532, 1205, 442, 339, 508, 1233, 443, 1211, 506, 361, 444, 1186, 502, 391, 444, 1192, 490, 1179, 535, 1204, 445, 1181, 506, 391, 481, 303, 506, 333, 507, 1177, 533, 306, 505, 1190, 520, 307, 508, 390, 446, 1177, 507, 393, 443, 397, 448, 335, 505, 395, 444, 1180, 504, 1180, 498, 341, 505, 1185, 492, 338, 536, 315, 492, 396, 444, 1187, 494, 398, 444, 338, 509, 341, 494, 1177, 537, 308, 508, 1170, 510, 1231, 443, 70082, 3477, 1568, 536, 310, 503, 1173, 507, 347, 487, 1180, 507, 345, 492, 1178, 508, 341, 499, 1175, 507, 340, 496, 1178, 508, 342, 499, 1175, 535, 1203, 444, 338, 509, 1232, 444, 338, 509, 1232, 444, 1186, 531, 1147, 503, 1234, 442, 345, 502, 338, 534, 361, 443, 1180, 537, 361, 444, 1180, 507, 392, 443, 343, 532, 1204, 444, 349, 496, 336, 536, 363, 481, 305, 500, 1235, 444, 1180, 537, 361, 443, 1180, 537, 315, 492, 348, 492, 340, 534, 1205, 444, 338, 509, 345, 495, 392, 444, 1211, 468, 399, 442, 1188, 495, 1181, 533};

SimpleIR ir(D1, D0);

#ifdef ENABLE_CLOUD_FUNCTION
void setDateTimeFromNTP()
{
    printf("Accessing NTP..");
    NTPClient ntp(&wifi);
    time_t tstamp = ntp.get_timestamp();
    if (tstamp < 0)
    {
        printf("Cannot get time from NTP.");
        rtc.setDateTime(2018, 4, 19, 16, 40, 0);
    }
    else
    {
        printf("done\r\n");
        struct tm tim = *localtime(&tstamp);
        rtc.setDateTime(tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);
    }
}
#endif

void panic()
{
    // 異常終了した場合、LED1を高速で点滅する
    char led = 1;
    while (1)
    {
        digitalWrite(LED_RED, led);
        led = (1 - led);
        delay(100);
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(USER_BUTTON0, INPUT);
    pinMode(USER_BUTTON1, INPUT);

    pinMode(PIN_SERVO0, OUTPUT);
    pinMode(PIN_SERVO1, OUTPUT);
    pinMode(PIN_LIGHT, OUTPUT);

    // Servo
    servo0.attach(PIN_SERVO0);
    servo1.attach(PIN_SERVO1);
    servo0.write(g_servo0_pos);
    servo1.write(g_servo1_pos);

#ifdef ENABLE_CLOUD_FUNCTION
    // WiFi
    rtc.begin();
    printf("Connecting...\r\n");
    int ret = wifi.connect(WIFI_SSID, WIFI_PW, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0)
    {
        printf("Connection error\r\n");
        panic();
    }
    printf("Success\r\n\r\n");
    printf("MAC: %s\r\n", wifi.get_mac_address());
    printf("IP: %s\r\n", wifi.get_ip_address());
    printf("Netmask: %s\r\n", wifi.get_netmask());
    printf("Gateway: %s\r\n", wifi.get_gateway());
    printf("RSSI: %d\r\n\r\n", wifi.get_rssi());

    // NTP
    setDateTimeFromNTP();
#endif

    // 赤外線リモコン信号の受信
    if (sizeof(ir_data) > 1023)
    {
        int led = 0;
        while (1)
        {
            printf("IR signal waiting..\r\n");
            digitalWrite(LED_GREEN, led);
            led = (1 - led);

            int ret = ir.receive(ir_data, sizeof(ir_data) / sizeof(ir_data[0]));
            if (ret > 0)
            {
                for (int i = 0; i < ret; i++)
                {
                    printf("%d, ", ir_data[i]);
                }
                printf("\r\n");
                break;
            }
        }
        digitalWrite(LED_GREEN, LOW);
    }

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
        else if (ret == "ON")
        {
            digitalWrite(PIN_LIGHT, HIGH);
        }
        else if (ret == "OFF")
        {
            digitalWrite(PIN_LIGHT, LOW);
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
            ir.transmit(ir_data, sizeof(ir_data) / sizeof(ir_data[0]));
            // const char message[] = "テレビをつけました";
            // firebase.post_message(message, strlen(message));
        }

        last_millis = millis();
    }
#endif

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

    //  Servo
    if (digitalRead(USER_BUTTON1) == LOW)
    {
        g_servo0_pos = g_servo0_pos + g_servo0_inc;
        g_servo1_pos = g_servo1_pos + g_servo1_inc;
        servo0.write(g_servo0_pos);
        servo1.write(g_servo1_pos);
        if (g_servo0_pos == 170 || g_servo0_pos == 10)
        {
            g_servo0_inc = g_servo0_inc * -1;
        }
        if (g_servo1_pos == 170 || g_servo1_pos == 10)
        {
            g_servo1_inc = g_servo1_inc * -1;
        }
        delay(INTERVAL);
    }
}
