//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/ObjectDetectionModel.h"

ObjectDetectionModel::ObjectDetectionModel()
{
    model = tflite::FlatBufferModel::BuildFromFile(modelPath);
    if (model == nullptr)
    {
        fprintf(stderr, "failed to load model\n");
        exit(-1);
    }
    // Initiate Interpreter
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder (*model, resolver)(&interpreter);
    if (interpreter == nullptr)
    {
        fprintf(stderr, "Failed to initiate the interpreter\n");
        exit(-1);
    }

    if (interpreter->AllocateTensors() != kTfLiteOk)
    {
        fprintf(stderr, "Failed to allocate tensor\n");
        exit(-1);
    }
}


Mat* ObjectDetectionModel::processFrame(Mat* currentFrame) {

}