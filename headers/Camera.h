//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_CAMERA_H
#define WEBCAMERA_CAMERA_H

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;
class Camera {
    private:
        VideoCapture vidCap;
        Mat currentFrame;
    public:
        VideoCapture getVideoCaptureDevice();
        explicit Camera(int cameraIndex);
        ~Camera();
        void captureFrame();
        Mat& getCurrentFrame();
        int getWidth();
        int getHeight();

};


#endif //WEBCAMERA_CAMERA_H
