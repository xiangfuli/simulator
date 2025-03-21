#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/common_utils/FileSystem.hpp"
#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <map>

// Helper function to create a grid layout
cv::Mat createGridLayout(const std::map<std::string, cv::Mat>& images, float fps, std::vector<uint32_t> trap_signal, int grid_size = 2) {
    // Calculate the maximum dimensions
    int max_width = 0, max_height = 0;
    for (const auto& pair : images) {
        max_width = std::max(max_width, pair.second.cols);
        max_height = std::max(max_height, pair.second.rows);
    }

    // Create the grid canvas
    cv::Mat grid(max_height * grid_size, max_width * grid_size, CV_8UC3, cv::Scalar(0, 0, 0));

    // Place images in the grid
    int idx = 0;
    for (const auto& pair : images) {
        if (idx >= grid_size * grid_size) break;  // Limit to grid_size x grid_size

        int row = idx / grid_size;
        int col = idx % grid_size;
        
        cv::Mat roi = grid(cv::Rect(col * max_width, row * max_height, pair.second.cols, pair.second.rows));
        pair.second.copyTo(roi);

        // Add vehicle name as text
        cv::putText(grid, pair.first, 
                   cv::Point(col * max_width + 10, row * max_height + 30),
                   cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        
        idx++;
    }

    // Add FPS counter at the top-right corner
    std::string trap_text = "Trap times: " + std::to_string(trap_signal[0]) + " Trapped: " + std::to_string(trap_signal[1]);
    cv::putText(grid, trap_text,
                cv::Point(grid.cols - 475, grid.rows - 70),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

    std::string fps_text = "FPS: " + std::to_string(static_cast<int>(fps));
    cv::putText(grid, fps_text,
                cv::Point(grid.cols - 200, grid.rows - 30),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

    return grid;
}

int main() 
{
    using namespace msr::airlib;
    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::duration<float>;

    // Connect to the AirSim simulator
    msr::airlib::MultirotorRpcLibClient client;
    client.confirmConnection();

    // Get all vehicle names
    std::vector<std::string> vehicle_names = client.listVehicles();
    std::cout << "Found " << vehicle_names.size() << " vehicles" << std::endl;

    // Create a map to store images for each vehicle
    std::map<std::string, cv::Mat> vehicle_images;

    // Create main window
    cv::namedWindow("AirSim Vehicles", cv::WINDOW_AUTOSIZE);

    // Variables for FPS calculation
    auto last_time = Clock::now();
    float fps = 0.0f;
    const float fps_alpha = 0.1f;  // Smoothing factor for FPS calculation

    // Main loop
    while (true) {
        auto current_time = Clock::now();
        float frame_time = std::chrono::duration_cast<Duration>(current_time - last_time).count();
        last_time = current_time;

        // Calculate smoothed FPS
        float current_fps = 1.0f / frame_time;
        if (fps == 0.0) {
            fps = 10;
        } else {
            fps = fps_alpha * current_fps + (1.0f - fps_alpha) * fps;
        }

        // check trap signal
        std::vector<uint32_t> trap_signal = client.ifTrapped();
            
        // Clear previous images
        vehicle_images.clear();

        // For each vehicle
        for (const auto& vehicle_name : vehicle_names) {
            // Request image from the default camera ("0")
            std::vector<ImageCaptureBase::ImageRequest> request = { 
                ImageCaptureBase::ImageRequest("0", ImageCaptureBase::ImageType::Scene, false, false)
            };
            
            // Get image
            const std::vector<ImageCaptureBase::ImageResponse>& response = 
                client.simGetImages(request, vehicle_name);
            
            if (response.size() > 0 && response[0].width > 0) {
                // Convert raw image data to OpenCV format
                cv::Mat img = cv::Mat(response[0].height, response[0].width, 
                                     CV_8UC3, (void*)response[0].image_data_uint8.data());
                
                // Store the image
                vehicle_images[vehicle_name] = img.clone();
            }
        }

        // Create and display grid layout
        if (!vehicle_images.empty()) {
            cv::Mat grid = createGridLayout(vehicle_images, fps, trap_signal);
            cv::imshow("AirSim Vehicles", grid);
        }
        
        // Check for ESC key press
        if (cv::waitKey(1) == 27) {  // Changed to 1ms delay for better FPS
            std::cout << "ESC pressed, exiting..." << std::endl;
            break;
        }
    }

    // Clean up
    cv::destroyAllWindows();
    
    return 0;
}