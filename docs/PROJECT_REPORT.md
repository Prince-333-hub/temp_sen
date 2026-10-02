# IoT Temperature Monitoring System - Project Report

## Stage 1: Project Introduction & Scope
- **Objective**: Develop an IoT temperature monitoring solution using Linux Device Drivers, System Programming concepts, and C++.
- **Problem Statement**: Real-time telemetry monitoring requires handling system-level hardware streams with application-level logging and alerting.
- **Application**: Industrial equipment temperature monitoring and overheating prevention.

## Stage 2: Requirements & PRD
- **Functional Requirements**:
  1. Read temperature data from `/dev/temp_sensor` or fallback to user-space generator.
  2. Evaluate threshold violations (Trigger alert above 60.0°C).
  3. Format output with standard timestamps (`YYYY-MM-DD HH:MM:SS`).
  4. Write log streams to `temperature_log.txt`.
- **Non-Functional Requirements**:
  - High availability via user-space fallback mode.
  - Modular build configuration using `Makefile`.

## Stage 3: System Architecture
- **Data Flow**: Kernel Driver (`temp_driver.c`) -> `/dev/temp_sensor` -> C++ Application (`main.cpp`) -> Terminal Display & Log File (`temperature_log.txt`).
- **State Machine**:
  - Normal State: Temperature <= 60.0°C.
  - Alert State: Temperature > 60.0°C.

## Stage 4 & 5: Implementation & Testing
- **Compilation**: Verified using GCC/G++ build toolchains.
- **Integration**: Handles kernel module presence gracefully.

## Stage 6: Final Results
- Project compiled, executed, and validated under Linux environment.
