#include <pthread.h>

#define DATA_BYTES_POS 2 //0 command byte, 1 address byte, 2 data bytes
#define DATA_BYTES_LENGHT 4 //sizeof unsinged long
#define BT_MESSAGE_LENGHT 6
#define COMMAND_WRITE 0x01
#define COMMAND_STREAM 0x02
#define COMMAND_READ 0x03
#define ADDR_SEND_BYTES	0x01
#define ADDR_SHOOT_COOLDOWN	0x02
#define ADDR_SHOOT_DELAY 0x03
#define ADDR_SHOOT_DURATION_MIN 0x04
#define ADDR_SHOOT_DURATION_MAX 0x05
#define ADDR_SHOOT_MODE	0x06
#define ADDR_BURST_AMOUNT_MIN	0x07
#define ADDR_BURST_AMOUNT_MAX	0x08
#define ADDR_BURST_COOLDOWN	0x09
#define ADDR_MAGAZINE_SIZE 0x0A
#define PIN_TRIGGER	5
//#define PIN_RELOAD
#define SEND_TRIGGER_STATUS_INTERVAL 100   //use this or when status change?
#define CHECK_INTERVAL_BT_IN_MS 100
#define CHECK_INTERVAL_IR_IN_MS	100
#define SHOOT_MODE_MANUAL 0x00000001
#define SHOOT_MODE_AUTO 0x00000002

typedef struct Shoot_config {

  //4 byte
	unsigned long	send_bytes = 0x00FFFFFF, //standard value without bt connection
	    cooldown_in_ms = 1000, //min 1000 ms break after end of shot
			trigger_delay_in_ms = 0,  //0 ms delay after triggershoot
			duration_min_in_ms = 100, //shot duration 100 ms
			duration_max_in_ms = 100,
			smode = SHOOT_MODE_MANUAL,
			burst_amount_min = 1, //one shot per trigger
			burst_amount_max = 1,
			burst_cooldown_in_ms = 0,  //no burst shot
      magazine_size = 0, //infinte
      current_magazine_level = 0;
} Shoot_config;
Shoot_config shootconf;

volatile boolean  trigger_status=false;

typedef enum {READY, STREAM} state_t ;
state_t state = READY;


pthread_t	thread_check_trigger,
		thread_read_bt,
		thread_bt_stream,
		thread_ir_stream;

//pthread_mutex_t ;
