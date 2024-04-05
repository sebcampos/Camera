//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/StreamCamera.h"

using namespace cv;

/**
 * Sets up an open cv VideoCapture instance, and an instance of
 * the object detection model
 * @param cameraIndex
 */
StreamCamera::StreamCamera(int cameraIndex)
{
     VideoCapture cap(cameraIndex);
     vidCap = cap;
     currentFrame = new Mat();
     outputFrame = Mat();
     tfLiteModel = new ObjectDetectionModel(getWidth(), getHeight());
     std::cout << "Initialized camera and detection model" << std::endl;
}

/**
 * releases the VideoCapture instance and deletes the
 * current frame & object detection model
 */
StreamCamera::~StreamCamera()
{
    vidCap.release();
    delete tfLiteModel;
    delete currentFrame;
    std::cout << "Released and closed!" << std::endl;

}

/**
 * gets the opencv capture device width / 6
 * @return integer
 */
int StreamCamera::getWidth() {
    return (int)vidCap.get(CAP_PROP_FRAME_WIDTH) / 5;
}

/**
 * gets the opencv capture device height / 6
 * @return integer
 */
int StreamCamera::getHeight() {
    return (int)vidCap.get(CAP_PROP_FRAME_HEIGHT) / 5;
}

/**
 * Returns a refrence to the current outputFrame
 * @return Mat instanc, result of object detection and video capture
 */
Mat& StreamCamera::getCurrentFrame() {
    return outputFrame;
}

/**
 * Captures a frame, writing it to the currentFrame, resizes the frame,
 * and colors the frame inplace.
 */
void StreamCamera::captureFrame() {
    vidCap >> *currentFrame;
    resize(*currentFrame, *currentFrame, Size(getWidth(), getHeight()));
//    cvtColor(*currentFrame, *currentFrame, COLOR_BGR2RGB);
}

/**
 * this method is launched as a separate thread by the socket server class.
 * it reads in frames, processes them through the object
 * detection model, and then writes them to the outputFrame while the stopFeedFlag
 * is set to false
 */
void StreamCamera::processFeed()
{
    if (!vidCap.isOpened())
    {
        std::cout << "No video stream detected" << std::endl;
        system("pause");
        return;
    }
    while (true)
    {
        captureFrame();
        if (currentFrame->empty())
        {
            break;
        }
        tfLiteModel->processFrameInPlace(*currentFrame);
        outputFrame = currentFrame->clone();
        if (stopFeedFlag)
        {
            break;
        }

    }
}


/**
 *
 */
 void StreamCamera::startRecording(std::string fileName)
{
     recording = true;

     // register object detection event
    VideoWriter video("fileName", VideoWriter::fourcc('M','J','P','G'), 10, Size(getWidth(),getHeight()), true);

    // TODO start recording
     while (recording)
     {
         // write frame to file
         video.write(getCurrentFrame());
     }
}

/**
 * Called to set stopFeedFlag to True, stopping the ProcessFeed thread.
 */
void StreamCamera::stopFeed()
{
    stopFeedFlag = true;
}
