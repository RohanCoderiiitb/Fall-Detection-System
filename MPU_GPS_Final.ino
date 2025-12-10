#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPSPlus.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
MPU6050 mpu;
TinyGPSPlus gps;
#define gpsSerial Serial2

bool deviceConnected = false;
int buzPin = 5;
unsigned long lastGPSPrint = 0;

#define SERVICE_UUID        "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
float ax, ay, az, gx, gy, gz;
boolean fall = false;
boolean trigger1 = false;
boolean trigger2 = false;
boolean trigger3 = false;
byte trigger1count = 0;
byte trigger2count = 0;
byte trigger3count = 0;
float angleChange = 0;

#define FREEFALL_THRESHOLD    3
#define IMPACT_THRESHOLD      5
#define ORIENTATION_THRESHOLD 20

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    pServer->startAdvertising();
  }
};

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Wire.begin(21, 22);

  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
  }

  BLEDevice::init("ESP32_LocationNode");
  BLEDevice::setMTU(128);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  pinMode(buzPin, OUTPUT);
  digitalWrite(buzPin, LOW);
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  mpu.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);

  ax = (AcX - 2050) / 16384.0;
  ay = (AcY - 77) / 16384.0;
  az = (AcZ - 1947) / 16384.0;
  gx = (GyX + 270) / 131.07;
  gy = (GyY - 351) / 131.07;
  gz = (GyZ + 136) / 131.07;

  float Raw_Amp = sqrt(ax * ax + ay * ay + az * az);
  int Amp = Raw_Amp * 10;
  angleChange = sqrt(gx * gx + gy * gy + gz * gz);

  if (Amp <= FREEFALL_THRESHOLD && !trigger2) {
    trigger1 = true;
  }

  if (trigger1) {
    trigger1count++;
    if (Amp >= IMPACT_THRESHOLD) {
      trigger2 = true;
      trigger1 = false;
      trigger1count = 0;
    }
  }

  if (trigger2) {
    trigger2count++;
    if (angleChange >= ORIENTATION_THRESHOLD) {
      trigger3 = true;
      trigger2 = false;
      trigger2count = 0;
    }
  }

  bool lyingPosture = (abs(az) > 0.7) && (abs(ay) < 0.4);

  if (trigger3 && lyingPosture) {
    fall = true;
    trigger3 = false;
    digitalWrite(buzPin, HIGH);

    float lat = gps.location.isValid() ? gps.location.lat() : 0.0;
    float lng = gps.location.isValid() ? gps.location.lng() : 0.0;

    String fallJson = "{";
    fallJson += "\"fall_detected\":true,";
    fallJson += "\"lat\":" + String(lat, 6) + ",";
    fallJson += "\"lon\":" + String(lng, 6);
    fallJson += "}";

    if (deviceConnected) {
      pCharacteristic->setValue(fallJson.c_str());
      pCharacteristic->notify();
    }

    delay(2000);
    digitalWrite(buzPin, LOW);
    lastGPSPrint = millis();
  }

  if (trigger1count >= 10) { trigger1 = false; trigger1count = 0; }
  if (trigger2count >= 10) { trigger2 = false; trigger2count = 0; }

  if (millis() - lastGPSPrint > 5000) {
    lastGPSPrint = millis();

    if (gps.location.isValid() && deviceConnected) {
      float lat = gps.location.lat();
      float lng = gps.location.lng();

      String gpsJson = "{";
      gpsJson += "\"lat\":" + String(lat, 6) + ",";
      gpsJson += "\"lon\":" + String(lng, 6);
      gpsJson += "}";

      pCharacteristic->setValue(gpsJson.c_str());
      pCharacteristic->notify();
    }
  }

  static unsigned long lastDebugPrint = 0;
  if (millis() - lastDebugPrint > 200) {
    Serial.print("Amp: "); Serial.print(Amp);
    Serial.print(" | ay: "); Serial.print(ay, 2);
    Serial.print(" | az: "); Serial.print(az, 2);
    Serial.print(" | angleChange: "); Serial.print(angleChange);
    Serial.print(" | Posture: "); Serial.println(lyingPosture);
    lastDebugPrint = millis();
  }

  fall = false;
  delay(50);
}