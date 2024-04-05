//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_STREAMCAMERA_H
#define WEBCAMERA_STREAMCAMERA_H

#include<opencv2/opencv.hpp>
#include<iostream>
#include "ObjectDetectionModel.h"

using namespace cv;

class StreamCamera {
    private:
        VideoCapture vidCap;
        Mat* currentFrame;
        Mat outputFrame;
        bool recording = false;
        ObjectDetectionModel* tfLiteModel;
        bool stopFeedFlag = false;
        int getWidth();
        int getHeight();
        void captureFrame();
    public:
        StreamCamera(int cameraIndex);
        ~StreamCamera();
        void processFeed();
        void stopFeed();
        void startRecording(std::string fileName);
        void stopRecording();
        Mat& getCurrentFrame();

};


#endif //WEBCAMERA_STREAMCAMERA_H
