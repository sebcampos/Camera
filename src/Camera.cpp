//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/StreamCamera.h"
#include "../headers/HttpClient.h"

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
     setTracking(HttpClient::getTrackingList());
     std::cout << "Initialized camera and detection model" << std::endl;
}

/**
 * Setting for the tracking list used to identify what should be tracked
 * @param tracking vector of strings containing object labels
 */
void StreamCamera::setTracking(const std::vector<std::string>& tracking)
{
    trackingList = tracking;
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
 * Returns a reference to the current outputFrame
 * @return Mat instance, result of object detection and video capture
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
        for (const auto& label : trackingList)
        {
            if (tfLiteModel->isInFrame(label) && !recording)
            {
                int labelIndex = tfLiteModel->getObjectLabelIndex(label);
                recordingLabel = label;
                videoId = HttpClient::createObjectDetectionEvent(labelIndex);
                recordingThread = std::thread(&StreamCamera::startRecording, this, videoId);
            }
            else if (recording && !tfLiteModel->isInFrame(recordingLabel))
            {
                stopRecording();
            }

        }
        if (stopFeedFlag)
        {
            break;
        }

    }
}


/**
 * To be launches as a seperate thread, this method
 * takes a file name and begins recording the current frame to a video file.
 * Once the recording flag is set to false it stops
 * @param fileName String of the file to name the video recording
 */
 void StreamCamera::startRecording(const std::string& fileName)
{
     recording = true;
     std::cout << "started recording " << std::endl;

     // register object detection event
    VideoWriter video(fileName, VideoWriter::fourcc('M','J','P','G'), 10, Size(getWidth(),getHeight()), true);

    // TODO start recording
     while (recording)
     {
         // write frame to file
         video.write(getCurrentFrame());
     }
}


/**
 * sets the recording flag to false and waits until the thread is complete.
 */
void StreamCamera::stopRecording()
{
     recording = false;
     recordingThread.join();
     std::cout << "stopped recording " << std::endl;
     HttpClient::updateObjectDetectionEvent(videoId, false);
     recordingLabel= "";
     videoId = "";
}

/**
 * Called to set stopFeedFlag to True, stopping the ProcessFeed thread.
 */
void StreamCamera::stopFeed()
{
    stopFeedFlag = true;
}
