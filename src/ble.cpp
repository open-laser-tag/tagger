/**
 * @file ble.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief bluetooth low energy functions
 * @date 2019-01-23
 * 
 * 
 */

#include "ble.h"

bool device_connected = false;

class Ir_send_callbacks : public BLECharacteristicCallbacks
{
    /**
   * @brief handles ble input data
   * This method is called when data is written to ir_send_char characteristic.
   * It hands over incoming data to infrared module.
   * @param ir_send_char 
   */
    void onWrite(BLECharacteristic *ir_send_char)
    {
        const char *logtag = "BT";

        std::string value = ir_send_char->getValue();

        if (value.length() > 0)
        {
            ESP_LOGI(logtag, "BT incoming: 0x%X", value);
            ESP_LOGW(logtag, "Sending this data via ir is not implemented, just sending 0xFFFF");
            ir_led.send(0xFFFF);
            latency = millis() - last_time_button_pressed_timestamp;
            ESP_LOGI(logtag, "time in ms since last trigger: %u", latency);
            //send latency via BT to app
            vTaskResume(xHandle_send_latency);
        }
    }
};

class Led_callbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *ir_send_char)
    {
        std::string value = ir_send_char->getValue();

        for (int i = 0; i < value.length() / 4 && i / 4 < NUM_LEDS; i = i + 4)
        {
            uint8_t led = value.at(i);
            uint8_t r = value.at(i + 1);
            uint8_t g = value.at(i + 2);
            uint8_t b = value.at(i + 3);
            leds[led].setRGB(r, g, b);
        }

        FastLED.show();
    }
};

class OTA_callbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *ota_char)
    {
        ESP_LOGI(logtag, "setting OTA flag");
        EEPROM.write(EEPROM_ADDR_OTA, 1);
        EEPROM.commit();
        ESP_LOGI(logtag, "OTA flag set, rebooting");
        ESP.restart();
    }
};

class MyServerCallbacks : public BLEServerCallbacks
{
    /**
   * @brief turn led on connect on
   * This method is called when a device connects to tagger via ble.
   * The blue LED is turned on.
   * @param pServer 
   */
    void onConnect(BLEServer *pServer)
    {
        ESP_LOGI(logtag, "device connected");
        led.light_on();
        leds[LED_INDEX_BT].setColorCode(COLOR_BT_CONNECTION_ON);
        FastLED.show();
        device_connected = true;
        return;
    }

    /**
   * @brief turn led on disconnect off
   * This method is called when a via ble connected device disconnects.
   * The blue LED is turned off.
   * @param pServer 
   */
    void onDisconnect(BLEServer *pServer)
    {
        const char *logtag = "BT";

        ESP_LOGI(logtag, "device disconnected");
        led.light_off();
        leds[LED_INDEX_BT].setColorCode(COLOR_BT_CONNECTION_OFF);
        FastLED.show();
        device_connected = false;
        return;
    }
};

/**
 * @brief call once for init of ble
 * This method initializes bluetooth low energy.
 * Device, server, service, and characteristics are created.
 */
void init_ble()
{
    /*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
  */
    const char *logtag = "BT";

    ESP_LOGD(logtag, "creating BLE device...");
    BLEDevice::init("Open Laser Tag Tagger"); // Give it a name

    ESP_LOGD(logtag, "creating BLE server...");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    ESP_LOGD(logtag, "creating BLE service...");
    BLEService *pService = pServer->createService(SERVICE_UUID);

    ESP_LOGD(logtag, "creating BLE led characteristic...");
    led_char = pService->createCharacteristic(
        CHARACTERISTIC_LED_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    led_char->setCallbacks(new Led_callbacks());

    ESP_LOGD(logtag, "creating BLE OTA characteristic...");
    ota_char = pService->createCharacteristic(
        CHARACTERISTIC_OTA_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    ota_char->setCallbacks(new OTA_callbacks());

    ESP_LOGD(logtag, "creating BLE trigger characteristic...");
    trigger_char = pService->createCharacteristic(
        CHARACTERISTIC_TRIGGER_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    trigger_char->addDescriptor(new BLE2902());

    ESP_LOGD(logtag, "creating BLE ir receive characteristic...");
    ir_receive_char = pService->createCharacteristic(
        CHARACTERISTIC_IR_RECEIVE_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    ir_receive_char->addDescriptor(new BLE2902());

    ESP_LOGD(logtag, "creating BLE ir send characteristic...");
    ir_send_char = pService->createCharacteristic(
        CHARACTERISTIC_IR_SEND_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    ir_send_char->setCallbacks(new Ir_send_callbacks());

    ESP_LOGD(logtag, "creating BLE latency characteristic...");
    latency_char = pService->createCharacteristic(
        CHARACTERISTIC_LATENCY_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    latency_char->addDescriptor(new BLE2902());

    ESP_LOGD(logtag, "creating BLE version characteristic...");
    version_char = pService->createCharacteristic(
        CHARACTERISTIC_VERSION_UUID,
        BLECharacteristic::PROPERTY_READ);
    version_char->setValue(GIT_TAG);

    ESP_LOGD(logtag, "starting BLE service...");
    pService->start();

    ESP_LOGD(logtag, "starting advertising...");
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    /* MAC address */
    BLEAddress mac_address = BLEDevice::getAddress();
    std::string mac_str = mac_address.toString();
    ESP_LOGI(logtag, "MAC address: %s", mac_str);
    return;
}

/**
 * @brief ble send data
 * Activates bluetooth low energy notify, with semaphore handlers.
 */
void ble_notify(BLECharacteristic *characteristic)
{
    const char *logtag = "BT";

    if (device_connected)
    {
        ESP_LOGD(logtag, "Sending BT...");
        //check if Mutex was successfully created
        if (xMutex_BT != NULL)
        {
            /* See if we can obtain the semaphore.  If the semaphore is not
      available wait 10 ms to see if it becomes free. */
            if (xSemaphoreTake(xMutex_BT, 10 / portTICK_PERIOD_MS) == pdTRUE)
            {
                /* We were able to obtain the semaphore and can now access the
          shared resource. */
                characteristic->notify();
                /* We have finished accessing the shared resource.  Release the
          semaphore. */
                xSemaphoreGive(xMutex_BT);
                ESP_LOGD(logtag, "BT successfully sent");
            }
            else
                ESP_LOGE(logtag, "BT Mutex locked for over 10ms, message possibly dropped");
        }
        else
            ESP_LOGE(logtag, "BT Mutex not available.");
    }
    else
    {
        ESP_LOGW(logtag, "No device connected, no message sent.");
    }

    return;
}
