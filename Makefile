CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = temp_monitor

all: $(TARGET) driver_module

$(TARGET): src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

driver_module:
	$(MAKE) -C driver

clean:
	rm -f $(TARGET) temperature_log.txt
	$(MAKE) -C driver clean

.PHONY: all driver_module clean
