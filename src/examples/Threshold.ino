/* Threshold example with OpenCV */
/* Public Domain             */

#include <Arduino.h>
#include <Camera.h>
#include <opencv.hpp>
#include <DisplayApp.h>

// To monitor realtime on PC, you need DisplayApp on following site.
// Connect USB0(not for mbed interface) to your PC
// https://os.mbed.com/users/dkato/code/DisplayApp/

using namespace cv;

#define IMAGE_HW 640
#define IMAGE_VW 480

Camera camera(IMAGE_HW, IMAGE_VW);
DisplayApp display_app;
void setup() {
	Serial.begin(9600);
	camera.begin();
}

void loop() {
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

	Mat src;
	cvtColor(img_raw, src, COLOR_YUV2GRAY_YUY2); // convert YUV to GRAY
	threshold(src, src, 128, 255, THRESH_BINARY); //threshold

	size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, src.data, Camera::FORMAT_GRAY);
	display_app.SendJpeg(camera.getJpegAdr(), jpegSize);

}
