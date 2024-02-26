//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_CAMERA_H
#define WEBCAMERA_CAMERA_H
#include<opencv2/opencv.hpp>//OpenCV header to use VideoCapture class//
#include<iostream>
using namespace cv;
using namespace std;
class Camera {
    private:
        static VideoCapture vidCap;
        static Mat* currentFrame;
    protected:
        static VideoCapture getVideoCaptureDevice();
        static void setCurrentFrame(Mat* newFrame);
    public:
        static Mat getCurrentFrame();


};


#endif //WEBCAMERA_CAMERA_H
