#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// #define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// #define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID (0xFE03)
#define VENDOR_ID (0x0171)


void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  uint16_t service = (uint16_t)SERVICE_UUID;

  BLEDevice::init("LED Clock");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(BLEUUID(service));
  // BLECharacteristic *pCharacteristic = pService->createCharacteristic(
  //                                        CHARACTERISTIC_UUID,
  //                                        BLECharacteristic::PROPERTY_READ |
  //                                        BLECharacteristic::PROPERTY_WRITE
  //                                      );

  // pCharacteristic->setValue("Hello World says Neil");
  pService->start();

  /* Comes from https://developer.amazon.com/en-US/docs/alexa/alexa-gadgets-toolkit/bluetooth-le-settings.html#adv-packet-for-pairing */
  std::string strServiceData = "";
  strServiceData += 0x02;     
  // strServiceData += (char)0x01;   
  // strServiceData += (char)0x06;   
  // strServiceData += (char)3;   
  // strServiceData += (char)0x03;   
  // strServiceData += (char)0xFE;
  // strServiceData += (char)0x03;
  // strServiceData += (char)23;   
  // strServiceData += (char)0x16;   
  // strServiceData += (char)0xFE;  
  // strServiceData += (char)0x03; 
  // strServiceData += (char)0x01;
  // strServiceData += (char)0x71;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0xFF;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x01;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   
  // strServiceData += (char)0x00;   

  BLEAdvertisementData data;
  data.addData(strServiceData);

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID(service));
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->setAdvertisementData(data);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() 
{
  sleep(200);
}