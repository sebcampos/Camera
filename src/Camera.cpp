//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/Camera.h"


Camera::Camera(int cameraIndex)
{
     VideoCapture cap(cameraIndex);
     vidCap = cap;
}

Camera::~Camera() {vidCap.release();}

VideoCapture Camera::getVideoCaptureDevice() {return vidCap;}

void Camera::setCurrentFrame(Mat* newFrame) {currentFrame = newFrame;}

Mat Camera::getCurrentFrame() {return *currentFrame;}



