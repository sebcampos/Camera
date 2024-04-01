//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/ObjectDetectionModel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>

using namespace std;
const char* ObjectDetectionModel::modelPath = "../resources/1.tflite";
const char* ObjectDetectionModel::labelsFile = "../resources/labelmap.txt";

struct ObjectDetectionResult {
    cv::Rect rec;
    int      class_id;
    float    prob;
};


void ObjectDetectionModel::addToTracking(std::string label)
{
    tracking.emplace_back(label);
}


void ObjectDetectionModel::removeFromTracking(std::string label)
{
    auto itr = std::find(tracking.begin(), tracking.end(), label);
    if (itr != tracking.end()) tracking.erase(itr);
}

/**
 * inverse of the logit function
 * @param x
 * @return
 */
float expit(float x) {
    return 1.f / (1.f + expf(-x));
}


/**
 * IoU is used to evaluate the performance of object detection by comparing the ground truth bounding box to
 * the predicted bounding box. Theses boxes are collected from the output of the tensorflow lite object detection
 * model
 * @param rectA opencv Rect reference
 * @param rectB opencv Rect reference
 * @return
 */
float intersectionOverUnion(cv::Rect& rectA, cv::Rect& rectB)
{
    int x1 = std::max(rectA.x, rectB.x);
    int y1 = std::max(rectA.y, rectB.y);
    int x2 = std::min(rectA.x + rectA.width, rectB.x + rectB.width);
    int y2 = std::min(rectA.y + rectA.height, rectB.y + rectB.height);
    int w = std::max(0, (x2 - x1 + 1));
    int h = std::max(0, (y2 - y1 + 1));
    float inter = w * h;
    float areaA = (float)rectA.width * (float)rectA.height;
    float areaB = (float)rectB.width * (float)rectB.height;
    float o = inter / (areaA + areaB - inter);
    return (o >= 0) ? o : 0;
}

/**
 * A post-processing technique that is commonly used in object detection to eliminate
 * duplicate detections and select the most relevant
 * @param boxes
 * @param nms_threshold
 */
void nonMaximumSuppression(vector<ObjectDetectionResult>& boxes, const double nms_threshold)
{
    vector<int> scores;
    for(auto & box : boxes){
        scores.push_back((int)box.prob);
    }
    vector<int> index;
    for(int i = 0; i < scores.size(); ++i){
        index.push_back(i);
    }
    sort(index.begin(), index.end(), [&](int a, int b){
        return scores[a] > scores[b]; });
    vector<bool> del(scores.size(), false);
    for(size_t i = 0; i < index.size(); i++){
        if( !del[index[i]]){
            for(size_t j = i+1; j < index.size(); j++){
                if(intersectionOverUnion(boxes[index[i]].rec, boxes[index[j]].rec) > nms_threshold){
                    del[index[j]] = true;
                }
            }
        }
    }
    vector<ObjectDetectionResult> new_obj;
    for(const auto i : index){
        ObjectDetectionResult obj;
        if(!del[i])
        {
            obj.class_id = boxes[i].class_id;
            obj.rec.x =  boxes[i].rec.x;
            obj.rec.y =  boxes[i].rec.y;
            obj.rec.width =  boxes[i].rec.width;
            obj.rec.height =  boxes[i].rec.height;
            obj.prob =  boxes[i].prob;
        }
        new_obj.push_back(obj);


    }
    boxes = new_obj;


}

/**
 * Object detection model constructor recieves he width and height of the camera
 * @param width
 * @param height
 */
ObjectDetectionModel::ObjectDetectionModel(int width, int height)
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
    interpreter->AllocateTensors();
    addToTracking("person");
}



void ObjectDetectionModel::processFrameInPlace(cv::Mat& currentFrame)
{
    cv::Mat frameCopy;
    std::vector<std::string> labels = loadLabels();
    std::vector<float> locations;
    std::vector<float> classes;


    // collect model height and width
    int input = interpreter->inputs()[0];
    auto modelHeight = interpreter->tensor(input)->dims->data[1];
    auto modelWidth = interpreter->tensor(input)->dims->data[2];


    cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGR2RGB);

    // resize frame to the correct size for tensorflow processing
    resize(currentFrame, frameCopy, cv::Size(modelWidth, modelHeight), cv::INTER_NEAREST);

    // copy resized frame to the input tensor
    memcpy(interpreter->typed_input_tensor<uchar>(0), frameCopy.data, frameCopy.total() * frameCopy.elemSize());
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(6);
    interpreter->Invoke();


    output_locations = interpreter->tensor(interpreter->outputs()[0]);
    auto output_data = output_locations->data.f;

    output_classes   = interpreter->tensor(interpreter->outputs()[1]);
    auto out_cls = output_classes->data.f;
    num_detections   = interpreter->tensor(interpreter->outputs()[2]);
    auto nums = num_detections->data.f;

    for (int i = 0; i < 20; i++){
        auto output = output_data[i];
        locations.push_back(output);
        classes.push_back(out_cls[i]);
    }

    int count=0;
    std::vector<ObjectDetectionResult> objects;

    for(int j = 0; j < locations.size(); j+=4){
        auto yMin= locations[j] * (float)cam_height;
        auto xMin= locations[j + 1] * (float)cam_width;
        auto yMax= locations[j + 2] * (float)cam_height;
        auto xMax= locations[j + 3] * (float)cam_width;
        auto width= xMax - xMin;
        auto height= yMax - yMin;

        // auto rec = Rect(xMin, yMin, width, height);

        //float score = expit(nums[count]);
        float score = nums[count];

        // auto id=outputClasses;
        ObjectDetectionResult object;
        object.class_id = (int)classes[count];
        object.rec.x = (int)xMin;
        object.rec.y = (int)yMin;
        object.rec.width = (int)width;
        object.rec.height = (int)height;
        object.prob = score;
        objects.push_back(object);

        count+=1;


    }
    nonMaximumSuppression(objects, 0.5);
    cv::RNG rng(12345);
    for (auto object : objects)
    {
        auto score=object.prob;
        if (score < 0.60f) continue;
        cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
        auto cls = object.class_id;
        std::stringstream message;

        // TODO record if label is found and not recording
        message << labels[cls+1] << ": " << std::fixed << std::setprecision(2) << score;
        cv::rectangle(currentFrame, object.rec,color, 1);
        cv::putText(currentFrame, message.str(), cv::Point(object.rec.x, object.rec.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX, .30, cv::Scalar(10, 255, 30));
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
