#include <iostream>
#include <chrono>
#include "cuda_runtime_api.h"
#include "logging.h"
#include "common.hpp"
#include "time.h"
#include "include.h"
#include "Visualizer.h"
#define USE_FP16  // comment out this if want to use FP32
#define DEVICE 0  // GPU id
#define NMS_THRESH 0.4
#define CONF_THRESH 0.5
#define BATCH_SIZE 1
#define VIDEO_TYPE (0) //0 means laptop camera ;1 means images,2 means Videos,3 means RealsenseD435;
#define NET s  // s m l x
#define NETSTRUCT(str) createEngine_##str
#define CREATENET(net) NETSTRUCT(net)
#define STR1(x) #x
#define STR2(x) STR1(x)
char* input="../input/VOC";
char* output="../output/";
std::string filename;
std::string Video_name="../input/sample.mp4";
std::string weight="../engine/";
std::string classname_path="../engine/coco.names";
// stuff we know about the network and the input/output blobs
static const int INPUT_H = Yolo::INPUT_H;
static const int INPUT_W = Yolo::INPUT_W;
static const int CLASS_NUM = Yolo::CLASS_NUM;
static const int OUTPUT_SIZE = Yolo::MAX_OUTPUT_BBOX_COUNT * sizeof(Yolo::Detection) / sizeof(float) + 1;  // we assume the yololayer outputs no more than MAX_OUTPUT_BBOX_COUNT boxes that conf >= 0.1
const char* INPUT_BLOB_NAME = "data";
const char* OUTPUT_BLOB_NAME = "prob";
static Logger gLogger;
///////////////////
static float Data[BATCH_SIZE * 3 * INPUT_H * INPUT_W];
//for (int i = 0; i < 3 * INPUT_H * INPUT_W; i++)
//    data[i] = 1.0;
static float prob[BATCH_SIZE * OUTPUT_SIZE];
IRuntime* runtime;
ICudaEngine* engine;
IExecutionContext* context;
const auto window_name1 = "RGB Image";
int inputIndex;
int outputIndex;
void* buffers[2];
cudaStream_t stream;
std::vector<std::string> classNamesVec;
cv::VideoCapture cap;
cv::Mat color_mat,Depthmate,Dec_img;
vector<Objection> ObjectionOfOneMat;//一幅图中的目标
////////////////////
void doInference(IExecutionContext& context, cudaStream_t& stream, void **buffers, float* input, float* output, int batchSize) {
    // DMA input batch data to device, infer on the batch asynchronously, and DMA output back to host
    CHECK(cudaMemcpyAsync(buffers[0], input, batchSize * 3 * INPUT_H * INPUT_W * sizeof(float), cudaMemcpyHostToDevice, stream));
    context.enqueue(batchSize, buffers, stream, nullptr);
    CHECK(cudaMemcpyAsync(output, buffers[1], batchSize * OUTPUT_SIZE * sizeof(float), cudaMemcpyDeviceToHost, stream));
    cudaStreamSynchronize(stream);
}
cv::Mat Dection(cv::Mat &img){
    int fcount = 0;
        fcount++;
        for (int b = 0; b < fcount; b++) {
            if (img.empty()) continue;
            cv::Mat pr_img = preprocess_img(img); // letterbox BGR to RGB
            int i = 0;
            for (int row = 0; row < INPUT_H; ++row) {
                uchar *uc_pixel = pr_img.data + row * pr_img.step;
                for (int col = 0; col < INPUT_W; ++col) {
                    Data[b * 3 * INPUT_H * INPUT_W + i] = (float) uc_pixel[2] / 255.0;
                    Data[b * 3 * INPUT_H * INPUT_W + i + INPUT_H * INPUT_W] = (float) uc_pixel[1] / 255.0;
                    Data[b * 3 * INPUT_H * INPUT_W + i + 2 * INPUT_H * INPUT_W] = (float) uc_pixel[0] / 255.0;
                    uc_pixel += 3;
                    ++i;
                }
            }
        }
        // Run inference
        auto start = std::chrono::system_clock::now();
        doInference(*context, stream, buffers, Data, prob, BATCH_SIZE);
        auto end = std::chrono::system_clock::now();
//        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
        std::vector<std::vector<Yolo::Detection>> batch_res(fcount);
        for (int b = 0; b < fcount; b++) {
            auto& res = batch_res[b];
            nms(res, &prob[b * OUTPUT_SIZE], CONF_THRESH, NMS_THRESH);
        }
        for (int b = 0; b < fcount; b++) {
            auto& res = batch_res[b];
            std::cout <<"num:"<<res.size()<<" ";
            for (size_t j = 0; j < res.size(); j++) {
                cv::Rect r = get_rect(img, res[j].bbox);
                cv::rectangle(img, r, cv::Scalar(0x27, 0xC1, 0x36), 2);
                cv::putText(img, classNamesVec[(int)res[j].class_id], cv::Point(r.x, r.y - 1), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0xFF, 0xFF, 0xFF), 2);
                std::cout<<classNamesVec[(int)res[j].class_id]<<";";
                if (VIDEO_TYPE==3)
                    ObjectionOfOneMat.push_back(Objection(r,(int)res[j].class_id));
            }
            std::cout<<std::endl;
            if (VIDEO_TYPE==1)
                cv::imwrite(std::string(output) + filename, img);
        }
    return img;
}
int main(int argc, char** argv) {
    cudaSetDevice(DEVICE);
    int f=0;
    std::vector<std::string> file_names;
    dlib::perspective_window Win_3D;//3D显示窗口
    clock_t last_time;
    // create a model using the API directly and serialize it to a stream
    char *trtModelStream{ nullptr };
    size_t size{ 0 };
    std::string engine_name = STR2(NET);
    engine_name = weight+"yolov5" + engine_name + ".engine";
    std::ifstream file(engine_name);
    if (file.good()) {
        file.seekg(0, file.end);
        size = file.tellg();
        file.seekg(0, file.beg);
        trtModelStream = new char[size];
        assert(trtModelStream);
        file.read(trtModelStream, size);
        file.close();
    }
    std::ifstream classNamesFile(classname_path);
    if (classNamesFile.is_open())
    {
        std::string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }
    if (VIDEO_TYPE==0) {
        cap.open(0);
        if (!cap.isOpened()) {
            std::cout << "error" << std::endl;
        }
    }
    else if (VIDEO_TYPE==1){
        if (read_files_in_dir(input, file_names) < 0) {
            std::cout << "read_files_in_dir failed." << std::endl;
            return -1;
        }
    } else if (VIDEO_TYPE==2){
        cap.open(Video_name);
        std::cout<<"The Video FPS:"<<cap.get(CAP_PROP_FPS)<<std::endl;
    }else if (VIDEO_TYPE==3){//D435
        Realsense_config();//相机初始化
        Get_referance();
        pipes.start();
        namedWindow(window_name1, WINDOW_AUTOSIZE);
        Win_3D.set_title("ALL Objection 3D Point Cloud");
    }
    /*
     */
    // prepare input data ---------------------------
    runtime = createInferRuntime(gLogger);
    assert(runtime != nullptr)  ;
    engine = runtime->deserializeCudaEngine(trtModelStream, size);
    assert(engine != nullptr);
    context = engine->createExecutionContext();
    assert(context != nullptr);
    delete[] trtModelStream;
    assert(engine->getNbBindings() == 2);
    // In order to bind the buffers, we need to know the names of the input and output tensors.
    // Note that indices are guaranteed to be less than IEngine::getNbBindings()
    inputIndex = engine->getBindingIndex(INPUT_BLOB_NAME);
    outputIndex = engine->getBindingIndex(OUTPUT_BLOB_NAME);
    assert(inputIndex == 0);
    assert(outputIndex == 1);
    // Create GPU buffers on device
    CHECK(cudaMalloc(&buffers[inputIndex], BATCH_SIZE * 3 * INPUT_H * INPUT_W * sizeof(float)));
    CHECK(cudaMalloc(&buffers[outputIndex], BATCH_SIZE * OUTPUT_SIZE * sizeof(float)));
    // Create stream
    CHECK(cudaStreamCreate(&stream));
    while (cv::waitKey(1) != 27){
        auto time = clock();
        auto Time=(double)(time - last_time) / CLOCKS_PER_SEC;
        last_time=time;
        std::cout<<"Using time:"<<Time<<"s"<<std::endl;
        if (VIDEO_TYPE==0)
            cap.read(color_mat);
        else if (VIDEO_TYPE==1){
                if (f < (int)file_names.size()) {
                    filename=file_names[f];
                    f++;
                }
            color_mat=cv::imread(std::string(input) + "/" + filename);
        }else if (VIDEO_TYPE==2){
            cap.read(color_mat);
        } else if (VIDEO_TYPE==3){
            auto data = pipes.wait_for_frames();
            auto color_frame = data.get_color_frame();
            auto depth_frame = data.get_depth_frame();//.apply_filter(color_map)
            color_mat = frame_to_mat(color_frame);
            Mat depth_mat(Size(640,480),
                          CV_16U,(void*)depth_frame.get_data(),Mat::AUTO_STEP);
            Depthmate = depth_mat;
            Win_3D.clear_overlay();//清除原有点云信息
            ObjectionOfOneMat.clear();//清空上一幅图像的目标
        }
        Dec_img=Dection(color_mat);
        if (VIDEO_TYPE==3)
            Win_3D.add_overlay(Visualizer{ObjectionOfOneMat}.Report_PCLOneMat());//画出点云
        imshow(window_name1, Dec_img);
//        Win_3D.wait_until_closed();//暂停
    }

    // Release stream and buffers
    cudaStreamDestroy(stream);
    CHECK(cudaFree(buffers[inputIndex]));
    CHECK(cudaFree(buffers[outputIndex]));
    // Destroy the engine
    context->destroy();
    engine->destroy();
    runtime->destroy();
    return 0;

}
