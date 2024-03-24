//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/Camera.h"

using namespace cv;

/**
 * Sets up an open cv VideoCapture instance, and an instance of
 * the object detection model
 * @param cameraIndex
 */
Camera::Camera(int cameraIndex)
{
     VideoCapture cap(cameraIndex);
     vidCap = cap;
     currentFrame = new Mat();
     outputFrame = Mat();
     tfLiteModel = new ObjectDetectionModel(getWidth(), getHeight());
     std::cout << "Initialized camera and detection model" << std::endl;
}

/**
 * releases the VideoCapture instance and deletes the
 * current frame & object detection model
 */
Camera::~Camera()
{
    vidCap.release();
    delete tfLiteModel;
    delete currentFrame;
    std::cout << "Released and closed!" << std::endl;

}


int Camera::getWidth() {
    return (int)vidCap.get(CAP_PROP_FRAME_WIDTH) / 6;
}

int Camera::getHeight() {
    return (int)vidCap.get(CAP_PROP_FRAME_HEIGHT) / 6;
}

Mat& Camera::getCurrentFrame() {
    return outputFrame;
}

void Camera::captureFrame() {
    vidCap >> *currentFrame;
    resize(*currentFrame, *currentFrame, Size(getWidth(), getHeight()));
    cvtColor(*currentFrame, *currentFrame, COLOR_BGR2RGB);
}


void Camera::processFeed()
{
    if (!vidCap.isOpened()){ //This section prompt an error message if no video stream is found//
        std::cout << "No video stream detected" << std::endl;
        system("pause");
        return;
    }
    while (true){ //Taking an everlasting loop to show the video//
        captureFrame();
        if (currentFrame->empty()){ //Breaking the loop if no video frame is detected//
            break;
        }
        tfLiteModel->processFrameInPlace(*currentFrame);
        outputFrame = currentFrame->clone();
        if (stopFeedFlag)
        {
            break;
        }

    }
}



void Camera::stopFeed()
{
    stopFeedFlag = true;
}
