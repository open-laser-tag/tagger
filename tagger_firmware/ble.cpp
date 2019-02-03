/**
 * @file ble.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief bluetooth low energy functions
 * @date 2019-01-23
 * 
 * 
 */

#include "ble.h"

class Ir_send_callbacks: public BLECharacteristicCallbacks {
  
  /**
   * @brief handles ble input data
   * This method is called when data is written to ir_send_char characteristic.
   * It hands over incoming data to infrared module.
   * @param ir_send_char 
   */
  void onWrite(BLECharacteristic *ir_send_char) {
    std::string value = ir_send_char->getValue();

    if (value.length() > 0) {
      usb.print("bt incoming: ");
      for (int i = 0; i < value.length(); i++) usb.print(value[i]);
      usb.println();

      ir.write((const unsigned char*)value.c_str(),value.length());
      latenz = millis() - latenz_timestamp;
      usb.print("latency value: ");
      usb.println(latenz);
      //send latency via BT to app
      vTaskResume(xHandle_send_latency);
      usb.print("sent to ir module: ");
      usb.println(value.c_str());


    }
  }
};

class MyServerCallbacks: public BLEServerCallbacks {
  /**
   * @brief turn led on connect on
   * This method is called when a device connects to tagger via ble.
   * The blue LED is turned on.
   * @param pServer 
   */
  void onConnect(BLEServer* pServer) {
    usb.println("device connected");
    led.light_on();
    return;
  }

  /**
   * @brief turn led on disconnect off
   * This method is called when a via ble connected device disconnects.
   * The blue LED is turned off.
   * @param pServer 
   */
  void onDisconnect(BLEServer* pServer) {
    usb.println("device disconnected");
    led.light_off();
    return;
  }
};

/**
 * @brief call once for init of ble
 * This method initializes bluetooth low energy.
 * Device, server, service, and characteristics are created.
 */
void init_ble() {
  /*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
  */

  usb.println("create BLE device");
  BLEDevice::init("OpenLT Tagger"); // Give it a name

  usb.println("create BLE server");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  usb.println("create BLE service");
  BLEService *pService = pServer->createService(SERVICE_UUID);

  usb.println("create BLE trigger characteristic");
  trigger_char     = pService->createCharacteristic(
                      CHARACTERISTIC_TRIGGER_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  trigger_char     ->addDescriptor(new BLE2902());

  usb.println("create BLE ir receive characteristic");
  ir_receive_char  = pService->createCharacteristic(
                      CHARACTERISTIC_IR_RECEIVE_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  ir_receive_char  ->addDescriptor(new BLE2902());

  usb.println("create BLE ir send characteristic");
  ir_send_char     = pService->createCharacteristic(
                      CHARACTERISTIC_IR_SEND_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );
  ir_send_char     ->addDescriptor(new BLE2902());
  ir_send_char     ->setCallbacks(new Ir_send_callbacks());
  
  usb.println("create BLE latency characteristic");
  latency_char     = pService->createCharacteristic(
                      CHARACTERISTIC_LATENCY_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  latency_char     ->addDescriptor(new BLE2902());

  /*
   * TODO: not used yet
   */
  usb.println("create BLE version characteristic");
  version_char     = pService->createCharacteristic(
                      CHARACTERISTIC_VERSION_UUID,
                      BLECharacteristic::PROPERTY_READ
                    );
  version_char     ->addDescriptor(new BLE2902());
  
  usb.println("start BLE service");
  pService->start();

  usb.println("start advertising");
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  BLEAddress mac_address = BLEDevice::getAddress();
  std::string mac_str = mac_address.toString();
  usb.print("MAC address: ");
  usb.write((const unsigned char*)mac_str.c_str(),mac_str.length());
  usb.println();

  /* Create a mutex type semaphore. */
   xMutex_BT = xSemaphoreCreateMutex();

  usb.println("Create BT Mutex");
  if( xMutex_BT != NULL ) usb.println("BT Mutex successfully created.");
  else usb.println("Error while creating BT Mutex");
  
  return;
}

/**
 * @brief ble send data
 * Activates bluetooth low energy notify, with semaphore handlers.
 */
void ble_notify(BLECharacteristic *characteristic) {

  usb.println("Sending BT...");
  //check if Mutex was successfully created
  if( xMutex_BT != NULL ) {
    /* See if we can obtain the semaphore.  If the semaphore is not
    available wait 10 ms to see if it becomes free. */
    if( xSemaphoreTake( xMutex_BT, 10 / portTICK_PERIOD_MS ) == pdTRUE ) {
        /* We were able to obtain the semaphore and can now access the
        shared resource. */
        characteristic->notify();
        /* We have finished accessing the shared resource.  Release the
        semaphore. */
        xSemaphoreGive( xMutex_BT );
        usb.println("BT successfully sent");
    }
    else usb.println("Error: BT Mutex locked for over 10ms, message possibly dropped");
  }
  else usb.println("Error: BT Mutex not available.");

  return;
}
