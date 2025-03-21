
#include "common/ImageCaptureBase.hpp"
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include <fstream>
#include <chrono>

using namespace msr::airlib;


int main(int argc, char *argv[]) 
{
    using namespace msr::airlib;

    // for car use CarRpcLibClient
    MultirotorRpcLibClient client("127.0.0.1");

    // get right, left and depth images. First two as png, second as float16.
    std::vector<ImageCaptureBase::ImageRequest> request = { 
        //uncompressed RGB array bytes
        ImageCaptureBase::ImageRequest("0", ImageCaptureBase::ImageType::Scene, false, false)     
    };


    int num = 0;
    try {
        std::ofstream outfile("frame.h264", std::ios::binary);
        while (num < 100) {
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<uint8_t> result = client.retrieveCameraH264Stream(request, "PX40", false);
            std::cout << "Receiving data: " <<  result.size() << std::endl;
            outfile.write(reinterpret_cast<char*>(result.data()), result.size());
            num++;
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

