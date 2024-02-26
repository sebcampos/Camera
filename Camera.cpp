//
// Created by Sebastian Campos on 2/25/24.
//

#include "Camera.h"

VideoCapture Camera::getVideoCaptureDevice() {return vidCap;}

void Camera::setCurrentFrame(Mat* newFrame) {currentFrame = newFrame;}

Mat Camera::getCurrentFrame() {return *currentFrame;}

