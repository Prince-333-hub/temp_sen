CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = temp_monitor

all: $(TARGET)

$(TARGET): src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET) temperature_log.txt
