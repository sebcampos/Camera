
#include "headers/Camera.h"
#include "headers/ObjectDetectionModel.h"
#include<iostream>


using namespace std;
using namespace cv;

int main() {

    namedWindow("Video Player");//Declaring the video to show the video//
    auto* camera = new Camera(0);
    ObjectDetectionModel::setUpInterpreter(camera->getWidth(), camera->getHeight());
    if (!camera->getVideoCaptureDevice().isOpened()){ //This section prompt an error message if no video stream is found//
        cout << "No video stream detected" << endl;
        system("pause");
        return-1;
    }
    while (true){ //Taking an everlasting loop to show the video//
        camera->captureFrame();
        if (camera->getCurrentFrame()->empty()){ //Breaking the loop if no video frame is detected//
            break;
        }
        // ObjectDetectionModel::processFrameInPlace(camera->getCurrentFrame());
        imshow("Video Player", *(camera->getCurrentFrame()));
        char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27){ //If 'Esc' is entered break the loop//
            break;
        }
    }
    delete camera;
    return 0;
}