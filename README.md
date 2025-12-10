# 🛡️ ElderGuard — Smart IoT Jacket for Elderly Safety

A wearable IoT system for real-time fall detection, heart-rate monitoring, stress assessment, GPS tracking, and caregiver alerts.

---

## 🌟 Overview

ElderGuard is a smart jacket designed to monitor elderly individuals using two ESP32 microcontrollers.  
The system performs on-device sensing and processing, sends data over Bluetooth Low Energy (BLE) to a mobile Web Bluetooth app, and uploads timestamped packets to Firebase.  
A caretaker dashboard provides live updates, Google Maps location, and audible fall alerts.

---

## 🚀 Features

### 🩹 Fall Detection (ESP32-A)
- MPU6050 IMU sampled at 20 Hz  
- 3-stage trigger system:
  - Freefall (Amp < 0.3g)  
  - Impact (Amp > 0.5g)  
  - Orientation change (>20°/s) + lying-posture check  
- Sends immediate BLE alert with GPS coordinates  
- GPS updates every 5 seconds  

### ❤️ Heart-Rate Monitoring (ESP32-B)
- MAX30105 PPG sensor at 400 Hz  
- Detects beats using IR peaks  
- Computes BPM and 3-beat moving average  
- Flags fainting risk when BPM < 50  

### 😟 Stress Detection (GSR)
- ADC-based measurement of skin resistance  
- Stress classification:
  - Relaxed  
  - Calm  
  - Stressed  
  - Very Stressed  

### 🛰️ Cloud-Connected Caregiver Dashboard
- Live BPM, stress state, and location  
- Instant fall-detected modal popup  
- Audible alarm on fall events  

---

## 📡 System Architecture

ESP32 Motion Node → BLE → Mobile Web App → Firebase → Caretaker Dashboard  
ESP32 Health Node → BLE → Mobile Web App → Firebase → Caretaker Dashboard

### Motion Node Hardware
- ESP32-WROOM  
- MPU6050 IMU  
- NEO-6M GPS  

### Health Node Hardware
- ESP32-WROOM  
- MAX30105 Pulse Sensor  
- Analog GSR circuit  

---

## 📦 Suggested Repository Structure

ElderGuard/  
├── firmware_motion_node/  
│   └── motion_firmware.ino  
├── firmware_health_node/  
│   └── health_firmware.ino  
├── web_app/  
│   ├── index.html  
│   └── bluetooth.js  
├── caretaker_dashboard/  
│   ├── dashboard.html  
│   └── alerts.js  
└── README.md  

---

## ⚙️ How to Use

### 1️⃣ Flash the ESP32 Nodes
- Upload the motion firmware to ESP32-A  
- Upload the health firmware to ESP32-B  

### 2️⃣ Connect via Web Bluetooth
- Open the mobile Web Bluetooth app  
- Connect to **ESP32_LocationNode** and **ESP32_HealthNode**  
- Data will upload to Firebase automatically  

### 3️⃣ Open Caretaker Dashboard
- Displays live vitals and location  
- Triggers alarm on fall detection  

---

## 📊 Results

| Event                  | Avg. Latency |
|-----------------------|--------------|
| Heartbeat update      | ~150 ms      |
| Fall detection alert  | ~300 ms      |

GPS integration, fall detection, and cloud sync demonstrated successful end-to-end operation.

---

## 🎥 Demo Video
(Add your demo link here)

---

## 👥 Team
- Amitaash Rao  
- Gaurav Nayak  
- Harshvardhan Mishra  
- Nirmeet Udeshi  
- Prashant VSG  
- Rohan Kamath  

---

## 📜 License
Add a LICENSE file if you want to allow reuse.
