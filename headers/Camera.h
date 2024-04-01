//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_CAMERA_H
#define WEBCAMERA_CAMERA_H

#include<opencv2/opencv.hpp>
#include<iostream>
#include "ObjectDetectionModel.h"

using namespace cv;

class Camera {
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
        Camera(int cameraIndex);
        ~Camera();
        void processFeed();
        void stopFeed();
        void startRecording();
        void stopRecording();
        Mat& getCurrentFrame();

};


#endif //WEBCAMERA_CAMERA_H
