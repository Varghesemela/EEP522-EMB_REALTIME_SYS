# Starry – RPI-Based Outdoor Imaging System

## Objective

The Starry project aims to develop an innovative outdoor imaging system based on the Raspberry Pi (RPI) platform. The primary focus is on capturing high-quality starlapses and long-exposure night sky photographs. The system is designed with a modular architecture, enabling future enhancements such as a birdwatching feature. This report details the project’s conceptual framework, hardware architecture, and the implementation process focusing on camera testing, temperature sensor integration, and enclosure design.

## 1. Introduction

Outdoor imaging, particularly for astronomy, demands reliable hardware and software integration under challenging environmental conditions. The Starry project addresses this need by developing a robust, RPI-based imaging system that automates the process of capturing night sky images. Furthermore, the system’s modular design allows for future feature expansion, such as incorporating a birdwatching module to monitor avian activity near nests.

## 2. The Idea

### 2.1 Project Objectives

- **Primary Goal:** To develop a reliable and high-quality system for capturing starlapses and long-exposure night sky images using a Raspberry Pi platform.
- **Future Scope:** To potentially integrate additional functionalities, including a birdwatching feature for monitoring and recording bird activity near nests.

### 2.2 Concept Overview

The system is built on an RPI-based platform, integrating a high-resolution camera module and environmental sensors to optimize imaging performance under various outdoor conditions. A modular hardware design is employed, which allows for the future addition of other sensors and functionalities without significant redesign.

### 2.3 Hardware Block Diagram

A simplified hardware block diagram for the Starry system includes:

- **RPI Controller:** The central processing unit that manages data acquisition, sensor inputs, and control signals.
- **Camera Module:** High-resolution sensor for capturing starlapses and long-exposure night sky images.
- **Environmental Sensors:** Temperature sensor to monitor ambient conditions, which can affect image quality and system performance.
- **Storage & Connectivity Modules:** Local data storage on SD card as well as network interfaces for remote monitoring and data backup.
- **Enclosure:** A weatherproof housing designed to protect the components from outdoor elements while ensuring optimal performance.
![Hardware Block Diagram](/docs/images/hardware_diagram.jpg)

## 3. The Implementation

This section details the three core areas of implementation: camera testing, temperature sensor integration, and enclosure design.

### 3.1 Camera Testing & Implementation

- **Image Quality Evaluation:**  
  The camera module was subjected to rigorous tests under low-light conditions to ensure that it could capture the subtle details of the night sky. Key parameters such as resolution, noise levels, and exposure were evaluated to achieve optimal performance in starlapse photography.
  
- **Exposure Settings & Control:**  
  The testing phase involved fine-tuning exposure settings, including shutter speed and ISO sensitivity. These adjustments were crucial for capturing long-exposure images without overexposure or motion blur.
  
- **System Integration:**  
  The camera was successfully integrated with the Raspberry Pi, allowing automated control via custom scripts. This integration ensures seamless operation, from image capture to data storage.
  
- **Stability & Reliability:**  
  Tests were conducted to verify the stability of the imaging process. The system maintained consistent performance in prolonged use, ensuring that the starlapse sequences are recorded accurately over extended periods.

![Camera Logs](/docs/images/camera_logs.jpg)

### 3.2 Temperature Sensor Testing & Implementation

- **Sensor Accuracy & Calibration:**  
  The selected temperature sensor was calibrated to ensure accurate ambient temperature measurements. This data is essential for adjusting the system’s performance and for logging environmental conditions that could impact imaging quality.
  
- **Response Time & Data Integration:**  
  The sensor’s rapid response time was verified to ensure that real-time temperature data could be captured and logged effectively. The sensor is integrated into the RPI system, allowing for continuous monitoring and potential alerts if temperature extremes are detected.
  
- **Impact on Imaging:**  
  Temperature data helps in understanding the environmental conditions during image capture, which can be used for post-processing adjustments and system diagnostics.
![temperature sensor Logs](/docs/images/tmp_sensor_logs.jpg)

### 3.3 Enclosure Design

- **Weatherproofing:**  
  A robust enclosure was designed to protect the sensitive electronics from rain, dust, and other outdoor elements. The design prioritizes durability and long-term exposure to varying weather conditions.
  
- **Ventilation & Insulation:**  
  The enclosure incorporates proper ventilation to prevent overheating while maintaining adequate insulation to protect the components from extreme cold.
  
- **Ease of Installation & Maintenance:**  
  The enclosure design includes accessible ports and mounting options to facilitate quick installation and maintenance. This ensures that the system can be deployed with minimal setup time and can be easily serviced in the field.
  
- **Component Accessibility:**  
  Special consideration was given to ensure that critical components, such as sensors and the camera module, are easily accessible for troubleshooting and upgrades.
![Enclosure ](/docs/images/enclosure.jpg)

### 3.4 Source code and design structure

- The source code is organized modularly to allow easy updates and feature additions.
- Clear separation between camera control, sensor data logging, and system monitoring.
- Comprehensive unit tests and integration tests ensure code reliability.
- Code is documented and commented thoroughly to facilitate future development.
- Version control practices have been followed, making collaboration and debugging more efficient.

Below is the flow of implementation for Streaming live feed from camera over IP:
![camstream ](/docs/images/camera-stream.jpg)

Below is the flow of implementation for the I2C sensor Driver for the 2 SHTC3 sensors:
![shtc3-driver ](/docs/images/driver_flow.jpg)

## 4. Results & Discussion

- **Camera Testing:**  
  The initial tests confirmed that the camera module could capture high-quality starlapse images with consistent exposure settings. The automated control via RPI proved effective, and image quality under low-light conditions met the project’s benchmarks.
  
- **Temperature Sensor Performance:**  
  The sensor provided accurate and real-time temperature readings, which are now integrated into the system for logging purposes. These readings have already demonstrated potential for refining image processing techniques in response to environmental conditions.
  
- **Enclosure Effectiveness:**  
  The weatherproof enclosure has successfully shielded the hardware from outdoor elements during preliminary field tests. Its design not only protects the system but also ensures ease of maintenance and future upgrades.

## 5. Future Scope

While the current focus is on developing a high-quality night sky imaging system, the modular design opens the possibility for future enhancements. One potential future scope is the integration of a birdwatching module. This would allow the system to detect and record bird activity near nests, expanding its use cases to wildlife monitoring. Further development in this direction will involve additional sensor integration and software enhancements.

The project successfully meets most requirements, yet there remains potential for software and sensor data optimization.

- **Areas for Improvement:**  
  - Further refinement in automated exposure control and overall camera control could lead to better image quality.
  - Enhanced software features for real-time environmental adjustments will help in adapting to different climatic conditions.
  - Having the ability to host.
  - The prototype is well-suited for limited production in research and hobbyist environments. But with additional design refinement and cost optimization, the system could be scaled for mass production targeting amateur astronomers and outdoor enthusiasts.
- **Innovative Extensions:**  
  - Exploring machine learning techniques for real-time image enhancement and object detection.
  - Investigating cloud-based storage and remote monitoring for data analysis.
  - Conceptualizing a dual-mode operation to support both astronomical imaging and wildlife monitoring.

## 6. Conclusion

The Starry project represents a significant step forward in developing versatile, outdoor imaging systems using affordable and scalable technology like the Raspberry Pi. The current implementation has successfully achieved its primary objective of high-quality night sky imaging. With robust hardware testing and thoughtful design, the project is well-positioned for future enhancements such as birdwatching. The lessons learned from this phase will inform subsequent iterations and additional feature integrations.

- **Challenges Encountered:**  
  - Integrating multiple hardware components (camera, sensor, RPI) reliably.
  - Addressing imaging and streaming challenges.
  - Adding drivers to existing rpi kernel.
  - Ensuring system durability in varying outdoor conditions.
- **Engineering Solutions:**  
  - Advanced calibration of camera settings.
  - Robust sensor integration with real-time data logging.
  - Custom enclosure design to protect and cool the electronics effectively.

## 7. References

- [Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/)  
- [Camera Module - RPI HQ Camera](https://www.raspberrypi.com/documentation/accessories/camera.html)  
- [Temperature sensor - SHTC3](https://sensirion.com/media/documents/643F9C8E/63A5A436/Datasheet_SHTC3.pdf)  
- [Enclosure Design Guidelines](https://www.youtube.com/watch?v=Vrzjk_YtZJI)  
- [Prototype housing design ideas](https://github.com/AllskyTeam/allsky/discussions/3653)  
- [Allsky Project documentation](https://github.com/AllskyTeam/allsky)
