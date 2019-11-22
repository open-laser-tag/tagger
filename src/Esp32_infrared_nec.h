/* NEC remote infrared RMT 
   Build from:
   https://github.com/espressif/esp-idf/blob/a45e9985344575a80acb1dc9c12e7bec4d8af401/examples/peripherals/rmt_nec_tx_rx/main/infrared_nec_main.c
*/
#ifndef Esp32_infrared_nec_h
#define Esp32_infrared_nec_h

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"

#define NEC_TAG "NEC"

//CHOOSE SELF TEST OR NORMAL TEST
#define RMT_RX_SELF_TEST   0

/******************************************************/
/*****                SELF TEST:                  *****/
/*Connect RMT_TX_GPIO_NUM with RMT_RX_GPIO_NUM        */
/*TX task will send NEC data with carrier disabled    */
/*RX task will print NEC data it receives.            */
/******************************************************/
#if RMT_RX_SELF_TEST
#define RMT_RX_ACTIVE_LEVEL  1   /*!< Data bit is active high for self test mode */
#define RMT_TX_CARRIER_EN    1   /*!< Disable carrier for self test mode  */
#else
//Test with infrared LED, we have to enable carrier for transmitter
//When testing via IR led, the receiver waveform is usually active-low.
#define RMT_RX_ACTIVE_LEVEL  0   /*!< If we connect with a IR receiver, the data is active low */
#define RMT_TX_CARRIER_EN    1   /*!< Enable carrier for IR transmitter test with IR led */
#endif

#define RMT_RX_CHANNEL    RMT_CHANNEL_7     /*!< RMT channel for receiver */
#define RMT_RX_GPIO_NUM  GPIO_NUM_27     /*!< GPIO number for receiver */
#define RMT_CLK_DIV      100    /*!< RMT counter clock divider */
#define RMT_TICK_10_US    (80000000/RMT_CLK_DIV/100000)   /*!< RMT counter value for 10 us.(Source clock is APB clock) */

#define NEC_HEADER_HIGH_US    9000                         /*!< NEC protocol header: positive 9ms */
#define NEC_HEADER_LOW_US     4500                         /*!< NEC protocol header: negative 4.5ms*/
#define NEC_BIT_ONE_HIGH_US    560                         /*!< NEC protocol data bit 1: positive 0.56ms */
#define NEC_BIT_ONE_LOW_US    (2250-NEC_BIT_ONE_HIGH_US)   /*!< NEC protocol data bit 1: negative 1.69ms */
#define NEC_BIT_ZERO_HIGH_US   560                         /*!< NEC protocol data bit 0: positive 0.56ms */
#define NEC_BIT_ZERO_LOW_US   (1120-NEC_BIT_ZERO_HIGH_US)  /*!< NEC protocol data bit 0: negative 0.56ms */
#define NEC_BIT_END            560                         /*!< NEC protocol end: positive 0.56ms */
#define NEC_BIT_MARGIN         20                          /*!< NEC parse margin time */

#define NEC_ITEM_DURATION(d)  ((d & 0x7fff)*10/RMT_TICK_10_US)  /*!< Parse duration time from memory register value */
#define NEC_DATA_ITEM_NUM   34  /*!< NEC code item number: header + 32bit data + end */
#define RMT_TX_DATA_NUM  1    /*!< NEC tx test data number */
#define rmt_item32_tIMEOUT_US  9500   /*!< RMT receiver timeout value(us) */


class Esp32_infrared_nec_tx {
    public:
    void init(rmt_channel_t rmt_channel, gpio_num_t gpio_num);
    void send(uint16_t cmd, uint16_t addr);

    private:
    void fill_item_level(rmt_item32_t* item, int high_us, int low_us);
    void fill_item_header(rmt_item32_t* item);
    void fill_item_bit_one(rmt_item32_t* item);
    void fill_item_bit_zero(rmt_item32_t* item);
    void fill_item_end(rmt_item32_t* item);
    int build_items(int channel, rmt_item32_t* item, int item_num, uint16_t addr, uint16_t cmd_data);

    rmt_channel_t _rmt_channel;
    gpio_num_t _gpio_num;
};

class Esp32_infrared_nec_rx {
    public:
    void init(rmt_channel_t rmt_channel, gpio_num_t gpio_num);

    private:
    inline bool check_in_range(int duration_ticks, int target_us, int margin_us);
    bool header_if(rmt_item32_t* item);
    bool bit_one_if(rmt_item32_t* item);
    bool bit_zero_if(rmt_item32_t* item);
    int parse_items(rmt_item32_t* item, int item_num, uint16_t* addr, uint16_t* data);
    void rmt_example_nec_rx_task(void *arg);
};

#endif