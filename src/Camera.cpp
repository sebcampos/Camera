//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/Camera.h"


Camera::Camera(int cameraIndex)
{
     VideoCapture cap(cameraIndex);
     vidCap = cap;
     currentFrame = Mat();
     cout << "Initialized!" << endl;
}

Camera::~Camera() {
    vidCap.release();
    cout << "Released and closed!" << endl;

}

VideoCapture Camera::getVideoCaptureDevice() {return vidCap;}

int Camera::getWidth() {
    return (int)vidCap.get(CAP_PROP_FRAME_WIDTH);
}

int Camera::getHeight() {
    return (int)vidCap.get(CAP_PROP_FRAME_HEIGHT);
}

Mat& Camera::getCurrentFrame() {return currentFrame;}

void Camera::captureFrame() {
    getVideoCaptureDevice() >> currentFrame;
}

