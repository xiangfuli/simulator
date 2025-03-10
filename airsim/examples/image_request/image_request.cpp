
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"

int main() 
{
    using namespace msr::airlib;

    // for car use CarRpcLibClient
    MultirotorRpcLibClient client("10.42.0.1");

    std::vector<uint8_t> png_image = client.simGetImage("0", ImageCaptureBase::ImageType::Scene);
    
    // do something with images
}
