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
        if (!camera->getCurrentFrame()->empty()) { //Breaking the loop if no video frame is detected//
            frameCaptured = true;
        }
    }
    imwrite("/Users/sebash/CLionProjects/Camera/resources/test.jpg", *(camera->getCurrentFrame()));
    delete camera;
}

void setUp()
{
    std::cout << "setting up interpreter" << std::endl;
    auto* camera = new Camera(0);
    // ObjectDetectionModel::setUpInterpreter(camera->getWidth(), camera->getHeight());
    delete camera;
}

void testProcessFrame()
{
    auto* tfModel = new ObjectDetectionModel();
    auto* camera = new Camera(0);
    tfModel->setUpInterpreter(camera->getWidth(), camera->getHeight());
    Mat image = imread("/Users/sebash/CLionProjects/Camera/resources/test.jpg",  IMREAD_GRAYSCALE);
    tfModel->processFrameInPlace(&image);
    delete tfModel;
    delete camera;
}

//int main()
//{
//    testProcessFrame();
//    return 0;
//}