class Ir_send_callbacks: public BLECharacteristicCallbacks {
  
  void onWrite(BLECharacteristic *ir_send_char) {
    std::string value = ir_send_char->getValue();

    if (value.length() > 0) {
      usb.print("bt incoming: ");
      for (int i = 0; i < value.length(); i++)
        usb.print(value[i]);
      usb.println();

      ir.write((const unsigned char*)value.c_str(),value.length()); //TODO: is this right?
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
    void onConnect(BLEServer* pServer) {
      usb.println("device connected");
      led.light_on();
      return;
    }
    void onDisconnect(BLEServer* pServer) {
      usb.println("device disconnected");
      led.light_off();
      return;
    }
};

void init_ble() {
  /*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
  */

  usb.println("create BLE device");
  BLEDevice::init("ESP32"); // Give it a name

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
  return;
}
