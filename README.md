🛡️ ElderGuard — Smart IoT Jacket for Elderly Safety

Real-time fall detection, health monitoring, and cloud-connected caregiver alerts.

🌟 Overview

ElderGuard is a wearable IoT safety system designed to monitor elderly individuals in real time.
The jacket integrates motion sensing, GPS tracking, heart-rate measurement, and stress detection, communicating continuously with a cloud-connected caretaker dashboard.

The system uses:

Two ESP32 microcontrollers

One for motion + fall detection + GPS

One for heart rate (PPG) + GSR stress sensing

BLE (Bluetooth Low Energy) for real-time data transfer

A Web Bluetooth mobile app as the gateway

Firebase Realtime Database for cloud synchronization

A caretaker dashboard with alerts, vitals, and live location

According to the report, the system delivers <300 ms full pipeline latency from sensor → cloud → alert. 

Final_Report_Elder_Guard

📸 System Architecture
🔧 Hardware Overview

Referenced from Figures 1 and 5 in the PDF:

ESP32-A Motion Node

MPU6050 IMU (accelerometer + gyroscope)

NEO-6M GPS module

Fall-detection logic executed every 50 ms

ESP32-B Health Node

MAX30105 pulse sensor (400 Hz sampling)

Analog GSR circuit (skin resistance → stress classification)

Power: 5V power bank

Wearable assembly: Sensor nodes wired into a jacket for mobility and safety monitoring

Final_Report_Elder_Guard

🧠 Features
🩹 1. Fall Detection (ESP32-A)

Fall detection uses a 3-stage trigger system from the report:

Freefall: total acceleration < 0.3g

Impact: acceleration > 0.5g

Orientation: rotation > 20°/s + lying posture check

GPS is sent every 5 s; fall alerts are sent immediately.

❤️ 2. Heart-Rate Monitoring (ESP32-B)

MAX30105 IR sensor at 400 Hz

Beat detection based on IR peak timing

Computes BPM and 3-beat moving average

Detects fainting risk when BPM < 50

😟 3. Stress Detection (GSR Circuit)

Skin resistance measured via ADC

Stress levels mapped as:

80 kΩ → Relaxed

40–80 kΩ → Calm

20–40 kΩ → Stressed

<20 kΩ → Very Stressed

🛰️ 4. Cloud-Connected Caregiver Dashboard

From the real-time dashboard described in Section 5.2:

Live BPM updates

Stress levels

GPS coordinates with Google Maps link

Instant fall-alert popup + audio alarm


Final_Report_Elder_Guard

🖥️ Software Workflow
📱 Patient Mobile App (Web Bluetooth)

Connects to both ESP32 nodes

Receives JSON packets via BLE notifications

Adds timestamp

Uploads to Firebase via REST API

🧭 Caretaker Dashboard

Subscribes to Firebase changes

Displays live vitals, stress, location

Triggers an alarm when "fall_detected": true is received

📂 Project Structure (Suggested Repo Layout)
ElderGuard/
├── firmware_motion_node/        # ESP32-A: MPU6050 + GPS + fall detection
├── firmware_health_node/        # ESP32-B: MAX30105 + GSR + BPM extraction
├── web_app/                     # Web Bluetooth code for patient-side gateway
├── caretaker_dashboard/         # HTML/JS dashboard with alerts and live data
└── docs/
    └── Final_Report_Elder_Guard.pdf

🛠️ Hardware Used

From Section 4 of the PDF:

ESP32-WROOM-32 × 2

MPU6050 IMU

NEO-6M GPS module

MAX30105 Pulse/PPG Sensor

Analog GSR sensor

Portable power bank


Final_Report_Elder_Guard

🚀 Running the System
1️⃣ Flash the ESP32 Nodes

Upload the Motion Node firmware to ESP32-A

Upload the Health Node firmware to ESP32-B
(The PDF includes full firmware listings: Appendix A & Appendix B.)

2️⃣ Start the Patient Web App

Enable Web Bluetooth

Connect to:

"ESP32_LocationNode"

"ESP32_HealthNode"

The app will begin uploading data to Firebase.

3️⃣ Open Caretaker Dashboard

Displays:

Heart rate

Stress status

Location

Fall alerts with siren audio

📊 Results

According to Table 1 (page 8):

Event	Cloud Latency
Routine heartbeat update	~150 ms
Fall detected (alert)	~300 ms

GPS integration and fall detection were confirmed working in demonstration testing.


Final_Report_Elder_Guard

🎥 Demo Video

(As referenced in Section 7)
Google Drive Link:
https://drive.google.com/file/d/1ESebDfQ9aW7mKUBl7Qa6kqjXh2BnNB3O/view?usp=sharing



Final_Report_Elder_Guard

🤝 Team

Amitaash Rao

Gaurav Nayak

Harshvardhan Mishra

Nirmeet Udeshi

Prashant VSG

Rohan Kamath

🧾 References

(List from page 9 included automatically from PDF.)


Final_Report_Elder_Guard

If you want, I can also generate:

✅ A shorter README (compressed for GitHub)
✅ A graphical architecture diagram
✅ A markdown badge section ("Made with ESP32", "Firebase", etc.)
Just tell me!

Sources
You said:
Something which can be copy pasted onto github readme
ChatGPT said:

Here is a clean, polished, copy-paste-ready GitHub README for your ElderGuard project.
No citations, no clutter — just a perfect README.md you can drop directly into your repo.

🛡️ ElderGuard — Smart IoT Jacket for Elderly Safety

A wearable, cloud-connected system for real-time fall detection, heart-rate monitoring, stress assessment, and caregiver alerts.

🌟 Overview

ElderGuard is an IoT-enabled smart jacket designed to enhance safety for elderly individuals.
The system integrates:

Motion sensing (fall detection)

GPS tracking

Heart-rate monitoring (PPG)

Stress measurement (GSR)

Web-Bluetooth mobile gateway

Real-time caretaker dashboard with alerts

Two ESP32 nodes perform on-device processing and stream data via Bluetooth Low Energy (BLE), enabling sub-300 ms cloud-to-alert latency.

🧠 Features
🩹 Fall Detection (ESP32-A)

MPU6050 IMU sampled at 20 Hz

3-stage detection pipeline:

Freefall (acceleration < 0.3g)

Impact (acceleration > 0.5g)

Orientation change (>20°/s) + posture check

Immediate BLE alert with GPS coordinates

GPS updates every 5 seconds

❤️ Heart-Rate Monitoring (ESP32-B)

MAX30105 PPG sensor at 400 Hz

Beat-to-beat timing → BPM computation

Detects bradycardia / fainting risk (BPM < 50)

😟 Stress Detection (GSR)

Skin resistance measurement

Stress levels: Relaxed / Calm / Stressed / Very Stressed

🛰️ Live Cloud Dashboard

Real-time vitals

Stress state updates

GPS location embedding

Fall alert popup + audible alarm

📡 System Architecture
ESP32 Motion Node -> BLE -> Patient Web App -> Firebase -> Caretaker Dashboard
ESP32 Health Node -> BLE -> Patient Web App -> Firebase -> Caretaker Dashboard


Motion Node

MPU6050 (IMU)

NEO-6M GPS

On-device fall detection logic

Health Node

MAX30105 pulse oximetry sensor

Analog GSR sensing circuit

Cloud + App

Web Bluetooth (mobile)

Firebase Realtime Database

Browser-based caretaker UI

📂 Project Structure (Suggested)
ElderGuard/
├── firmware_motion_node/
│   └── esp32_motion_firmware.ino
├── firmware_health_node/
│   └── esp32_health_firmware.ino
├── web_app/
│   ├── index.html
│   └── bluetooth.js
├── caretaker_dashboard/
│   ├── dashboard.html
│   └── alerts.js
└── docs/
    └── ElderGuard_Report.pdf

🛠️ Hardware Used

2 × ESP32-WROOM-32

MPU6050 IMU

NEO-6M GPS module

MAX30105 PPG sensor

GSR analog sensor

5V portable power bank

🚀 How It Works
1️⃣ Flash the ESP32 Nodes

Upload the Motion Node firmware to ESP32-A and the Health Node firmware to ESP32-B.

2️⃣ Connect via Web Bluetooth

Open the mobile web app and connect to:

ESP32_LocationNode

ESP32_HealthNode

3️⃣ Data Flow

The app timestamps packets and pushes them to Firebase.

4️⃣ Caretaker Dashboard

Opens in a browser and provides:

Live BPM & stress indicators

GPS coordinates with Google Maps links

Immediate fall alerts with alarm sound

📊 Results
Event	Average Latency
Heartbeat update	~150 ms
Fall detection alert	~300 ms

GPS integration, fall detection, and health monitoring were successfully validated with end-to-end cloud connectivity.

🎥 Demo Video

A full demonstration of the system (fall detection, vitals monitoring, dashboard alerts):
👉 Add your video link here

🧑‍💻 Team

Amitaash Rao

Gaurav Nayak

Harshvardhan Mishra

Nirmeet Udeshi

Prashant VSG

Rohan Kamath

📜 License

This project is released for academic and research use. Add a license if needed
