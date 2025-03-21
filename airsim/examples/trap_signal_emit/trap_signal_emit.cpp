
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"

int main() 
{
    using namespace msr::airlib;

    // for car use CarRpcLibClient
    MultirotorRpcLibClient client;

    std::vector<std::string> vehicle_names = client.listVehicles();
    std::cout << "Now we have " << vehicle_names.size() << " vehicles" << std::endl;
    for (const auto& name : vehicle_names) {
        std::cout << "Vehicle name: " << name << std::endl;
    }
    // emit trap signal
    // 1.0f is the trap threshold, 2.0f is the simulation duration
    client.emitTrapSignal("PX40", 1.0f, 2.0f);

    // check trap signal
    // two numbers, first is if the drone is trapped, second is the number of traps triggered
    std::vector<uint32_t> trap_signal = client.ifTrapped();
    std::cout << "Trap times: " << trap_signal[0] << ", Target drone trapped: " << trap_signal[1] << std::endl;
}