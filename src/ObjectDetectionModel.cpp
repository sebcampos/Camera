//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/ObjectDetectionModel.h"

const char* ObjectDetectionModel::modelPath = "/Users/sebash/CLionProjects/Camera/resources/rpi-model.tflite";
const char* ObjectDetectionModel::labelsFile = "/Users/sebash/CLionProjects/Camera/resources/labelmap.txt";
int ObjectDetectionModel::width;
int ObjectDetectionModel::height;

std::unique_ptr<tflite::Interpreter> ObjectDetectionModel::interpreter;

void ObjectDetectionModel::setUpInterpreter()
{
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(modelPath);
    if (model == nullptr)
    {
        fprintf(stderr, "failed to load model\n");
        exit(-1);
    }

    // Initiate Interpreter
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



    // Configure the interpreter
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(1);

    // Get Input Tensor Dimensions
    int input = interpreter->inputs()[0];
    ObjectDetectionModel::height = interpreter->tensor(input)->dims->data[1];
    ObjectDetectionModel::width = interpreter->tensor(input)->dims->data[2];

}


void ObjectDetectionModel::processFrameInPlace(Mat* currentFrame) {
    // Copy image to input tensor
    // cv::Mat image;
    //cv::resize(*currentFrame, image, cv::Size(width, height), cv::INTER_NEAREST);
    memcpy(interpreter->typed_input_tensor<unsigned char>(0), currentFrame->data, currentFrame->total() * currentFrame->elemSize());

    // Inference
    std::chrono::steady_clock::time_point start, end;
    start = std::chrono::steady_clock::now();
    interpreter->Invoke();
    end = std::chrono::steady_clock::now();
    auto inference_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Get Output
    int output = interpreter->outputs()[0];
    TfLiteIntArray *output_dims = interpreter->tensor(output)->dims;
    auto output_size = output_dims->data[output_dims->size - 1];
    std::vector<std::pair<float, int>> top_results;
    float threshold = 0.01f;
    switch (interpreter->tensor(output)->type)
    {
        case kTfLiteFloat32:
            tflite::label_image::get_top_n<float>(interpreter->typed_output_tensor<float>(0), output_size, 1, threshold, &top_results, kTfLiteFloat32);
            break;

        case kTfLiteUInt8:
            tflite::label_image::get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0), output_size, 1, threshold, &top_results, kTfLiteUInt8);
            break;
        default:
            fprintf(stderr, "cannot handle output type\n");
            exit(-1);
    }
    // Print inference ms in input image
    cv::putText(*currentFrame, "Inference Time in ms: " + std::to_string(inference_time), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);

    // Load Labels
    auto labels = loadLabels();

    // Print labels with confidence in input image
    for (const auto &result : top_results)
    {
        const float confidence = result.first;
        const int index = result.second;
        std::string output_txt = "Label :" + labels[index] + " Confidence : " + std::to_string(confidence);
        cv::putText(*currentFrame, output_txt, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
    }

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