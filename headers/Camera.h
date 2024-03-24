//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_CAMERA_H
#define WEBCAMERA_CAMERA_H

#include<opencv2/opencv.hpp>
#include<iostream>


class Camera {
    private:
        cv::VideoCapture vidCap;
        cv::Mat currentFrame;
    public:
        cv::VideoCapture getVideoCaptureDevice();
        explicit Camera(int cameraIndex);
        ~Camera();
        void captureFrame();
        cv::Mat& getCurrentFrame();
        int getWidth();
        int getHeight();

};


#endif //WEBCAMERA_CAMERA_H
