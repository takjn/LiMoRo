/*
 LCD.h - LCD library for GR-LYCHEE and GR-PEACH
 Copyright (c) 2018 Renesas Electronics.  All right reserved.

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

#ifndef LCD_H_
#define LCD_H_

#include "mbed.h"
#if MBED_CONF_APP_LCD
  // lcd-type
  #define GR_PEACH_4_3INCH_SHIELD     0x0000
  #define GR_PEACH_7_1INCH_SHIELD     0x0001
  #define GR_PEACH_RSK_TFT            0x0002
  #define GR_PEACH_DISPLAY_SHIELD     0x0100
  #define GR_LYCHEE_TF043HV001A0      0x1000
  #define GR_LYCHEE_ATM0430D25        0x1001
  #define GR_LYCHEE_FG040346DSSWBG03  0x1002

  #define GR_LYCHEE_LCD               GR_LYCHEE_TF043HV001A0

  #ifndef MBED_CONF_APP_LCD_TYPE
    #if defined(TARGET_RZ_A1H)
      #define MBED_CONF_APP_LCD_TYPE    GR_PEACH_4_3INCH_SHIELD
    #elif defined(TARGET_GR_LYCHEE)
      #define MBED_CONF_APP_LCD_TYPE    GR_LYCHEE_LCD
    #endif
  #endif

  #if defined(TARGET_RZ_A1H) && ((MBED_CONF_APP_LCD_TYPE & 0xFF00) != 0x0000)
    #error "MBED_CONF_APP_LCD_TYPE is not supported in this target."
  #endif
  #if defined(TARGET_GR_LYCHEE) && ((MBED_CONF_APP_LCD_TYPE & 0xFF00) != 0x1000)
    #error "MBED_CONF_APP_LCD_TYPE is not supported in this target."
  #endif

  #if MBED_CONF_APP_LCD_TYPE == GR_PEACH_4_3INCH_SHIELD
    #include "LCD_shield_config_4_3inch.h"
  #elif MBED_CONF_APP_LCD_TYPE == GR_PEACH_7_1INCH_SHIELD
    #include "LCD_shield_config_7_1inch.h"
  #elif MBED_CONF_APP_LCD_TYPE == GR_PEACH_RSK_TFT
    #include "LCD_shield_config_RSK_TFT.h"
  #elif MBED_CONF_APP_LCD_TYPE == GR_PEACH_DISPLAY_SHIELD
    #include "Display_shield_config.h"
  #elif MBED_CONF_APP_LCD_TYPE == GR_LYCHEE_TF043HV001A0
    #include "LCD_config_TF043HV001A0.h"
  #elif MBED_CONF_APP_LCD_TYPE == GR_LYCHEE_ATM0430D25
    #include "LCD_config_ATM0430D25.h"
  #elif MBED_CONF_APP_LCD_TYPE == GR_LYCHEE_FG040346DSSWBG03
    #include "LCD_config_FG040346DSSWBG03.h"
  #else
    #error "No lcd chosen. Please add 'config.lcd-type.value' to your mbed_app.json (see README.md for more information)."
  #endif
#endif


class LCD {
public:
	enum {
		FORMAT_YUV422 = 0,
		FORMAT_RGB888,
	};

	LCD(uint16_t width = LCD_PIXEL_WIDTH, uint16_t height = LCD_PIXEL_HEIGHT, uint8_t format = FORMAT_YUV422, float backlight = 1.0f);
	~LCD();
	void begin(uint8_t* buf, uint16_t image_width, uint16_t image_height);
	void stop();
	void restart();
	void restart(uint8_t* buf);
	void clear();
	uint16_t getWidth();
	uint16_t getHeight();
	void BGR2YUV(uint8_t* inbuf, uint8_t* outbuf, int width, int height);
	void GRAY2YUV(uint8_t* inbuf, uint8_t* outbuf, int width, int height);
	void setBacklight(float backlight);
	float getBacklight();

private:
	uint16_t _width;
	uint16_t _height;
	uint8_t _format;
	uint8_t* _buf;
	float _backlight;
};

#endif /* Camera_h */
