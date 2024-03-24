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
        if (!camera->getInputFrame().empty()) { //Breaking the loop if no video frame is detected//
            frameCaptured = true;
        }
    }
    imwrite("./test.png", camera->getInputFrame());
    delete camera;
}


void testProcessFrame()
{
    auto* camera = new Camera(0);
    auto* tfModel = new ObjectDetectionModel(camera->getWidth(), camera->getHeight());
    cv::Mat image = imread("./test.png",  cv::IMREAD_GRAYSCALE);
    tfModel->processFrameInPlace(image);
    imwrite("./test.png", camera->getInputFrame());
    delete tfModel;
    delete camera;
}

int main()
{
    testProcessFrame();
    return 0;
}