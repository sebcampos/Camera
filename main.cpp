
#include "headers/Camera.h"
//#include<opencv2/opencv.hpp>
#include<iostream>
#include "tensorflow/lite/interpreter.h"


using namespace std;
using namespace cv;

int main() {

    namedWindow("Video Player");//Declaring the video to show the video//
    auto* camera = new Camera(0);
    if (!camera->getVideoCaptureDevice().isOpened()){ //This section prompt an error message if no video stream is found//
        cout << "No video stream detected" << endl;
        system("pause");
        return-1;
    }
    while (true){ //Taking an everlasting loop to show the video//
        camera->captureFrame();
        if (camera->getCurrentFrame().empty()){ //Breaking the loop if no video frame is detected//
            break;
        }
        imshow("Video Player", camera->getCurrentFrame());//Showing the video//
        char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27){ //If 'Esc' is entered break the loop//
            break;
        }
    }
    delete camera;
    return 0;
}