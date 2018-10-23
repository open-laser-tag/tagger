
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        usb.println("*********");
        usb.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          usb.print(value[i]);

        usb.println();
        usb.println("*********");
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

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
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


//void handle_bt(void * parameter) {
//  int bt_data = 0;   // for incoming serial data from ir
//  
//  while(true) {
//    while (ble.available() > 0) {
//        bt_data = ble.read();
//        usb.print("Incoming BT: ");
//        usb.print(bt_data); //TODO: is this correct?
//        usb.print("\n");
//    }
//    ble.println("Hi, I'm BT");
//    vTaskDelay(500 / portTICK_PERIOD_MS); //Block for 500ms.
//  }
//}

//void bt_to_config() {
//
//  unsigned char bt_data[BT_MESSAGE_LENGHT];
//  uint8_t byte_counter=0; 
//  
//  while(true) {
//
//    if (bt.available() > 0) {
//      bt_data[byte_counter]=bt.read();
//      byte_counter++;
//      
//      //when command byte == COMMAND_STREAM go directly to stream mode
//      if(bt_data[0] == COMMAND_STREAM) {
//        state = STREAM;
//        return;
//      }
//      
//      //receive BT_MESSAGE_LENGHT bytes then handle message
//      else if(byte_counter >= BT_MESSAGE_LENGHT) {
//        //check command byte
//        if(bt_data[0] == COMMAND_WRITE) {
//      
//          //check address byte and write to conf
//          if(bt_data[1] == ADDR_SEND_BYTES) write_to_config(bt_data, &shootconf.send_bytes);
//          else if(bt_data[1] == ADDR_SHOOT_COOLDOWN) write_to_config(bt_data, &shootconf.cooldown_in_ms);
//          else if(bt_data[1] == ADDR_SHOOT_DELAY) write_to_config(bt_data, &shootconf.trigger_delay_in_ms);
//          else if(bt_data[1] == ADDR_SHOOT_DURATION_MIN) write_to_config(bt_data, &shootconf.duration_min_in_ms);
//          else if(bt_data[1] == ADDR_SHOOT_DURATION_MAX) write_to_config(bt_data, &shootconf.duration_max_in_ms);
//          else if(bt_data[1] == ADDR_SHOOT_MODE) write_to_config(bt_data, &shootconf.smode);
//          else if(bt_data[1] == ADDR_BURST_AMOUNT_MIN) write_to_config(bt_data, &shootconf.burst_amount_min);
//          else if(bt_data[1] == ADDR_BURST_AMOUNT_MAX) write_to_config(bt_data, &shootconf.burst_amount_max);
//          else if(bt_data[1] == ADDR_BURST_COOLDOWN) write_to_config(bt_data, &shootconf.burst_cooldown_in_ms);
//          else if(bt_data[1] == ADDR_MAGAZINE_SIZE) write_to_config(bt_data, &shootconf.magazine_size);        
//      
//          //TODO else error when unknown address
//        }
//      
//        else if (bt_data[0] == COMMAND_READ) {
//            //TODO send config via bt
//        
//          //error when unknown command byte
//          // TODO else error
//          memset(bt_data,0,BT_MESSAGE_LENGHT*sizeof(unsigned char));
//          byte_counter = 0;
//        }
//      }
//    }
//  }
//}
//
//void write_to_config(unsigned char bt_data[BT_MESSAGE_LENGHT], unsigned long *conf) {
//
//  *conf = 0;
//  
//  for (int i=0; i<DATA_BYTES_LENGHT-1; i++) {
//      shootconf.send_bytes <<= sizeof(unsigned char);
//      *conf |= (unsigned long)(bt_data[DATA_BYTES_POS+i]);
//  }
//}
