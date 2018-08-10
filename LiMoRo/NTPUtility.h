#ifndef _NTP_UTILITY_H_
#define _NTP_UTILITY_H_

#include <NTPClient.h>
#include <RTC.h>

extern ESP32Interface wifi;
RTC rtc;

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