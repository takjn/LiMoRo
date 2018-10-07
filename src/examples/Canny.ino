/* Canny example with OpenCV */
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

void setup() {
	Serial.begin(9600);
	camera.begin();
}

void loop() {
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

	Mat src, dst;
	cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY
	Canny(src, dst, 50, 150); // Canny

	size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, dst.data, Camera::FORMAT_GRAY);
	display_app.SendJpeg(camera.getJpegAdr(), jpegSize);

}
