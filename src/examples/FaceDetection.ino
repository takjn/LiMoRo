/* Face detection example with OpenCV */
/* Public Domain             */

#include <Arduino.h>
#include <Camera.h>
#include <opencv.hpp>
#include <DisplayApp.h>

// To monitor realtime on PC, you need DisplayApp on following site.
// Connect USB0(not for mbed interface) to your PC
// https://os.mbed.com/users/dkato/code/DisplayApp/
#include "mbed.h"
#include "SdUsbConnect.h"

#define IMAGE_HW 320
#define IMAGE_VW 240
using namespace cv;

/* FACE DETECTOR Parameters */
#define DETECTOR_SCALE_FACTOR (2)
#define DETECTOR_MIN_NEIGHBOR (4)
#define DETECTOR_MIN_SIZE     (30)
#define FACE_DETECTOR_MODEL     "/storage/lbpcascade_frontalface.xml"


static Camera camera(IMAGE_HW, IMAGE_VW);
static DisplayApp  display_app;
static CascadeClassifier detector_classifier;

void setup() {
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);

    // Camera
    camera.begin();

    // SD & USB
    SdUsbConnect storage("storage");
    storage.wait_connect();

    // Load the cascade classifier file
    detector_classifier.load(FACE_DETECTOR_MODEL);

    if (detector_classifier.empty()) {
    	digitalWrite(PIN_LED_RED, HIGH); // Error
        CV_Assert(0);
        mbed_die();
    }
}

void loop(){
	Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());

	Mat src;
	cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY

    // Detect a face in the frame
    Rect face_roi;
    if (detector_classifier.empty()) {
    	digitalWrite(PIN_LED_RED, HIGH); // Error
    }

    // Perform detected the biggest face
    std::vector<Rect> rect_faces;
    detector_classifier.detectMultiScale(src, rect_faces,
                                         DETECTOR_SCALE_FACTOR,
                                         DETECTOR_MIN_NEIGHBOR,
                                         CASCADE_FIND_BIGGEST_OBJECT,
                                         Size(DETECTOR_MIN_SIZE, DETECTOR_MIN_SIZE));

    if (rect_faces.size() > 0) {
        // A face is detected
    	face_roi = rect_faces[0];
    } else {
        // No face is detected, set an invalid rectangle
    	face_roi.x = -1;
    	face_roi.y = -1;
    	face_roi.width = -1;
    	face_roi.height = -1;
    }

    if (face_roi.width > 0 && face_roi.height > 0) {   // A face is detected
        digitalWrite(PIN_LED_GREEN, HIGH);
        printf("Detected a face X:%d Y:%d W:%d H:%d\n",face_roi.x, face_roi.y, face_roi.width, face_roi.height);
        digitalWrite(PIN_LED_GREEN, LOW);
    } else {
    }

    Mat dst;
	cvtColor(img_raw, dst, COLOR_YUV2BGR_YUYV); //covert from YUV to BGR
	Scalar red(0, 0, 255), green(0, 255, 0), blue(255, 0, 0);

	rectangle(dst, Point(face_roi.x, face_roi.y), Point(face_roi.x + face_roi.width, face_roi.y + face_roi.height), red, 2);
	size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, dst.data, Camera::FORMAT_RGB888);
	display_app.SendJpeg(camera.getJpegAdr(), jpegSize);
}
