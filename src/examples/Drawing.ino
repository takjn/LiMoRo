/* Drawing example with OpenCV */
/* Public Domain               */

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
	static int x = 0, y = 0, ax = 10, ay = 10;
	static long time = millis();
	Scalar red(0, 0, 255), green(0, 255, 0), blue(255, 0, 0);
	Scalar yellow = red + green;
	Scalar white = Scalar::all(255);
	Scalar pink = Scalar(154, 51, 255);

	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

	Mat src;
	cvtColor(img_raw, src, COLOR_YUV2BGR_YUYV); //covert YUV to RGB

	if((millis() - time) > 50){
		x+=ax; y+=ay;
		if(x > (src.cols - 10) || x < 10){
			ax*=-1;
		}
		if(y > (src.rows - 10) || y < 10){
			ay*=-1;
		}
	}

	line(src, Point(10, 10), Point(src.cols - 10, 10), blue, 3, LINE_AA); //Line
	line(src, Point(10, src.rows - 10), Point(src.cols - 10, src.rows - 10), blue, 3, LINE_AA); //Line
	rectangle(src, Point(10, 30), Point(src.cols - 10, 60), white, FILLED);
	putText(src, "Gadget Renesas", Point(15, 55), FONT_HERSHEY_COMPLEX, 1, pink, 2);
	circle(src, Point(x, y), 10, yellow, FILLED);

	stringstream ss;
	ss << x << ", " << y;
	putText(src, ss.str(), Point(10, src.rows - 20), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, white, 1);

	size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, src.data,
			Camera::FORMAT_RGB888);
	display_app.SendJpeg(camera.getJpegAdr(), jpegSize);

}
