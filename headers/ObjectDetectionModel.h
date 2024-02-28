//
// Created by Sebastian Campos on 2/25/24.
//

#ifndef WEBCAMERA_OBJECTDETECTIONMODEL_H
#define WEBCAMERA_OBJECTDETECTIONMODEL_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/examples/label_image/get_top_n.h"
#include "tensorflow/lite/model.h"
#include <fstream>
#include <string>
#include <vector>

using namespace cv;

class ObjectDetectionModel {
    private:
        int height, width, channels;
        const char* modelPath = "/Users/sebash/CLionProjects/Camera/resources/1.tflite";
        std::unique_ptr<tflite::Interpreter> interpreter;
        std::vector<std::string> loadLabels(std::string labelsFile);
    protected:
        Mat* processFrame(Mat* currentFrame);
    public:
        ObjectDetectionModel();
        ~ObjectDetectionModel();

};


#endif //WEBCAMERA_OBJECTDETECTIONMODEL_H
