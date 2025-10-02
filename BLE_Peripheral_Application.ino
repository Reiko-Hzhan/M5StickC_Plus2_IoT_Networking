#include <M5StickCPlus2.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define DEVICE_NAME "Reiko-Hzhan"

// Define custom service UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// Define custom characteristic UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Connection callback class
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      M5.Lcd.println("Device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      M5.Lcd.println("Device disconnected");
      // Restart advertising to be discoverable again
      pServer->getAdvertising()->start();
    }
};

void setup() {
  // Initialize M5StickCPlus2 device
  M5.begin();
  M5.Lcd.setRotation(3);  // Adjust screen orientation to match usage posture
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("BLE Broadcasting...");
  
  // Initialize BLE device
  BLEDevice::init(DEVICE_NAME);
  
  // Create BLE server instance
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Create BLE service with custom UUID
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create BLE characteristic with read/write/notify/indicate properties
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  
  // Add BLE 2902 descriptor (required for notify/indicate functions)
  pCharacteristic->addDescriptor(new BLE2902());
  
  // Start the BLE service
  pService->start();
  
  // Configure and start BLE advertising (make device discoverable)
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);  // Advertise the custom service
  pAdvertising->setScanResponse(true);         // Enable scan response
  pAdvertising->setMinPreferred(0x06);         // Set minimum preferred connection parameter (handles 0 internally)
  pAdvertising->setMinPreferred(0x12);         // Set additional minimum preferred parameter
  BLEDevice::startAdvertising();               // Start advertising process
  
  // Display device information on screen
  M5.Lcd.println("Device initialized:");
  M5.Lcd.print("Device Name: ");
  M5.Lcd.println(DEVICE_NAME);
}

void loop() {
  // Detect changes in BLE connection status
  if (deviceConnected != oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;  // Update connection status flag
  }
  
  // Delay 30 seconds to maintain device activity (prevent idle sleep)
  delay(30000);
}