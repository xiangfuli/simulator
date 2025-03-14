
#include "common/ImageCaptureBase.hpp"
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>


using namespace msr::airlib;

void saveRawImage(const cv::Mat& image, const std::string& filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return;
    }
    file.write(reinterpret_cast<const char*>(image.data), image.total() * image.elemSize());
    file.close();
}

int main() 
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

    // raw RGB data
    

    cv::Mat image(response[1].height, response[1].width, CV_8UC3, (void *)response[1].image_data_uint8.data());

    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

    bool success = cv::imwrite("output.png", image);
    if (success) {
        std::cout << "Image saved successfully!" << std::endl;
    } else {
        std::cout << "Failed to save image!" << std::endl;
    }

    saveRawImage(image, "output.raw");

    return 0;
}