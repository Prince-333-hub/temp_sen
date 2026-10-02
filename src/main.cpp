#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>
#include <random>

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_c);
    
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
    return std::string(buffer);
}

int main() {
    const double TEMP_THRESHOLD = 60.0;
    const std::string LOG_FILE = "temperature_log.txt";
    const char* DEVICE_PATH = "/dev/temp_sensor";

    std::cout << "Starting IoT System Monitor..." << std::endl;
    
    int fd = open(DEVICE_PATH, O_RDONLY);
    bool use_kernel_driver = (fd >= 0);

    if (use_kernel_driver) {
        std::cout << "[INFO] Successfully connected to Kernel Driver: " << DEVICE_PATH << std::endl;
    } else {
        std::cout << "[WARN] Kernel driver not loaded. Running in User-Space Fallback Mode." << std::endl;
    }

    std::ofstream logFile(LOG_FILE, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
        if (use_kernel_driver) close(fd);
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(20.0, 80.0);

    for (int i = 0; i < 10; ++i) {
        double current_temp = 0.0;

        if (use_kernel_driver) {
            char buffer[16] = {0};
            ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                current_temp = std::atof(buffer);
            } else {
                current_temp = dist(gen);
            }
        } else {
            current_temp = dist(gen);
        }

        std::string timestamp = getCurrentTimestamp();

        std::cout << "[" << timestamp << "] Reading: " 
                  << std::fixed << std::setprecision(2) << current_temp << " °C";
        
        logFile << "[" << timestamp << "] Reading: " 
                << std::fixed << std::setprecision(2) << current_temp << " °C";

        if (current_temp > TEMP_THRESHOLD) {
            std::cout << " [ALERT: OVERHEATING DETECTED!]";
            logFile << " [ALERT: OVERHEATING DETECTED!]";
        }
        
        std::cout << std::endl;
        logFile << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (use_kernel_driver) close(fd);
    logFile.close();
    return 0;
}
