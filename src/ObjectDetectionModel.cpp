//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/ObjectDetectionModel.h"



const char* ObjectDetectionModel::modelPath = "/Users/sebash/CLionProjects/webcamera/resources/rpi-model.tflite";
const char* ObjectDetectionModel::labelsFile = "/Users/sebash/CLionProjects/webcamera/resources/labelmap.txt";
std::unique_ptr<tflite::FlatBufferModel> ObjectDetectionModel::model;
int ObjectDetectionModel::cam_width;
int ObjectDetectionModel::cam_height;

std::unique_ptr<tflite::Interpreter> ObjectDetectionModel::interpreter;

void ObjectDetectionModel::setUpInterpreter(int width, int height)
{
    cam_height = height;
    cam_width = width;
    model = tflite::FlatBufferModel::BuildFromFile(modelPath);
    if (model == nullptr)
    {
        fprintf(stderr, "failed to load model\n");
        exit(-1);
    }

    // Build the interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter);


}


void ObjectDetectionModel::processFrameInPlace(Mat* currentFrame) {
    interpreter-
}


std::vector<std::string> ObjectDetectionModel::loadLabels() {
        std::ifstream file(labelsFile);
        if (!file.is_open())
        {
            fprintf(stderr, "unable to open label file\n");
            exit(-1);
        }
        std::string label;
        std::vector<std::string> labels;

        while (std::getline(file, label))
        {
            if (!label.empty())
                labels.push_back(label);
        }
        file.close();
        return labels;
}