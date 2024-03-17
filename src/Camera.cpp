//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/Camera.h"


Camera::Camera(int cameraIndex)
{
     cv::VideoCapture cap(cameraIndex);
     vidCap = cap;
     currentFrame = cv::Mat();
     std::cout << "Initialized!" << std::endl;
}

Camera::~Camera() {
    vidCap.release();
    std::cout << "Released and closed!" << std::endl;

}

cv::VideoCapture Camera::getVideoCaptureDevice() {return vidCap;}

int Camera::getWidth() {
    return (int)vidCap.get(cv::CAP_PROP_FRAME_WIDTH);
}

int Camera::getHeight() {
    return (int)vidCap.get(cv::CAP_PROP_FRAME_HEIGHT);
}

cv::Mat& Camera::getCurrentFrame() {return currentFrame;}

void Camera::captureFrame() {
    getVideoCaptureDevice() >> currentFrame;
}

