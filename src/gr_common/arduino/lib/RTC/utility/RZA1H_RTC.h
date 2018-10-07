/***************************************************************************
 *
 * PURPOSE
 *   RTC(Real Time Clock) function module file.
 *
 * TARGET DEVICE
 *   RX63N
 *
 * AUTHOR
 *   Renesas Electronics Corp.
 *
 *
 ***************************************************************************
 * Copyright (C) 2014 Renesas Electronics. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 ***************************************************************************/
/**
 * @file  RZA1H_RTC.h
 * @brief RTC�i���A���E�^�C���E�N���b�N�j�֐� �w�b�_�t�@�C��
 *
 * Modified 27th May 2014 by Yuuki Okamiya from RL78duino.cpp
 */
#ifndef RZA1H_RTC_H
#define RZA1H_RTC_H

/***************************************************************************/
/*    Include MCU depend defines.                                          */
/***************************************************************************/
//#include "Arduino.h"


/***************************************************************************/
/*    Interrupt handler                                                    */
/***************************************************************************/


/***************************************************************************/
/*    Include Header Files                                                 */
/***************************************************************************/


/***************************************************************************/
/*    Macro Definitions                                                    */
/***************************************************************************/
#define RTC_WEEK_SUNDAY     0x00    //!< �j���ݒ�i���j���j
#define RTC_WEEK_MONDAY     0x01    //!< �j���ݒ�i���j���j
#define RTC_WEEK_TUESDAY    0x02    //!< �j���ݒ�i�Ηj���j
#define RTC_WEEK_WEDNESDAY  0x03    //!< �j���ݒ�i���j���j
#define RTC_WEEK_THURSDAY   0x04    //!< �j���ݒ�i�ؗj���j
#define RTC_WEEK_FRIDAY     0x05    //!< �j���ݒ�i���j���j
#define RTC_WEEK_SATURDAY   0x06    //!< �j���ݒ�i�y�j���j

#define RTC_ALARM_SUNDAY    0x00    //!< �A���[���j���ݒ�i���j���j
#define RTC_ALARM_MONDAY    0x01    //!< �A���[���j���ݒ�i���j���j
#define RTC_ALARM_TUESDAY   0x02    //!< �A���[���j���ݒ�i�Ηj���j
#define RTC_ALARM_WEDNESDAY 0x03    //!< �A���[���j���ݒ�i���j���j
#define RTC_ALARM_THURSDAY  0x04    //!< �A���[���j���ݒ�i�ؗj���j
#define RTC_ALARM_FRIDAY    0x05    //!< �A���[���j���ݒ�i���j���j
#define RTC_ALARM_SATURDAY  0x06    //!< �A���[���j���ݒ�i�y�j���j
#define RTC_ALARM_EVERYDAY  0x07    //!< �g�p����Ȃ��B�A���[�����W�X�^�ł�ENB��0����������


/***************************************************************************/
/*    Type  Definitions                                                    */
/***************************************************************************/
/*! RTC�\���� */
typedef struct tagRTC_TIMETYPE {
    unsigned short  year;   //!< �N
    unsigned char   mon;    //!< ��
    unsigned char   day;    //!< ��
    unsigned char   weekday;//!< �j��
    unsigned char   hour;   //!< ��
    unsigned char   min;    //!< ��
    unsigned char   second; //!< �b
} RTC_TIMETYPE;


/***************************************************************************/
/*    Function prototypes                                                  */
/***************************************************************************/
int rtc_init();
int rtc_deinit();
int rtc_set_time(RTC_TIMETYPE* time);
int rtc_get_time(RTC_TIMETYPE* time);
unsigned short rtc_get_year();
unsigned char rtc_get_mon();
unsigned char rtc_get_day();
unsigned char rtc_get_hour();
unsigned char rtc_get_min();
unsigned char rtc_get_second();
unsigned char rtc_get_weekday();
void rtc_attach_alarm_handler(void (*fFunction)(void));
int rtc_set_alarm_time(int hour, int min, int week_flag = RTC_ALARM_EVERYDAY);
void rtc_alarm_on();
void rtc_alarm_off();


/***************************************************************************/
/*    Global Variables                                                     */
/***************************************************************************/


/***************************************************************************/
/*    Local Variables                                                      */
/***************************************************************************/


/***************************************************************************/
/*    Global Routines                                                      */
/***************************************************************************/


/***************************************************************************/
/*    Local Routines                                                       */
/***************************************************************************/


/***************************************************************************/
/* End of module                                                           */
/***************************************************************************/

#endif /* RZA1H_RTC_H */
