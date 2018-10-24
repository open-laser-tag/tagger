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
      void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        usb.println("*********");
        usb.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          usb.print(value[i]);
        usb.println();
        usb.println("*********");

        switch(state) {
          case STREAM: {
            ir.write((const unsigned char*)value.c_str(),value.length()); //TODO: is this right?
            unsigned long latenz = millis() - latenz_timestamp;
            usb.println(latenz);
          } break;

          case AUTONOMOUS: {
            handle_bt_data(value);
          } break;
        }
      }
    }
};

void init_ble() {
  /*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
  */
  usb.println("Starting BLE work!");

  BLEDevice::init("ESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  
  usb.println("Characteristic defined! Now you can read it in your phone!");
}
