#include <Arduino.h>
#include <Camera.h>
#include <LCD.h>
#include <opencv.hpp>

#define IMAGE_HW 480
#define IMAGE_VW 272
Camera camera(IMAGE_HW, IMAGE_VW);
LCD lcd(IMAGE_HW, IMAGE_VW);
static uint8_t work_buf[BYTE_PER_PIXEL_YUV * IMAGE_HW * IMAGE_VW];

using namespace cv;
void ub0_interrupt() {
	lcd.stop();
	digitalWrite(PIN_LED_RED, HIGH);
}

void ub1_interrupt() {
	lcd.restart();
	digitalWrite(PIN_LED_RED, LOW);
}

void setup() {
	camera.begin();
	lcd.begin(work_buf, IMAGE_HW, IMAGE_VW);
	lcd.clear();
	pinMode(PIN_SW0, INPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	attachInterrupt(4, ub0_interrupt, FALLING);
	attachInterrupt(3, ub1_interrupt, FALLING);
}

void loop() {
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());
	Mat src, dst;
	cvtColor(img_raw, src, COLOR_YUV2GRAY_YUY2); // convert YUV to GRAY
	Canny(src, dst, 50, 150); // Canny
	lcd.GRAY2YUV(dst.data, work_buf, IMAGE_HW, IMAGE_VW);
	delay(10);
}

