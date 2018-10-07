#ifndef _WIFI_UTILITY_H_
#define _WIFI_UTILITY_H_

#include "NTPClient.h"
#include <RTC.h>

extern ESP32Interface wifi;
extern RTC rtc;

void setDateTimeFromNTP() {
    printf("Accessing NTP..");
    NTPClient ntp(&wifi);
    time_t tstamp = ntp.get_timestamp();
    if (tstamp < 0) {
        printf("Cannot get time from NTP.");
        rtc.setDateTime(2018, 4, 19, 16, 40, 0);
    } else {
        printf("done\r\n");
        struct tm tim = *localtime(&tstamp);
        rtc.setDateTime(tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);
    }
}

void panic() {
    // 異常終了した場合、LED1を高速で点滅する
    char led = 1;
    while (1) {
        digitalWrite(LED_RED, led);
        led = (1 - led);
        delay(100);
    }
}

void wifiSetup() {
    // WiFi
    rtc.begin();
    printf("Connecting...\r\n");
    int ret = wifi.connect(WIFI_SSID, WIFI_PW, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
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
}
#endif
