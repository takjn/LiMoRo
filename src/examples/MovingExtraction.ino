/* Moving Extraction example with OpenCV */
/* Public Domain             */

#include <Arduino.h>
#include <Camera.h>
#include <opencv.hpp>
#include <DisplayApp.h>

// To monitor realtime on PC, you need DisplayApp on following site.
// Connect USB0(not for mbed interface) to your PC
// https://os.mbed.com/users/dkato/code/DisplayApp/

#define IMAGE_HW 320
#define IMAGE_VW 240
using namespace cv;

static Camera camera(IMAGE_HW, IMAGE_VW);
static DisplayApp display_app;

void setup() {
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);

	// Camera
	camera.begin();

}

void loop() {
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());
	Mat src, diff, srcFloat, dstFloat, diffFloat;
	dstFloat.create(IMAGE_VW, IMAGE_HW, CV_32FC1);
	dstFloat.setTo(0.0);

	while(1){
		cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY

		src.convertTo(srcFloat, CV_32FC1, 1 / 255.0);
		addWeighted(srcFloat, 0.01, dstFloat, 0.99, 0, dstFloat, -1);
		absdiff(srcFloat, dstFloat, diffFloat);
		diffFloat.convertTo(diff, CV_8UC1, 255.0);

		size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, diff.data,
				Camera::FORMAT_GRAY);
		display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
	}

}
