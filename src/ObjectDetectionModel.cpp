//
// Created by Sebastian Campos on 2/25/24.
//

#include "../headers/ObjectDetectionModel.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>

using namespace std;

struct Object{
    cv::Rect rec;
    int      class_id;
    float    prob;
};



float expit(float x) {
    return 1.f / (1.f + expf(-x));
}


//nms
float iou(Rect& rectA, Rect& rectB)
{
    int x1 = std::max(rectA.x, rectB.x);
    int y1 = std::max(rectA.y, rectB.y);
    int x2 = std::min(rectA.x + rectA.width, rectB.x + rectB.width);
    int y2 = std::min(rectA.y + rectA.height, rectB.y + rectB.height);
    int w = std::max(0, (x2 - x1 + 1));
    int h = std::max(0, (y2 - y1 + 1));
    float inter = w * h;
    float areaA = rectA.width * rectA.height;
    float areaB = rectB.width * rectB.height;
    float o = inter / (areaA + areaB - inter);
    return (o >= 0) ? o : 0;
}

void nms(vector<Object>& boxes,  const double nms_threshold)
{
    vector<int> scores;
    for(int i = 0; i < boxes.size();i++){
        scores.push_back(boxes[i].prob);
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
                if(iou(boxes[index[i]].rec, boxes[index[j]].rec) > nms_threshold){
                    del[index[j]] = true;
                }
            }
        }
    }
    vector<Object> new_obj;
    for(const auto i : index){
        Object obj;
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


const char* ObjectDetectionModel::modelPath = "../resources/efficientdet.tflite";
const char* ObjectDetectionModel::labelsFile = "../resources/cocolabels.txt";
//std::unique_ptr<tflite::FlatBufferModel> ObjectDetectionModel::model;
//int ObjectDetectionModel::cam_width;
//int ObjectDetectionModel::cam_height;

//std::unique_ptr<tflite::Interpreter> ObjectDetectionModel::interpreter;

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
    interpreter->AllocateTensors();
    // auto input_sigs = interpreter->GetInputName(2);
    // interpreter->signature_outputs("")
}


void ObjectDetectionModel::processFrameInPlace(Mat* currentFrame) {
    std::vector<std::string> labels = loadLabels();
    TfLiteTensor* output_locations = nullptr;
    TfLiteTensor* output_classes = nullptr;
    TfLiteTensor* num_detections = nullptr;
    int input = interpreter->inputs()[0];
    auto modelHeight = interpreter->tensor(input)->dims->data[1];
    auto modelWidth = interpreter->tensor(input)->dims->data[2];
    auto channels = interpreter->tensor(input)->dims->data[3];
    Mat outputFrame;
    resize(*currentFrame, outputFrame, Size(modelWidth, modelHeight), INTER_NEAREST);
    memcpy(interpreter->typed_input_tensor<unsigned char>(0), outputFrame.data, outputFrame.total() * outputFrame.elemSize());
    interpreter->Invoke();
    int output = interpreter->outputs()[0];
    auto detection_boxes = interpreter->tensor(output)[0];
    auto detection_classes = interpreter->tensor(output)[1];
    auto detection_scores = interpreter->tensor(output)[2];
    auto num_detections_count = interpreter->tensor(output)[3];

    output_locations = interpreter->tensor(interpreter->outputs()[0]);
    auto output_data = output_locations->data.f;
    std::vector<float> locations;
    std::vector<float> cls;

    output_classes   = interpreter->tensor(interpreter->outputs()[1]);
    auto out_cls = output_classes->data.f;
    num_detections   = interpreter->tensor(interpreter->outputs()[3]);
    auto nums = num_detections->data.f;
    for (int i = 0; i < 20; i++){
        auto output = output_data[i];
        locations.push_back(output);
        cls.push_back(out_cls[i]);
        std::cout << labels[out_cls[i] + 1] << std::endl;
    }
    int count=0;
    std::vector<Object> objects;

    for(int j = 0; j <locations.size(); j+=4){
        auto ymin=locations[j]*cam_height;
        auto xmin=locations[j+1]*cam_width;
        auto ymax=locations[j+2]*cam_height;
        auto xmax=locations[j+3]*cam_width;
        auto width= xmax - xmin;
        auto height= ymax - ymin;

        // auto rec = Rect(xmin, ymin, width, height);

        float score = expit(nums[count]); // How has this to be done?
        // std::cout << "score: "<< score << std::endl;
        // if (score < 0.5f) continue;

        // auto id=outputClasses;
        Object object;
        object.class_id = cls[count];
        object.rec.x = xmin;
        object.rec.y = ymin;
        object.rec.width = width;
        object.rec.height = height;
        object.prob = score;
        objects.push_back(object);

        count+=1;



    nms(objects,0.5);
    RNG rng(12345);
    std::cout << "size: "<<objects.size() << std::endl;
    for(int l = 0; l < objects.size(); l++)
    {
        Object object = objects.at(l);
        auto score=object.prob;
        std::cout<<"score:"<< score<<std::endl;
//        if (score < 0.60f) continue;
        Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
        auto cls = object.class_id;

        cv::rectangle(*currentFrame, object.rec,color, 1);
        cv::putText(*currentFrame, labels[cls+1], cv::Point(object.rec.x, object.rec.y - 5),
                    cv::FONT_HERSHEY_COMPLEX, .8, cv::Scalar(10, 255, 30));
        std::cout<< labels[cls+1] << std::endl;
        //for blurring
        if (object.rec.x<0){
            std::cout<<object.rec.x<<std::endl;
            object.rec.x=0;
            std::cout<<object.rec.x<<std::endl;
        }
        if (object.rec.y<0){
            std::cout<<object.rec.y<<std::endl;
            object.rec.y=0;
            std::cout<<object.rec.y<<std::endl;
        }

        cv::Rect region(object.rec.x, object.rec.y, object.rec.width, object.rec.height);

        cv::GaussianBlur((*currentFrame)(region), (*currentFrame)(region), Size(0, 0), 4);
        std::cout<<"2"<<std::endl;//


    }
    cv::imwrite("/Users/sebash/CLionProjects/Camera/resources/cam.jpg", *currentFrame);
    cv::imwrite("/Users/sebash/CLionProjects/Camera/resources/cam.png", *currentFrame);
    }
    std::cout << "foo you" << std::endl;

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