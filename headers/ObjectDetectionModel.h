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

using namespace cv;

class ObjectDetectionModel {
    private:
        const char* modelPath = "path/to/model";
        std::unique_ptr<tflite::FlatBufferModel> model;
    protected:
        static Mat* processFrame(Mat* currentFrame);
    public:
        ObjectDetectionModel();

};


#endif //WEBCAMERA_OBJECTDETECTIONMODEL_H
