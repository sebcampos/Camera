//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/Camera.h"


Camera::Camera(int cameraIndex)
{
     VideoCapture cap(cameraIndex);
     vidCap = cap;
     currentFrame = new Mat();
     cout << "Initalized!" << endl;
}

Camera::~Camera() {
    vidCap.release();
    delete currentFrame;
    cout << "Realeased and closed!" << endl;

}

VideoCapture Camera::getVideoCaptureDevice() {return vidCap;}

void Camera::setCurrentFrame(Mat* newFrame) {
    currentFrame = newFrame;
}

Mat Camera::getCurrentFrame() {return *currentFrame;}

void Camera::captureFrame() {
    getVideoCaptureDevice() >> *currentFrame;
}

