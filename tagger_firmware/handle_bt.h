void send_bt(void * parameter) {
  while(true) {
    if(deviceConnected) {
      usb.println("sending via bt");
      pCharacteristic->setValue((int&)trigger_pressed);
      pCharacteristic->notify();
    }
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
  }
}

void handle_bt_data(std::string bt_data) {

  //check size of message
  if (bt_data.size() != 2+DATA_BYTES_LENGHT) {
    usb.println("Error: Wrong message size (allowed only 6): ");
    usb.print(bt_data.size()+'\n');
    return;
  }

  //interprete bt data
  unsigned char ctrl_byte = bt_data.at(0);
  unsigned char addr_byte = bt_data.at(1);
  std::string bt_data_subset = bt_data.substr(2, 2+DATA_BYTES_LENGHT);
  unsigned long data_bytes = *bt_data_subset.c_str(); //TODO: is this right?
  usb.println(bt_data_subset.c_str());
  
  //when command byte == COMMAND_STREAM go directly to stream mode
  if(ctrl_byte == COMMAND_STREAM) {
    usb.println("changing state to stream mode");
    state = STREAM;
    return;
  }
  
  else if(ctrl_byte == COMMAND_WRITE) {

    usb.println("writing to config");
  
    //check address byte and write to conf
    if(addr_byte == ADDR_SEND_BYTES) shootconf.send_bytes = data_bytes;
    else if(addr_byte == ADDR_SHOOT_COOLDOWN) shootconf.cooldown_in_ms = data_bytes;
    else if(addr_byte == ADDR_SHOOT_DELAY) shootconf.trigger_delay_in_ms = data_bytes;
    else if(addr_byte == ADDR_SHOOT_DURATION_MIN) shootconf.duration_min_in_ms = data_bytes;
    else if(addr_byte == ADDR_SHOOT_DURATION_MAX) shootconf.duration_max_in_ms = data_bytes;
    else if(addr_byte == ADDR_SHOOT_MODE) shootconf.smode = data_bytes;
    else if(addr_byte == ADDR_BURST_AMOUNT_MIN) shootconf.burst_amount_min = data_bytes;
    else if(addr_byte == ADDR_BURST_AMOUNT_MAX) shootconf.burst_amount_max = data_bytes;
    else if(addr_byte == ADDR_BURST_COOLDOWN) shootconf.burst_cooldown_in_ms = data_bytes;
    else if(addr_byte == ADDR_MAGAZINE_SIZE) shootconf.magazine_size = data_bytes;        
    else usb.println("unknown address");
  }

  else if (ctrl_byte == COMMAND_READ) {
      //TODO send config via bt
  
  }

  else usb.println("unknown command");
}

class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        usb.print("bt incoming: ");
        for (int i = 0; i < value.length(); i++)
          usb.print(value[i]);
        usb.println();

        switch(state) {
          case STREAM: {
            ir.write((const unsigned char*)value.c_str(),value.length()); //TODO: is this right?
            unsigned long latenz = millis() - latenz_timestamp;
            usb.println("latency value: ");
            usb.println(latenz);
            usb.println("sent to ir module: ");
            usb.println(value.c_str());
          } break;

          case AUTONOMOUS: {
            handle_bt_data(value);
          } break;
        }
      }
    }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {

      usb.println("device connected");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      usb.println("device disconnected");
      deviceConnected = false;
    }
};

void init_ble() {
  /*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
  */

    // Create the BLE Device
  BLEDevice::init("ESP32"); // Give it a name

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                     CHARACTERISTIC_UUID,
                     BLECharacteristic::PROPERTY_NOTIFY |
                     BLECharacteristic::PROPERTY_WRITE
                   );

  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  Serial.println("Waiting a client connection to notify...");

  BLEAddress mac_address = BLEDevice::getAddress();
  std::string mac_str = mac_address.toString();
  usb.print("MAC address: ");
  usb.write((const unsigned char*)mac_str.c_str(),mac_str.length());
  usb.println();
}
