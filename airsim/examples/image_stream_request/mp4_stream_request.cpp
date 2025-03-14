#include "common/ImageCaptureBase.hpp"
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include <opencv2/opencv.hpp>
#include <string>

class VideoEncoder {
private:
    cv::VideoWriter writer;
    std::string pipeline;
    int width;
    int height;
    int fps;

public:
    VideoEncoder(const std::string& filename, int width, int height, int fps = 30) 
        : width(width), height(height), fps(fps) {
        // 构建GStreamer管道
        pipeline = "appsrc ! videoconvert ! x264enc tune=zerolatency speed-preset=superfast ! "
                  "h264parse ! mp4mux ! filesink location=" + filename;
        
        // 初始化VideoWriter
        writer.open(pipeline, cv::CAP_GSTREAMER, 0, fps, cv::Size(width, height), true);
        
        if (!writer.isOpened()) {
            throw std::runtime_error("Could not open video writer");
        }
    }

    bool write(const cv::Mat& frame) {
        if (frame.empty()) return false;
        
        // 确保输入图像大小正确
        cv::Mat resized;
        if (frame.size() != cv::Size(width, height)) {
            cv::resize(frame, resized, cv::Size(width, height));
        } else {
            resized = frame;
        }
        
        writer.write(resized);
        return true;
    }

    ~VideoEncoder() {
        writer.release();
    }
};

// 使用示例
void saveMatToH264(const cv::Mat& frame, const std::string& outputPath) {
    try {
        VideoEncoder encoder(outputPath, 640, 480, 30);

        int num=0;

        while (num < 100) {
          encoder.write(frame);
          num++;
        }
        
        
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) 
{
    using namespace msr::airlib;

    // for car use CarRpcLibClient
    MultirotorRpcLibClient client;

    // get right, left and depth images. First two as png, second as float16.
    std::vector<ImageCaptureBase::ImageRequest> request = { 
        //png format
        ImageCaptureBase::ImageRequest("0", ImageCaptureBase::ImageType::Scene),
        //uncompressed RGB array bytes
        ImageCaptureBase::ImageRequest("1", ImageCaptureBase::ImageType::Scene, false, false),       
        //floating point uncompressed image  
        ImageCaptureBase::ImageRequest("1", ImageCaptureBase::ImageType::DepthPlanar, true) 
    };

    const std::vector<ImageCaptureBase::ImageResponse>& response = client.simGetImages(request);
    // do something with response which contains image data, pose, timestamp etc

    // save the rgb data into file
    /**
     * struct ImageResponse
        {
            vector<uint8_t> image_data_uint8;
            vector<float> image_data_float;

            std::string camera_name;
            Vector3r camera_position = Vector3r::Zero();
            Quaternionr camera_orientation = Quaternionr::Identity();
            TTimePoint time_stamp = 0;
            std::string message;
            bool pixels_as_float = false;
            bool compress = true;
            int width = 0, height = 0;
            ImageType image_type;
        };
    */

    cv::Mat image(response[1].height, response[1].width, CV_8UC3, (void *)response[1].image_data_uint8.data());
    
    saveMatToH264(image, "output.mp4");

    return 0;
}