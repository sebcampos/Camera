//
// Created by Sebastian Campos on 2/29/24.
//

#include "../headers/ObjectDetectionModel.h"
#include "../headers/Camera.h"



void takePhoto()
{
    bool frameCaptured = false;
    std::cout << "Taking screenshot for processing" << std::endl;
    auto* camera = new Camera(0);
    while (!frameCaptured) { //Taking an everlasting loop to show the video//
        camera->captureFrame();
        if (!camera->getCurrentFrame().empty()) { //Breaking the loop if no video frame is detected//
            frameCaptured = true;
        }
    }
    imwrite("./test.png", camera->getCurrentFrame());
    delete camera;
}


void testProcessFrame()
{
    auto* camera = new Camera(0);
    auto* tfModel = new ObjectDetectionModel(camera->getWidth(), camera->getHeight());
    cv::Mat image = imread("/Users/sebash/CLionProjects/Camera/resources/test.jpg",  cv::IMREAD_GRAYSCALE);
    tfModel->processFrameInPlace(image);
    delete tfModel;
    delete camera;
}

int main()
{
    takePhoto();
    return 0;
}