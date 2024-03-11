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
        static const char* modelPath;
        static const char* labelsFile;
        std::unique_ptr<tflite::FlatBufferModel> model;
        std::unique_ptr<tflite::Interpreter> interpreter;
        std::vector<std::string> loadLabels();
        int cam_width;
        int cam_height;

    public:
        void setUpInterpreter(int width, int height);
        void processFrameInPlace(Mat* currentFrame);

};


#endif //WEBCAMERA_OBJECTDETECTIONMODEL_H
