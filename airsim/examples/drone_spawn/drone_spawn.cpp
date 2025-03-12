
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"

int main() 
{
    using namespace msr::airlib;

    // for car use CarRpcLibClient
    MultirotorRpcLibClient client;

    std::vector<std::string> vehicle_names = client.listVehicles();

    std::cout << "Now we have " << vehicle_names.size() << " vehicles";

    // first list all the vehocles
    const std::string vehicle_name = "UAV_" + std::to_string(vehicle_names.size());
    std::cout << "Vehicle name:" << vehicle_name << std::endl;

    const Pose pose(Vector3r(3, 0, 0.5), Quaternionr(1, 0, 0, 0));
    client.simAddVehicle(vehicle_name, "simpleflight", pose, "");
    
}