//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/ObjectDetectionModel.h"


ObjectDetectionModel::ObjectDetectionModel()
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
    height = interpreter->tensor(input)->dims->data[1];
    width = interpreter->tensor(input)->dims->data[2];
    channels = interpreter->tensor(input)->dims->data[3];

}


Mat* ObjectDetectionModel::processFrame(Mat* currentFrame) {
    // Copy image to input tensor
    cv::Mat image;
    cv::resize(*currentFrame, image, cv::Size(width, height), cv::INTER_NEAREST);
    memcpy(interpreter->typed_input_tensor<unsigned char>(0), image.data, image.total() * image.elemSize());

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
        case kTfLiteInt32:
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
    cv::putText(frame, "Infernce Time in ms: " + std::to_string(inference_time), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);

    // Load Labels
    auto labels = load_labels(labelFile);

    // Print labels with confidence in input image
    for (const auto &result : top_results)
    {
        const float confidence = result.first;
        const int index = result.second;
        std::string output_txt = "Label :" + labels[index] + " Confidence : " + std::to_string(confidence);
        cv::putText(frame, output_txt, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
    }

    // Display image
    cv::imshow("Output", frame);
    cv::waitKey(0);
}


std::vector<std::string> loadLabels(std::string labelsFile) {
    {
        std::ifstream file(labelsFile.c_str());
        if (!file.is_open())
        {
            fprintf(stderr, "unable to open label file\n");
            exit(-1);
        }
        std::string label;
        std::vector<std::string> labels;

        while (std::getline(file, label))
        {
            if (label.size() > 0)
                labels.push_back(label);
        }
        file.close();
        return labels;
    }
}