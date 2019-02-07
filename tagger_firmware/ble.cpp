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
      log.info("bt incoming: ");
      for (int i = 0; i < value.length(); i++) log.print(value[i]);
      log.println();

      ir.write((const unsigned char*)value.c_str(),value.length());
      latenz = millis() - latenz_timestamp;
      log.info("latency value: "+std::to_string(latenz));
      //send latency via BT to app
      vTaskResume(xHandle_send_latency);
      log.info("sent to ir module: "+value);
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
    log.info("device connected");
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
    log.info("device disconnected");
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

  log.debug("creating BLE device...");
  BLEDevice::init("OpenLT Tagger"); // Give it a name

  log.debug("creating BLE server...");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  log.debug("creating BLE service...");
  BLEService *pService = pServer->createService(SERVICE_UUID);

  log.debug("creating BLE trigger characteristic...");
  trigger_char     = pService->createCharacteristic(
                      CHARACTERISTIC_TRIGGER_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  trigger_char     ->addDescriptor(new BLE2902());

  log.debug("creating BLE ir receive characteristic...");
  ir_receive_char  = pService->createCharacteristic(
                      CHARACTERISTIC_IR_RECEIVE_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  ir_receive_char  ->addDescriptor(new BLE2902());

  log.debug("creating BLE ir send characteristic...");
  ir_send_char     = pService->createCharacteristic(
                      CHARACTERISTIC_IR_SEND_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );
  ir_send_char     ->addDescriptor(new BLE2902());
  ir_send_char     ->setCallbacks(new Ir_send_callbacks());
  
  log.debug("creating BLE latency characteristic...");
  latency_char     = pService->createCharacteristic(
                      CHARACTERISTIC_LATENCY_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  latency_char     ->addDescriptor(new BLE2902());

  log.debug("creating BLE version characteristic...");
  version_char     = pService->createCharacteristic(
                      CHARACTERISTIC_VERSION_UUID,
                      BLECharacteristic::PROPERTY_READ
                    );
  version_char     ->addDescriptor(new BLE2902());
  version_char     ->setValue(GIT_TAG);

  log.debug("starting BLE service...");
  pService->start();

  log.debug("starting advertising...");
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  /* Create a mutex type semaphore. */
  log.debug("creating BT mutex...");
  xMutex_BT = xSemaphoreCreateMutex();

  if( xMutex_BT != NULL ) log.debug("BT Mutex successfully created.");
  else log.error("Could not create BT Mutex");
  
  BLEAddress mac_address = BLEDevice::getAddress();
  std::string mac_str = mac_address.toString();
  log.info("MAC address: "+mac_str);
  return;
}

/**
 * @brief ble send data
 * Activates bluetooth low energy notify, with semaphore handlers.
 */
void ble_notify(BLECharacteristic *characteristic) {

  log.debug("Sending BT...");
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
        log.debug("BT successfully sent");
    }
    else log.error("BT Mutex locked for over 10ms, message possibly dropped");
  }
  else log.error("BT Mutex not available.");

  return;
}
