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
    cv::Mat out = cv::Mat();
    cv::Mat* result = camera->processFeed(new cv::Mat());
    cv::cvtColor(*result, out, cv::COLOR_BGR2RGB);
    imwrite("./test.png", out);
    delete camera;
}


void testProcessFrame()
{
    auto* camera = new Camera(0);
    auto* tfModel = new ObjectDetectionModel(camera->getWidth(), camera->getHeight());
    cv::Mat image = imread("./test.png",  cv::IMREAD_GRAYSCALE);
    tfModel->processFrameInPlace(image);
    imwrite("./test_det.png", *(camera->getInputFrame()));
    delete tfModel;
    delete camera;
}

int main()
{
    takePhoto();
    //testProcessFrame();
    return 0;
}