/*
 LCD.cpp - LCD library for GR-LYCHEE and GR-PEACH.
 Copyright (c) 2017 Renesas Electronics.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "mbed.h"
#include "rtos.h"
#include "DisplayBace.h"
#include "LCD.h"
#include "Camera.h"
#include "dcache-control.h"

#if defined(TARGET_GR_LYCHEE)
static DigitalOut lcd_pwon(P5_12);
static DigitalOut lcd_sd(P7_5);
static PwmOut lcd_cntrst(P3_12);
#elif defined(TARGET_RZ_A1H)
static DigitalOut lcd_pwon(P7_15);
static DigitalOut lcd_blon(P8_1);
static DigitalOut lcd_cntrst(P8_15);
#endif

static DisplayBase Display;

LCD::LCD(uint16_t width, uint16_t height, uint8_t format, float backlight){
	_width = width;
	_height = height;
	_format = format;
	_backlight = backlight;
}

LCD::~LCD(){

}

/* Starts the camera */
void LCD::begin(uint8_t* buf, uint16_t image_width, uint16_t image_height)
{
	_buf = buf;
#if defined(TARGET_GR_LYCHEE)
    PinName lcd_pin[28] = {
        /* data pin */
        P6_15, P6_14, P6_13, P6_12, P6_11, P6_10, P6_9, P6_8, P6_7, P6_6, P6_5, P6_4,
        P6_3, P6_2, P6_1, P6_0, P3_7, P3_6, P3_5, P3_4, P3_3, P3_2, P3_1, P3_0, P5_2,
        P5_1, P5_0, P7_4,
    };

    lcd_pwon = 0;
    lcd_sd = 0;
    lcd_cntrst.period_us(500);
    Thread::wait(100);
    lcd_pwon = 1;
    Thread::wait(1);
    lcd_sd = 1;
    Display.Graphics_Lcd_Port_Init(lcd_pin, 28);
#elif defined(TARGET_RZ_A1H)
    PinName lvds_pin[8] = {
        /* data pin */
        P5_7, P5_6, P5_5, P5_4, P5_3, P5_2, P5_1, P5_0
    };

    lcd_pwon = 0;
    lcd_blon = 0;
    Thread::wait(100);
    lcd_pwon = 1;
    lcd_blon = 1;

    Display.Graphics_Lvds_Port_Init(lvds_pin, 8);
#endif
    Display.Graphics_init(&LcdCfgTbl_LCD_shield);
    Display.Graphics_Video_init(DisplayBase::INPUT_SEL_EXT, &Camera::ext_in_config);

    unsigned int stride;
    DisplayBase::graphics_format_t format;
    DisplayBase::wr_rd_swa_t wrrd;
    if(_format == FORMAT_RGB888){
    	stride = image_width * 3;
    	format = DisplayBase::GRAPHICS_FORMAT_RGB888;
    	wrrd = DisplayBase::WR_RD_WRSWA_32BIT;
    } else {
    	stride = image_width * 2;
    	format = DisplayBase::GRAPHICS_FORMAT_YCBCR422;
    	wrrd = DisplayBase::WR_RD_WRSWA_32_16BIT;
    }
    DisplayBase::rect_t rect;
    rect.hs = 0;
    if(image_width < _width){
        rect.hw = image_width;
    } else {
        rect.hw = _width;
    }
    rect.vs = 0;
    if(image_height < _height){
        rect.vw = image_height;
    } else {
        rect.vw = _height;
    }
    Display.Graphics_Read_Setting(
    		DisplayBase::GRAPHICS_LAYER_0,
			(void *) buf,
			stride,
			format,
			wrrd,
			&rect);

    Display.Graphics_Start(DisplayBase::GRAPHICS_LAYER_0);

    Thread::wait(50);
    lcd_cntrst = _backlight;
}

void LCD::stop(){
	Display.Graphics_Stop(DisplayBase::GRAPHICS_LAYER_0);
    Thread::wait(50);
    lcd_cntrst = 0;
}

void LCD::restart(){
    Display.Graphics_Start(DisplayBase::GRAPHICS_LAYER_0);
    Thread::wait(50);
    lcd_cntrst = _backlight;
}

void LCD::restart(uint8_t* buf){
	Display.Graphics_Read_Change(DisplayBase::GRAPHICS_LAYER_0, buf);
	restart();
}

void LCD::clear(){
	for (uint32_t i = 0;
			i < (uint32_t) (BYTE_PER_PIXEL_YUV * _width * _height); i += 2) {
		_buf[i + 0] = 0x10;
		_buf[i + 1] = 0x80;
	}

}

uint16_t LCD::getWidth(){
	return _width;
}
uint16_t LCD::getHeight(){
	return _height;
}

void LCD::BGR2YUV(uint8_t* inbuf, uint8_t* outbuf, int width, int height){
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j+=2) { // processing 2 pixels
            uint8_t r0,g0,b0,r1,g1,b1,y0,y1,u,v;
            r1 = inbuf[(i * width * BYTE_PER_PIXEL_RGB)
                    + BYTE_PER_PIXEL_RGB * j + 5];
            g1 = inbuf[(i * width * BYTE_PER_PIXEL_RGB)
                    + BYTE_PER_PIXEL_RGB * j + 4];
            b1 = inbuf[(i * width * BYTE_PER_PIXEL_RGB)
                    + BYTE_PER_PIXEL_RGB * j + 3];
            r0 = inbuf[(i * width * BYTE_PER_PIXEL_RGB)
                    + BYTE_PER_PIXEL_RGB * j + 2];
            g0 = inbuf[(i * width * BYTE_PER_PIXEL_RGB)
                    + BYTE_PER_PIXEL_RGB * j + 1];
            b0 = inbuf[(i * width * BYTE_PER_PIXEL_RGB)
                    + BYTE_PER_PIXEL_RGB * j + 0];

            y0 = round( 0.256788 * r0 + 0.504129 * g0 + 0.097906 * b0) +  16;
            y1 = round( 0.256788 * r1 + 0.504129 * g1 + 0.097906 * b1) +  16;
            if(r0!=g0){ // it's not gray that means color line
                u = round(-0.148223 * r0 - 0.290993 * g0 + 0.439216 * b0) + 128;
                v = round( 0.439216 * r0 - 0.367788 * g0 - 0.071427 * b0) + 128;
            } else if (r1!=g1){ // it's not gray that means color line
                u = round(-0.148223 * r1 - 0.290993 * g1 + 0.439216 * b1) + 128;
                v = round( 0.439216 * r1 - 0.367788 * g1 - 0.071427 * b1) + 128;
            } else { // it's gray.
                u = round(-0.148223 * r0 - 0.290993 * g0 + 0.439216 * b0) + 128;
                v = round( 0.439216 * r0 - 0.367788 * g0 - 0.071427 * b0) + 128;
            }

            outbuf[(i * width * BYTE_PER_PIXEL_YUV)
                    + BYTE_PER_PIXEL_YUV * j + 0] = y0;
            outbuf[(i * width * BYTE_PER_PIXEL_YUV)
                    + BYTE_PER_PIXEL_YUV * j + 1] = u;
            outbuf[(i * width * BYTE_PER_PIXEL_YUV)
                    + BYTE_PER_PIXEL_YUV * j + 2] = y1;
            outbuf[(i * width * BYTE_PER_PIXEL_YUV)
                    + BYTE_PER_PIXEL_YUV * j + 3] = v;
        }
    }
    dcache_clean(outbuf, 2 * width * height);
	return;
}

void LCD::GRAY2YUV(uint8_t* inbuf, uint8_t* outbuf, int width, int height){
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			outbuf[(i * width * BYTE_PER_PIXEL_YUV)
					+ BYTE_PER_PIXEL_YUV * j] = inbuf[i * width + j];
		}
	}
    dcache_clean(outbuf, 2 * width * height);
}

void LCD::setBacklight(float backlight){
	if(backlight > 1.0f){
		_backlight = 1.0f;
	} else if (backlight < 0.0f){
		_backlight = 0.0f;
	} else {
		_backlight = backlight;
	}

    lcd_cntrst = _backlight;
}

float LCD::getBacklight(){
	return _backlight;
}
