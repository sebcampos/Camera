//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_STREAMCAMERA_H
#define WEBCAMERA_STREAMCAMERA_H

#include<opencv2/opencv.hpp>
#include<iostream>
#include <thread>
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
        std::vector<std::string> trackingList;
        std::string videoId;
        std::string recordingLabel;
        std::thread recordingThread;
        int getWidth();
        int getHeight();
        void captureFrame();
    public:
        explicit StreamCamera(int cameraIndex);
        ~StreamCamera();
        void processFeed();
        void stopFeed();
        void startRecording(const std::string& fileName);
        void stopRecording();
        Mat& getCurrentFrame();
        void setTracking(const std::vector<std::string>& tracking)

};


#endif //WEBCAMERA_STREAMCAMERA_H
