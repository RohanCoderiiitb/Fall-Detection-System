#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

#define SERVICE_UUID        "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

#define GSR_PIN 34
#define VCC 3.3
#define ADC_MAX 4095

int rawValue = 0;
float voltage = 0.0;
float resistance = 0.0;
String state;

MAX30105 particleSensor;
const byte RATE_SIZE = 3;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute = 0;
int beatAvg = 0;

bool deviceConnected = false;

#define MIN_BPM_THRESHOLD 40
#define MAX_BPM_THRESHOLD 110

#define FALL_RISK_BPM_THRESHOLD 50
#define FALL_RISK_OXY_THRESHOLD 90

unsigned long startMillis;
bool initializing = true;

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
  delay(1000);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    while (1);
  }

  particleSensor.setup(0x1F, 4, 2, 400, 411, 4096);
  particleSensor.setPulseAmplitudeRed(0x15);
  particleSensor.setPulseAmplitudeGreen(0x00);

  pinMode(GSR_PIN, INPUT);

  BLEDevice::init("ESP32_HealthNode");
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
}

void loop() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(GSR_PIN);
    delay(3);
  }
  rawValue = sum / 10;
  voltage = (rawValue / (float)ADC_MAX) * VCC;
  const float R1 = 10.0;
  resistance = (R1 * (VCC - voltage)) / voltage;

  if (resistance > 80) state = "Relaxed";
  else if (resistance > 40) state = "Calm";
  else if (resistance > 20) state = "Stressed";
  else state = "Very Stressed";

  long irValue = particleSensor.getIR();

  if (irValue < 50000) {
    delay(100);
    return;
  }
  
  if (checkForBeat(irValue) == true) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 200 && beatsPerMinute > 35) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  String heartbeatStatus = "Normal";
  if (beatAvg < MIN_BPM_THRESHOLD || beatAvg > MAX_BPM_THRESHOLD) {
    heartbeatStatus = "Erratic";
  }

  String fallRisk = "No";
  if (beatAvg < FALL_RISK_BPM_THRESHOLD || resistance < 40) {
    fallRisk = "Yes";
  }

  String jsonData = "{";
  jsonData += "\"status\":\"" + state + "\",";
  jsonData += "\"avg_bpm\":" + String(beatAvg) + ",";
  jsonData += "\"heartbeat\":\"" + heartbeatStatus + "\",";
  jsonData += "\"fall_risk\":\"" + fallRisk + "\"";
  jsonData += "}";

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    lastPrint = millis();

    if (deviceConnected) {
      pCharacteristic->setValue(jsonData.c_str());
      pCharacteristic->notify();
    }

    Serial.println(jsonData);
  }

  delay(10);
}