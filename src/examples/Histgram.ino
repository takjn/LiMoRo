/* Histogram example with OpenCV */
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
	  vector<Scalar> cScalar{ Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255) };
	  Mat hist(Size(256, 256), CV_8UC3);

	while(1){
		Mat src;
		cvtColor(img_raw, src, COLOR_YUV2BGR_YUYV); //covert from YUV to RGB

	    vector<Mat> ch(3);
	    split(src, ch);

	    vector<int> size{ 256 };
	    vector<float> range{ 0, 256 };
	    hist.setTo(255);
	    for (int c = 0; c < 3; c++) {
	      vector<int> channel{ c };
	      Mat bins;
	      calcHist(ch, channel, noArray(), bins, size, range);
	      Point prev = Point(0, 255);
	      for (int x = 0; x < 256; x++) {
	        Point current = Point(x, 256 - (int)(bins.at<float>(x) / 50));
	        line(hist, prev, current, cScalar[c]);
	        prev = current;
	      }
	    }

		size_t jpegSize = camera.createJpeg(hist.cols, hist.rows, hist.data,
				Camera::FORMAT_RGB888);
		display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
	}

}
