#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <random>
#include <iomanip>
#include <ctime>

// Function to get current timestamp string
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

    // Random number generator for temperature simulation (20.0°C to 80.0°C)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(20.0, 80.0);

    std::cout << "Starting IoT Temperature Sensor Simulation..." << std::endl;
    std::cout << "Monitoring threshold set to: " << TEMP_THRESHOLD << " °C" << std::endl;
    std::cout << "Logging data to: " << LOG_FILE << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::ofstream logFile(LOG_FILE, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
        return 1;
    }

    // Run simulation loop 10 times
    for (int i = 0; i < 10; ++i) {
        double current_temp = dist(gen);
        std::string timestamp = getCurrentTimestamp();

        std::cout << "[" << timestamp << "] Temperature: " 
                  << std::fixed << std::setprecision(2) << current_temp << " °C";

        // Log to file
        logFile << "[" << timestamp << "] Temperature: " 
                << std::fixed << std::setprecision(2) << current_temp << " °C";

        // Threshold Alert Check
        if (current_temp > TEMP_THRESHOLD) {
            std::cout << " [ALERT: OVERHEATING DETECTED!]";
            logFile << " [ALERT: OVERHEATING DETECTED!]";
        }

        std::cout << std::endl;
        logFile << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    logFile.close();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Simulation completed. Check " << LOG_FILE << " for full log." << std::endl;

    return 0;
}
