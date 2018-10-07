/* Multiple function example with OpenCV */
/* Public Domain             */

#include <Arduino.h>
#include <Camera.h>
#include <opencv.hpp>
#include <DisplayApp.h>

// To monitor realtime on PC, you need DisplayApp on following site.
// Connect USB0(not for mbed interface) to your PC
// https://os.mbed.com/users/dkato/code/DisplayApp/

using namespace cv;

#define IMAGE_HW 320
#define IMAGE_VW 240

Camera camera(IMAGE_HW, IMAGE_VW);
DisplayApp display_app;

#define DEMO_NUMBER 4
#define LOOP_WAITTIME 16
static uint8_t g_mode = 0;

void ub0_interrupt() {
	while (digitalRead(PIN_SW0) == LOW)
		;
	g_mode++;
	if (g_mode >= DEMO_NUMBER)
		g_mode = 0;
}

void setup() {
	Serial.begin(9600);
	pinMode(PIN_SW0, INPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	digitalWrite(PIN_LED_RED, LOW);
	camera.begin();
	attachInterrupt(4, ub0_interrupt, FALLING);
}

void loop() {
	static unsigned long loop_time = millis();

	while ((millis() - loop_time) < LOOP_WAITTIME)
		; //
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

	if (g_mode == 0) {
		display_app.SendJpeg(camera.getJpegAdr(), (int) camera.createJpeg());
		loop_time = millis();
	} else if (g_mode == 1) {
		Mat src, dst;
		cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY
		Canny(src, dst, 50, 150); // Canny
		size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, dst.data,
				Camera::FORMAT_GRAY);
		display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
		loop_time = millis();
	} else if (g_mode == 2) {
		static int x = 0, y = 0, ax = 10, ay = 10;
		Scalar red(0, 0, 255), green(0, 255, 0), blue(255, 0, 0);
		Scalar yellow = red + green;
		Scalar white = Scalar::all(255);
		Scalar pink = Scalar(154, 51, 255);

		Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

		Mat src;
		cvtColor(img_raw, src, COLOR_YUV2BGR_YUYV); //covert YUV to RGB

		x += ax;
		y += ay;
		if (x > (src.cols - 10) || x < 10) {
			ax *= -1;
		}
		if (y > (src.rows - 10) || y < 10) {
			ay *= -1;
		}

		line(src, Point(10, 10), Point(src.cols - 10, 10), blue, 3, LINE_AA); //Line
		line(src, Point(10, src.rows - 10), Point(src.cols - 10, src.rows - 10),
				blue, 3, LINE_AA); //Line
		rectangle(src, Point(10, 30), Point(src.cols - 10, 60), white, FILLED);
		putText(src, "Gadget Renesas", Point(15, 55), FONT_HERSHEY_COMPLEX, 1,
				pink, 2);
		circle(src, Point(x, y), 10, yellow, FILLED);

		stringstream ss;
		ss << x << ", " << y;
		putText(src, ss.str(), Point(10, src.rows - 20),
				FONT_HERSHEY_SCRIPT_SIMPLEX, 1, white, 1);

		size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, src.data,
				Camera::FORMAT_RGB888);
		display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
		loop_time = millis();
	} else if (g_mode == 3) {
		Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());
		Mat src, diff, srcFloat, dstFloat, diffFloat;
		dstFloat.create(IMAGE_VW, IMAGE_HW, CV_32FC1);
		dstFloat.setTo(0.0);

		while (g_mode == 3) {
			cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY

			src.convertTo(srcFloat, CV_32FC1, 1 / 255.0);
			addWeighted(srcFloat, 0.01, dstFloat, 0.99, 0, dstFloat, -1);
			absdiff(srcFloat, dstFloat, diffFloat);
			diffFloat.convertTo(diff, CV_8UC1, 255.0);

			size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, diff.data,
					Camera::FORMAT_GRAY);
			display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
			loop_time = millis();
		}

	}

}
