/* Canny example with OpenCV */
/* Public Domain             */

#include <Arduino.h>
#include <Camera.h>
#include <opencv.hpp>

// To monitor realtime on PC, you need DisplayApp on following site.
// Connect USB0(not for mbed interface) to your PC
// https://os.mbed.com/users/dkato/code/DisplayApp/
#define DBG_PCMONITOR
// Insert SD or USB storage to a board.
// When you press BUTTON0, a photo will be captured.
#define DBG_CAPTURE

#ifdef DBG_PCMONITOR
#include <DisplayApp.h>
DisplayApp display_app;
#endif
#ifdef DBG_CAPTURE
#include <SD.h>
#include <SPI.h>
#include <RTC.h>
RTC rtc; // for time stamp
void dateTime(uint16_t* date, uint16_t* time) {
	int year, mon, day, hour, min, sec, week;
	rtc.getDateTime(year, mon, day, hour, min, sec, week);
	*date = FAT_DATE(year, mon, day);
	*time = FAT_TIME(hour, min, sec);
}
#endif

using namespace cv;

#define IMAGE_HW 320
#define IMAGE_VW 240

Camera camera(IMAGE_HW, IMAGE_VW);

void setup() {
	Serial.begin(9600);
	pinMode(PIN_USER_BUTTON0, INPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	pinMode(PIN_LED_GREEN, OUTPUT);
	camera.begin();
#ifdef DBG_CAPTURE
	rtc.begin();

	rtc.setDateTime(2017, 10, 14, 15, 40, 0);
	SdFile::dateTimeCallback(&dateTime);

	if (!SD.begin()) {
		Serial.println("Card failed, or not present.");
		pinMode(PIN_LED_RED, OUTPUT);
		digitalWrite(PIN_LED_RED, HIGH);
		while (1)
			;
	} else {
		Serial.println("Card founded.");
	}
#endif
}

void loop() {
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

	static uint8_t mode = 0;
	if (Serial.available()) {
		char c = Serial.read();
		if (c == 'm') {
			mode++;
			if (mode > 2)
				mode = 0;
			Serial.print("mode:");
			Serial.println(mode);
		}
	}

	Mat src, dst;
	size_t jpegSize = 0;
	if (mode == 0) {
		cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY
		Canny(src, dst, 50, 150); // Canny
		jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, dst.data,
				Camera::FORMAT_GRAY);
	} else if (mode == 1) {
		cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY
		jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, src.data,
				Camera::FORMAT_GRAY);
	} else if (mode == 2) {
		jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, img_raw.data);
	}
#ifdef DBG_PCMONITOR
	display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
#endif
#ifdef DBG_CAPTURE
	static int count = 0;
	if (digitalRead(PIN_SW) == 0) {
		char filename[13];
		sprintf(filename, "image%d.jpg", count);
		File file = SD.open(filename, FILE_WRITE);
		if (file) {
			digitalWrite(PIN_LED_GREEN, HIGH);
			uint8_t* adr = camera.getJpegAdr();
			for (size_t i = 0; i < jpegSize; i++) {
				file.write(*adr);
				adr++;
			}
			file.close();
			Serial.print("Saved a picture as ");
			Serial.println(filename);
			digitalWrite(PIN_LED_GREEN, LOW);
			count++;
		} else {
			Serial.println("Failed to open file.");
			digitalWrite(PIN_LED_RED, HIGH);
			while (1)
				;
		}
	}
#endif
	delay(10);
}
